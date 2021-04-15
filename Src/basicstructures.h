#ifndef BASICSTRUCTURES_H
#define BASICSTRUCTURES_H

#include<vector>

struct Point
{
    int i, j;

    friend bool operator==(const Point &a, const Point &b);
    friend Point operator+(const Point &a, const Point &b);
    friend Point operator-(const Point &a, const Point &b);
};

struct Agent
{
    Point start;
    Point goal;
};

struct SafeInterval
{
    int start_time, end_time;
};

struct AgentAction
{
    int start_time;
    Point goal;
};

using Trajectory = std::vector<AgentAction>;

#endif
