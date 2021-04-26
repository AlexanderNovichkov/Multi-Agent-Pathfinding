#ifndef ASEARCH_HERUISTICS_H
#define ASEARCH_HERUISTICS_H

#include<memory>
#include "basicstructures.h"

class Heuristic
{
public:
    virtual double operator()(const Point &from, const Point &to) const = 0;
};


class DiagonalHeuristic : public Heuristic
{
public:
    double operator()(const Point &from, const Point &to) const override;
};

class ManhattanHeuristic : public Heuristic
{
public:
    double operator()(const Point &from, const Point &to) const override;
};

class EuclideanHeuristic : public Heuristic
{
public:
    double operator()(const Point &from, const Point &to) const override;
};

class ChebyshevHeuristic : public Heuristic
{
public:
    double operator()(const Point &from, const Point &to) const override;
};


class ZeroHeuristic : public Heuristic
{
public:
    double operator()(const Point &from, const Point &to) const override;
};

std::unique_ptr<Heuristic> getHeuristic(int metrictype);


#endif
