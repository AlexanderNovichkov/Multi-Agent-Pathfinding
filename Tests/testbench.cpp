#include "mission.h"
#include "tinyxml2.h"
#include <cassert>

bool getRealResults(const char *fileName, int *realCost, bool *realSolutionFound);


int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Error! Pathfinding task file (XML) is not specified!" << std::endl;
        return 0;
    }
    bool solution_found;
    int cost;
    if (!getRealResults(argv[1], &cost, &solution_found)) {
        return -1;
    }

    Mission mission(argv[1]);

    if (!mission.getMap()) {
        std::cerr << "Incorrect map! Program halted!" << std::endl;
        return -1;
    }

    if (!mission.getConfig()) {
        std::cerr << "Incorrect configurations! Program halted!" << std::endl;
        return -1;
    }

    if (!mission.createLog()) {
        std::cerr << "Log chanel has not been created! Program halted!" << std::endl;
        return -1;
    }

    mission.createEnvironmentOptions();
    mission.createSearch();
    mission.startSearch();
    SearchResult result = mission.getSearchResult();


    if (solution_found != result.solution_found) {
        std::cerr << "Incorrect solution_found result" << std::endl;
        return 1;
    }

    if (solution_found && cost != result.cost) {
        std::cerr << "Incorrect cost";
    }

    return 0;
}

bool getRealResults(const char *fileName, int *realCost, bool *realSolutionFound)
{
    tinyxml2::XMLDocument doc;

    int cost = 0.0f;
    bool solution_found = false;

    // Load XML File
    if (doc.LoadFile(fileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cerr << "Error opening XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) {
        std::cerr << "Error! No '" << CNS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *log = root->FirstChildElement(CNS_TAG_LOG);
    if (!log) {
        std::cerr << "Error! No '" << CNS_TAG_LOG << "' tag found in XML file!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement *sum = log->FirstChildElement(CNS_TAG_SUM);
    if (!sum) {
        std::cerr << "Error! No '" << CNS_TAG_SUM << "' tag found in XML file!" << std::endl;
        return false;
    }

    if (sum->QueryIntAttribute(CNS_TAG_ATTR_COST, &cost) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error! No '" << CNS_TAG_ATTR_COST << "' tag found in XML file!" << std::endl;
        return false;
    }

    if (sum->QueryBoolAttribute(CNS_TAG_ATTR_SOLUTION_FOUND, &solution_found) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error! No '" << CNS_TAG_ATTR_SOLUTION_FOUND << "' tag found in XML file!" << std::endl;
        return false;
    }

    *realSolutionFound = solution_found;
    *realCost = cost;

    return true;
}
