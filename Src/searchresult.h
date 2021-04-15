#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <list>
#include "basicstructures.h"

struct AgentSearchResult
{
    bool trajectory_found = false;
    int trajectory_time = 0;
    Trajectory trajectory;
    unsigned int nodescreated = 0;
    unsigned int numberofsteps = 0;

    double search_time = 0;
};

struct SearchResult
{
    std::vector<AgentSearchResult> agents_sresult;
    bool solution_found = false;
    int cost = 0;
    double total_time = 0;
};

#endif
