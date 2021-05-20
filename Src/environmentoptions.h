#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H

#include "gl_const.h"

class EnvironmentOptions {
public:
    EnvironmentOptions(int AL, double HW, int AP, int BS, int BT, int MT, bool TP, double TL);

    EnvironmentOptions();

    int algorithm;
    double hweight;
    int agentpriority;
    int blockstart;
    int breakingties;
    int metrictype;     //Can be chosen Euclidean, Manhattan, Chebyshev and Diagonal distance
    bool tryotherpriority;
    double runtimelimit;
};

#endif // ENVIRONMENTOPTIONS_H
