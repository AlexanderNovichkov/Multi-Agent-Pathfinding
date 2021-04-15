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
                                   const std::unordered_map<Edge, std::set<int>, EdgeHasher> &edges);

    int getMinTimeWithoutSwapConflicts(const Point &a, const Point &b, int start_time, int end_time,
                                       const std::unordered_map<Edge, std::set<int>, EdgeHasher> &edges) const;

    void updateSafeIntervals(const Point &startPoint, const Trajectory &trajectory,
                             std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals);


    void updateEdges(const Point &startPoint, const Trajectory &trajectory,
                     std::unordered_map<Edge, std::set<int>, EdgeHasher> &edges);

    AgentSearchResult
    findAgentTrajectory(int agentId, const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                        std::unordered_map<Point, SafeIntervalsContainer, PointHasher> &point_to_intervals,
                        const std::unordered_map<Edge, std::set<int>, EdgeHasher> &edges);

    SearchResult findTrajectories(const Map &map, const EnvironmentOptions &options, const Heuristic &heuristic,
                                  const std::vector<int> &order);


    SearchResult sresult; //This will store the search result
};


#endif
