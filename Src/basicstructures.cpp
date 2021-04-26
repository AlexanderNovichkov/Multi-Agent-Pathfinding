#include "basicstructures.h"

bool operator==(const Point &a, const Point &b)
{
    return (a.i == b.i) && (a.j == b.j);
}

Point operator+(const Point &a, const Point &b)
{
    return {a.i + b.i, a.j + b.j};
}

Point operator-(const Point &a, const Point &b)
{
    return {a.i - b.i, a.j - b.j};
}

AgentAction::AgentAction() = default;

AgentAction::AgentAction(int start_time, const Point &from, const Point &to)
        : start_time(start_time), from(from), to(to)
{
}

