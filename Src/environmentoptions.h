#ifndef ENVIRONMENTOPTIONS_H
#define ENVIRONMENTOPTIONS_H

#include "gl_const.h"

class EnvironmentOptions {
public:
    EnvironmentOptions(int AL, double HW, int AP, int BS, int BT, int MT);

    EnvironmentOptions();

    int algorithm;
    double hweight;
    int agentpriority;
    int blockstart;
    int breakingties;
    int metrictype;     //Can be chosen Euclidean, Manhattan, Chebyshev and Diagonal distance
};

#endif // ENVIRONMENTOPTIONS_H
