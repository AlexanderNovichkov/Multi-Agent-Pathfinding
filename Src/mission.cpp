#include "mission.h"
#include <iostream>

Mission::Mission()
{
    logger = nullptr;
    fileName = nullptr;
}

Mission::Mission(const char *FileName)
{
    fileName = FileName;
    logger = nullptr;
}

Mission::~Mission()
{
    if (logger)
        delete logger;
}

bool Mission::getMap()
{
    return map.getMap(fileName);
}

bool Mission::getConfig()
{
    return config.getConfig(fileName);
}

bool Mission::createLog()
{
    if (logger != nullptr) delete logger;
    logger = new XmlLogger(config.LogParams[CN_LP_LEVEL]);
    return logger->getLog(fileName, config.LogParams);
}

void Mission::createEnvironmentOptions()
{
    options.algorithm = config.SearchParams[CN_SP_ST];

    options.hweight = config.SearchParams[CN_SP_HW];
    options.breakingties = config.SearchParams[CN_SP_BT];
    options.metrictype = config.SearchParams[CN_SP_MT];
    options.agentpriority = config.SearchParams[CN_SP_AP];
    options.blockstart = config.SearchParams[CN_SP_BS];
}

void Mission::createSearch()
{
//might be helpful in case numerous algorithms are added
}

void Mission::startSearch()
{
    sr = search.startSearch(logger, map, options);
}

void Mission::printSearchResultsToConsole() const
{
    std::cout << "Solution is" << (sr.solution_found ? "" : " NOT") << " found" << std::endl;
    std::cout << "Cost = " << sr.cost << std::endl;
    std::cout << "Total time = " << sr.total_time << std::endl;
    for (size_t i = 0; i < sr.agents_sresult.size(); i++) {
        const AgentSearchResult &asr = sr.agents_sresult[i];
        std::cout << std::endl << "Agent " << i + 1 << ":" << std::endl;
        std::cout << "trajectory is" << (asr.trajectory_found ? "" : " NOT") << " found!" << std::endl;
        std::cout << "numberofsteps = " << asr.numberofsteps << std::endl;
        std::cout << "nodescreated = " << asr.nodescreated << std::endl;
        if (asr.trajectory_found) {
            std::cout << "trajectory_execution_time = " << asr.trajectory_time << std::endl;
        }
        std::cout << "time = " << asr.search_time << std::endl;
    }
}

void Mission::saveSearchResultsToLog()
{
    logger->writeToLogAgentsSresult(sr);
    logger->saveLog();
}

SearchResult Mission::getSearchResult()
{
    return sr;
}

