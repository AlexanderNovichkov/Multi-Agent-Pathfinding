#ifndef SEARCH_H
#define SEARCH_H

#include "ilogger.h"
#include "searchresult.h"
#include "environmentoptions.h"
#include "searchstructures.h"
#include "heuristics.h"
#include <list>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>
#include <unordered_map>
#include <unordered_set>


class Search
{
public:
    Search();

    ~Search(void);

    SearchResult startSearch(ILogger *Logger, const Map &Map, const EnvironmentOptions &options);

protected:

    void sortByHeuristic(std::vector<int> *order, const Heuristic &heuristic, const Map &map, bool rev);

    Trajectory makeTrajectory(const Node *node);

    std::vector<Node> getSucessors(Node *parentNode, const Point &goal,
                                   const Map &map, const Heuristic &heuristic,
                                   std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals,
                                   const std::unordered_set<AgentAction, ActionHasher> &actions);

    int getMinTimeWithoutSwapConflicts(const Point &a, const Point &b, int start_time, int end_time,
                                       const std::unordered_set<AgentAction, ActionHasher> &actions) const;

    void updateSafeIntervals(const Point &startPoint, const Trajectory &trajectory,
                             std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals);


    void updateActions(const Point &startPoint, const Trajectory &trajectory,
                       std::unordered_set<AgentAction, ActionHasher> *actions);

    AgentSearchResult
    findAgentTrajectory(int agentId, const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                        std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals,
                        const std::unordered_set<AgentAction, ActionHasher> &actions);

    SearchResult findTrajectories(const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                                  const std::vector<int> &order);


    SearchResult sresult; //This will store the search result
};


#endif
