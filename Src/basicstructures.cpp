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