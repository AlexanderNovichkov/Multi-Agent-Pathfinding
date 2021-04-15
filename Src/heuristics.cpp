#include <cmath>
#include<memory>

#include "heuristics.h"
#include "gl_const.h"

Heuristic::Heuristic(double weight) : weight(weight){};
DiagonalHeuristic::DiagonalHeuristic(double weight) : Heuristic(weight){}
ManhattanHeuristic::ManhattanHeuristic(double weight) : Heuristic(weight){}
EuclideanHeuristic::EuclideanHeuristic(double weight) : Heuristic(weight){}
ChebyshevHeuristic::ChebyshevHeuristic(double weight) : Heuristic(weight){}
ZeroHeuristic::ZeroHeuristic(double weight) : Heuristic(weight){}

double Heuristic::operator()(const Point &from, const Point &to) const
{
    return weight * compute(from, to);
}


double DiagonalHeuristic::compute(const Point &from, const Point &to) const
{
    Point d = to - from;
    return abs(d.i - d.j) + CN_SQRT_TWO * std::min(d.i, d.j);
}

double ManhattanHeuristic::compute(const Point &from, const Point &to) const
{
    Point d = to - from;
    return d.i + d.j;
}

double EuclideanHeuristic::compute(const Point &from, const Point &to) const
{
    Point d = to - from;
    return sqrt(d.i * d.i + d.j * d.j);
}

double ChebyshevHeuristic::compute(const Point &from, const Point &to) const
{
    Point d = to - from;
    return std::max(d.i, d.j);
}

double ZeroHeuristic::compute(const Point &from, const Point &to) const
{
    return 0;
}

std::unique_ptr<Heuristic> getHeuristic(int metrictype, double weight)
{
    switch (metrictype) {
        case CN_SP_MT_DIAG:
            return std::unique_ptr<DiagonalHeuristic>(new DiagonalHeuristic(weight));
        case CN_SP_MT_MANH:
            return std::unique_ptr<ManhattanHeuristic>(new ManhattanHeuristic(weight));
        case CN_SP_MT_EUCL:
            return std::unique_ptr<EuclideanHeuristic>(new EuclideanHeuristic(weight));
        case CN_SP_MT_CHEB:
            return std::unique_ptr<ChebyshevHeuristic>(new ChebyshevHeuristic(weight));
        case CN_SP_MT_ZERO:
            return std::unique_ptr<ZeroHeuristic>(new ZeroHeuristic(weight));
    }

    throw std::invalid_argument("Wrong metrictype");
}