#include "environmentoptions.h"

EnvironmentOptions::EnvironmentOptions()
{
    metrictype = CN_SP_MT_EUCL;
}

EnvironmentOptions::EnvironmentOptions(int AL, double HW, int AP, int BS, int BT, int MT, bool TP)
        : algorithm(AL),
          hweight(HW),
          agentpriority(AP),
          blockstart(BS),
          breakingties(BT),
          metrictype(MT),
          tryotherpriority(TP)
{}

