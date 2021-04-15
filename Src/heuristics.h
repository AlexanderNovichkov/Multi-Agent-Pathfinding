#ifndef ASEARCH_HERUISTICS_H
#define ASEARCH_HERUISTICS_H

#include<memory>
#include "basicstructures.h"

class Heuristic
{
public:
    Heuristic(double weight);

    // with weight
    double operator()(const Point &from, const Point &to) const;

protected:
    // without weight
    virtual double compute(const Point &from, const Point &to) const = 0;

private:
    double weight;
};


class DiagonalHeuristic : public Heuristic
{
public:
    DiagonalHeuristic(double weight);

    double compute(const Point &from, const Point &to) const override;
};

class ManhattanHeuristic : public Heuristic
{
public:
    ManhattanHeuristic(double weight);

    double compute(const Point &from, const Point &to) const override;
};

class EuclideanHeuristic : public Heuristic
{
public:
    EuclideanHeuristic(double weight);

    double compute(const Point &from, const Point &to) const override;
};

class ChebyshevHeuristic : public Heuristic
{
public:
    ChebyshevHeuristic(double weight);

    double compute(const Point &from, const Point &to) const override;
};


class ZeroHeuristic : public Heuristic
{
public:
    ZeroHeuristic(double weight);

    double compute(const Point &from, const Point &to) const override;
};

std::unique_ptr<Heuristic> getHeuristic(int metrictype, double weight);


#endif
