#include "searchstructures.h"


// Edge
std::size_t EdgeHasher::operator()(const Edge &e) const
{
    return PointHasher()(e.from) ^ PointHasher()(e.to);
}

bool operator==(const Edge &a, const Edge &b)
{
    return (a.from == b.from) && (a.to == b.to);
}

// State
bool operator==(const State &a, const State &b)
{
    return (a.pos == b.pos) && (a.interval == b.interval);
}

std::size_t StateHasher::operator()(const State &state) const
{
    return PointHasher()(state.pos) ^ SafeIntervalHasher()(state.interval);
}

// SafeInterval
bool operator<(const SafeInterval &a, const SafeInterval &b)
{
    return a.start_time < b.start_time;
}

bool operator==(const SafeInterval &a, const SafeInterval &b)
{
    return (a.start_time == b.start_time) && (a.end_time == b.end_time);
}

std::size_t SafeIntervalHasher::operator()(const SafeInterval &interval) const
{
    return (uint64_t) (interval.start_time) | (uint64_t(interval.end_time) << 32u);
}


// Point
std::size_t PointHasher::operator()(const Point &p) const
{
    return (uint64_t) (p.i) | (uint64_t(p.j) << 32u);
}


SafeIntervalsContainer::SafeIntervalsContainer(): std::set<SafeInterval>({SafeInterval{0, MAX_TIME}})
{
}

void SafeIntervalsContainer::eraseIntervals(int startTime, int endTime)
{
    std::vector<SafeInterval> intersected_intervals;
    // slow version, upgrade later
    for (auto interval : (*this)) {
        if (std::max(startTime, interval.start_time) <= std::min(endTime, interval.end_time)) {
            intersected_intervals.push_back(interval);
        }
    }

    for (auto interval : intersected_intervals) {
        int left = std::max(interval.start_time, startTime);
        int right = std::min(interval.end_time, endTime);

        this->erase(interval);

        if (interval.start_time < left) {
            this->insert(SafeInterval{interval.start_time, left - 1});
        }
        if (right < interval.end_time) {
            this->insert(SafeInterval{right + 1, interval.end_time});
        }
    }
}


bool Comp::operator()(const Node *a, const Node *b) const
{
    if (a->F != b->F) {
        return a->F < b->F;
    }

    if (a->g != b->g) {
        if (breakingties == CN_SP_BT_GMIN) {
            return a->g < b->g;
        } else {
            return a->g > b->g;
        }
    }

    if (a->state.pos.i != b->state.pos.i) {
        return a->state.pos.i < b->state.pos.i;
    }
    return (a->state.pos.j < b->state.pos.j);
}

Comp::Comp(bool breakingties) : breakingties(breakingties)
{}


OpenContainer::OpenContainer(bool breakingties) : sorted_nodes(Comp(breakingties))
{}

void OpenContainer::updateNode(const Node &node)
{
    State state = node.state;

    auto it = state_to_node.find(state);
    if (it != state_to_node.end()) {
        if (node.F < it->second.F) {
            sorted_nodes.erase(&(it->second));
            it->second = node;
            sorted_nodes.insert(&(it->second));
        }
    } else {
        state_to_node[state] = node;
        sorted_nodes.insert(&state_to_node[state]);
    }
}

Node OpenContainer::extractMinNode()
{
    Node *nodeptr = *sorted_nodes.begin();
    sorted_nodes.erase(sorted_nodes.begin());

    State state = nodeptr->state;

    Node node = state_to_node[state];
    state_to_node.erase(state);
    return node;
}

size_t OpenContainer::size() const
{
    return sorted_nodes.size();
}
