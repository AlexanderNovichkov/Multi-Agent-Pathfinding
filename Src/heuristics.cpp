#include <cmath>
#include<memory>

#include "heuristics.h"
#include "gl_const.h"


double DiagonalHeuristic::operator()(const Point &from, const Point &to) const
{
    Point d = to - from;
    return abs(d.i - d.j) + CN_SQRT_TWO * std::min(d.i, d.j);
}

double ManhattanHeuristic::operator()(const Point &from, const Point &to) const
{
    Point d = to - from;
    return d.i + d.j;
}

double EuclideanHeuristic::operator()(const Point &from, const Point &to) const
{
    Point d = to - from;
    return sqrt(d.i * d.i + d.j * d.j);
}

double ChebyshevHeuristic::operator()(const Point &from, const Point &to) const
{
    Point d = to - from;
    return std::max(d.i, d.j);
}

double ZeroHeuristic::operator()(const Point &from, const Point &to) const
{
    return 0;
}

std::unique_ptr<Heuristic> getHeuristic(int metrictype)
{
    switch (metrictype) {
        case CN_SP_MT_DIAG:
            return std::unique_ptr<DiagonalHeuristic>(new DiagonalHeuristic());
        case CN_SP_MT_MANH:
            return std::unique_ptr<ManhattanHeuristic>(new ManhattanHeuristic());
        case CN_SP_MT_EUCL:
            return std::unique_ptr<EuclideanHeuristic>(new EuclideanHeuristic());
        case CN_SP_MT_CHEB:
            return std::unique_ptr<ChebyshevHeuristic>(new ChebyshevHeuristic());
        case CN_SP_MT_ZERO:
            return std::unique_ptr<ZeroHeuristic>(new ZeroHeuristic());
    }

    throw std::invalid_argument("Wrong metrictype");
}