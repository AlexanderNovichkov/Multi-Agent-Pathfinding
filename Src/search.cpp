#include "search.h"
#include "basicstructures.h"
#include "searchstructures.h"
#include <algorithm>
#include <memory>
#include <chrono>
#include<numeric>
#include<random>
#include <unordered_set>

#include "gl_const.h"
#include "heuristics.h"

double getDurationInSeconds(const std::chrono::time_point<std::chrono::steady_clock> &start_time)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time).count()
           / 1000000.0;
}


Search::Search()
{
//set defaults here
}

Search::~Search()
{}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    const auto start_time = std::chrono::steady_clock::now();
    const std::unique_ptr<Heuristic> heuristic = getHeuristic(options.metrictype);
    std::vector<int> order(map.getAgentsCount());
    std::iota(order.begin(), order.end(), 0);

    // try to find solution with specified heuristic
    if (options.agentpriority == CN_SP_AP_RAND) {
        std::mt19937 generator(std::time(nullptr));
        std::shuffle(order.begin(), order.end(), generator);
    } else if (options.agentpriority == CN_SP_AP_HRMIN) {
        sortByHeuristic(&order, *heuristic, map, false);
    } else if (options.agentpriority == CN_SP_AP_HRMAX) {
        sortByHeuristic(&order, *heuristic, map, true);
    }

    sresult = findTrajectories(map, options, *heuristic, order);
    if (sresult.solution_found || !options.tryotherpriority) {
        sresult.total_time = getDurationInSeconds(start_time);
        return sresult;
    }

    // try other priorities if solution not found
    if (options.agentpriority != CN_SP_AP_HRMAX) {
        sortByHeuristic(&order, *heuristic, map, true);
        SearchResult cursresult = findTrajectories(map, options, *heuristic, order);
        if (cursresult.solution_found) {
            sresult = std::move(cursresult);
            return sresult;
        }
    }

    if (options.agentpriority != CN_SP_AP_HRMIN) {
        sortByHeuristic(&order, *heuristic, map, false);
        SearchResult cursresult = findTrajectories(map, options, *heuristic, order);
        if (cursresult.solution_found) {
            sresult = std::move(cursresult);
            sresult.total_time = getDurationInSeconds(start_time);
            return sresult;
        }
    }

    if (options.agentpriority != CN_SP_AP_RAND) {
        std::mt19937 generator(std::time(nullptr));
        std::shuffle(order.begin(), order.end(), generator);
        SearchResult cursresult = findTrajectories(map, options, *heuristic, order);
        if (cursresult.solution_found) {
            sresult = std::move(cursresult);
            sresult.total_time = getDurationInSeconds(start_time);
            return sresult;
        }
    }

    sresult.total_time = getDurationInSeconds(start_time);
    return sresult;
}


SearchResult Search::findTrajectories(const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                                      const std::vector<int> &order)
{
    std::unordered_map<Point, SafeIntervalsContainer, PointHasher> point_to_intervals;

    std::unordered_set<AgentAction, ActionHasher> actions;
    SearchResult cursresult;

    if (options.blockstart > 0) {
        for (const Agent &agent : map.getAgents()) {
            point_to_intervals[agent.start] = {SafeInterval{options.blockstart, MAX_TIME}};
        }
    }

    cursresult.agents_sresult.resize(map.getAgentsCount());
    cursresult.solution_found = true;

    for (auto agent_id : order) {
        if (options.blockstart > 0) {
            point_to_intervals[map.getAgent(agent_id).start].insert(SafeInterval{0, options.blockstart - 1});
        }
        cursresult.agents_sresult[agent_id] = findAgentTrajectory(agent_id, map, options, heuristic,
                                                                  point_to_intervals, actions);
        if (!cursresult.agents_sresult[agent_id].trajectory_found) {
            cursresult.solution_found = false;
        } else {
            cursresult.cost += cursresult.agents_sresult[agent_id].trajectory_time;
        }


        updateSafeIntervals(map.getAgents()[agent_id].start, cursresult.agents_sresult[agent_id].trajectory,
                            point_to_intervals);
        updateActions(map.getAgents()[agent_id].start, cursresult.agents_sresult[agent_id].trajectory, &actions);
    }

    return cursresult;
}


AgentSearchResult
Search::findAgentTrajectory(int agentId, const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                            std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals,
                            const std::unordered_set<AgentAction, ActionHasher> &actions)
{
    if (point_to_intervals[map.getAgents()[agentId].goal].empty() ||
        point_to_intervals[map.getAgents()[agentId].start].empty()) {
        return AgentSearchResult{};
    }

    const auto start_time = std::chrono::steady_clock::now();

    State start_state{};
    start_state.pos = map.getAgents()[agentId].start;
    start_state.interval = *point_to_intervals[start_state.pos].begin();

    State goal_state{};
    goal_state.pos = map.getAgents()[agentId].goal;
    goal_state.interval = *point_to_intervals[goal_state.pos].rbegin();

    double start_node_heuristic = heuristic(start_state.pos, goal_state.pos);

    OpenContainer open(options.breakingties);
    std::unordered_map<State, Node, StateHasher> close;

    open.updateNode(Node{start_state, start_node_heuristic, 0, start_node_heuristic, nullptr});
    Node *goal_node = nullptr;

    while (open.size() > 0) {
        Node expanding_node = open.extractMinNode();
        close[expanding_node.state] = expanding_node;

        Node *cur_node_ptr = &close[expanding_node.state];

        if (cur_node_ptr->state == goal_state) {
            goal_node = cur_node_ptr;
            break;
        }

        std::vector<Node> sucessors = getSucessors(cur_node_ptr, goal_state.pos, map, heuristic,
                                                   point_to_intervals, actions);
        for (const Node &s : sucessors) {
            if (close.count(s.state)) {
                continue;
            }

            open.updateNode(s);
        }
    }

    AgentSearchResult asresult;
    asresult.trajectory_found = (goal_node != nullptr);
    if (asresult.trajectory_found) {
        asresult.trajectory = makeTrajectory(goal_node);
    }

    asresult.nodescreated = open.size() + close.size();
    asresult.numberofsteps = (int) (close.size()) - 1;
    asresult.trajectory_time = asresult.trajectory_found ? goal_node->g : 0;
    asresult.search_time = getDurationInSeconds(start_time);

    return asresult;
}

std::vector<Node> Search::getSucessors(Node *parentNode, const Point &goal, const Map &map, const Heuristic &heuristic,
                                       std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals,
                                       const std::unordered_set<AgentAction, ActionHasher> &actions)
{
    static const Point diff[4] = {{0,  -1},
                                  {0,  1},
                                  {-1, 0},
                                  {1,  0}};
    std::vector<Node> sucessors;

    for (const Point &d : diff) {
        Point pnew = parentNode->state.pos + d;
        if (!map.CellOnGrid(pnew) || map.CellIsObstacle(pnew)) {
            continue;
        }

        for (auto interval : point_to_intervals[pnew]) {
            int vertex_min_time = std::max(parentNode->g, interval.start_time - 1);
            int vertex_max_time = std::min(parentNode->state.interval.end_time, interval.end_time - 1);
            // no vertex conflicts in [vertex_min_time, vertex_max_time]
            if (vertex_min_time <= vertex_max_time) {
                int min_time = getMinTimeWithoutSwapConflicts(parentNode->state.pos, pnew,
                                                              vertex_min_time, vertex_max_time, actions);

                if (min_time == -1) {
                    continue;
                }

                double h = heuristic(pnew, goal);
                sucessors.push_back(Node{});
                Node &child = sucessors.back();
                child.state = {pnew, interval};
                child.h = heuristic(pnew, goal);
                child.g = min_time + 1;
                child.F = h + child.g;
                child.parent = parentNode;
            }
        }
    }

    return sucessors;
}

Trajectory Search::makeTrajectory(const Node *node)
{
    Trajectory trajectory;
    while (node->parent != nullptr) {
        trajectory.push_back(AgentAction(node->g - 1, node->parent->state.pos, node->state.pos));
        node = node->parent;
    }
    std::reverse(trajectory.begin(), trajectory.end());
    return trajectory;
}

void Search::updateSafeIntervals(const Point &startPoint, const Trajectory &trajectory,
                                 std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals)
{
    Point p = startPoint;
    int start_time = 0;
    for (const AgentAction action : trajectory) {
        int end_time = action.start_time;
        point_to_intervals[p].eraseIntervals(start_time, end_time);
        p = action.to;
        start_time = end_time + 1;
    }
    point_to_intervals[p].eraseIntervals(start_time, MAX_TIME);
}


void Search::updateActions(const Point &startPoint, const Trajectory &trajectory,
                           std::unordered_set<AgentAction, ActionHasher> *actions)
{
    for (const AgentAction action : trajectory) {
        actions->insert(action);
    }
}

int Search::getMinTimeWithoutSwapConflicts(const Point &a, const Point &b, int start_time, int end_time,
                                           const std::unordered_set<AgentAction, ActionHasher> &actions) const
{
    AgentAction action;
    action.from = b;
    action.to = a;
    for (int t = start_time; t <= end_time; t++) {
        action.start_time = start_time;
        if (actions.find(action) == actions.end()) {
            return t;
        }
    }

    return -1;
}

void Search::sortByHeuristic(std::vector<int> *order, const Heuristic &heuristic, const Map &map, bool rev)
{
    auto cmp = [&heuristic, &map](int a, int b) {
        double ha = heuristic(map.getAgent(a).start, map.getAgent(a).goal);
        double hb = heuristic(map.getAgent(b).start, map.getAgent(b).goal);
        return ha < hb;
    };
    if (!rev) {
        std::sort(order->begin(), order->end(), cmp);
    } else {
        std::sort(order->rbegin(), order->rend(), cmp);
    }
}
