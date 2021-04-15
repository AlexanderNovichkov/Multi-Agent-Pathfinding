#include "map.h"


Map::Map()
{
    height = -1;
    width = -1;
    Grid = nullptr;
    cellSize = 1;
}

Map::~Map()
{
    if (Grid) {
        for (int i = 0; i < height; ++i)
            delete[] Grid[i];
        delete[] Grid;
    }
}

bool Map::CellIsTraversable(const Point &p) const
{
    return (Grid[p.i][p.j] == CN_GC_NOOBS);
}

bool Map::CellIsObstacle(const Point &p) const
{
    return (Grid[p.i][p.j] != CN_GC_NOOBS);
}

bool Map::CellOnGrid(const Point &p) const
{
    return (p.i < height && p.i >= 0 && p.j < width && p.j >= 0);
}

bool Map::getMap(const char *FileName)
{
    int rowiter = 0, grid_i = 0, grid_j = 0;

    tinyxml2::XMLElement *root = nullptr, *map = nullptr, *element = nullptr, *mapnode;

    std::string value;
    std::stringstream stream;

    bool hasGridMem = false, hasGrid = false, hasHeight = false, hasWidth = false, hasCellSize = false, hasAgents = false;

    tinyxml2::XMLDocument doc;

    // Load XML File
    if (doc.LoadFile(FileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cout << "Error opening XML file!" << std::endl;
        return false;
    }
    // Get ROOT element
    root = doc.FirstChildElement(CNS_TAG_ROOT);
    if (!root) {
        std::cout << "Error! No '" << CNS_TAG_ROOT << "' tag found in XML file!" << std::endl;
        return false;
    }

    // Get MAP element
    map = root->FirstChildElement(CNS_TAG_MAP);
    if (!map) {
        std::cout << "Error! No '" << CNS_TAG_MAP << "' tag found in XML file!" << std::endl;
        return false;
    }

    for (mapnode = map->FirstChildElement(); mapnode; mapnode = mapnode->NextSiblingElement()) {
        element = mapnode->ToElement();
        value = mapnode->Value();
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        stream.str("");
        stream.clear();

        if (element->GetText()) {
            stream << element->GetText();
        }


        if (!hasGridMem && hasHeight && hasWidth) {
            Grid = new int *[height];
            for (int i = 0; i < height; ++i)
                Grid[i] = new int[width];
            hasGridMem = true;
        }

        if (value == CNS_TAG_HEIGHT) {
            if (hasHeight) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_HEIGHT << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_HEIGHT << "' =" << height << "will be used."
                          << std::endl;
            } else {
                if (!((stream >> height) && (height > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_HEIGHT
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_HEIGHT << "' tag should be an integer >=0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_HEIGHT
                              << "' tag will be encountered later..." << std::endl;
                } else
                    hasHeight = true;
            }
        } else if (value == CNS_TAG_WIDTH) {
            if (hasWidth) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_WIDTH << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_WIDTH << "' =" << width << "will be used." << std::endl;
            } else {
                if (!((stream >> width) && (width > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_WIDTH
                              << "' tag encountered (or could not convert to integer)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_WIDTH << "' tag should be an integer AND >0" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_WIDTH
                              << "' tag will be encountered later..." << std::endl;
                } else
                    hasWidth = true;
            }
        } else if (value == CNS_TAG_CELLSIZE) {
            if (hasCellSize) {
                std::cout << "Warning! Duplicate '" << CNS_TAG_CELLSIZE << "' encountered." << std::endl;
                std::cout << "Only first value of '" << CNS_TAG_CELLSIZE << "' =" << cellSize << "will be used."
                          << std::endl;
            } else {
                if (!((stream >> cellSize) && (cellSize > 0))) {
                    std::cout << "Warning! Invalid value of '" << CNS_TAG_CELLSIZE
                              << "' tag encountered (or could not convert to double)." << std::endl;
                    std::cout << "Value of '" << CNS_TAG_CELLSIZE
                              << "' tag should be double AND >0. By default it is defined to '1'" << std::endl;
                    std::cout << "Continue reading XML and hope correct value of '" << CNS_TAG_CELLSIZE
                              << "' tag will be encountered later..." << std::endl;
                } else
                    hasCellSize = true;
            }
        } else if (value == CNS_TAG_AGENTS) {
            if (hasAgents) {
                std::cout << "Warning! Duplicate of tag'" << CNS_TAG_AGENTS << "' encountered." << std::endl;
                std::cout << "Only first value will be used." << std::endl;
            } else {
                hasAgents = true;
                element = mapnode->FirstChildElement();
                while (element != nullptr) {
                    std::string tag_name = element->Value();
                    std::transform(tag_name.begin(), tag_name.end(), tag_name.begin(), ::tolower);
                    if (tag_name != CNS_TAG_AGENTDATA) {
                        std::cout << "ERROR! Expected " << CNS_TAG_AGENTDATA << " but got " << tag_name << " !"
                                  << std::endl;
                        return false;
                    }

                    Agent agent{};
                    tinyxml2::XMLError error1 = element->QueryIntAttribute(CNS_TAG_ATTR_STX, &agent.start.j);
                    tinyxml2::XMLError error2 = element->QueryIntAttribute(CNS_TAG_ATTR_STY, &agent.start.i);
                    tinyxml2::XMLError error3 = element->QueryIntAttribute(CNS_TAG_ATTR_FINX, &agent.goal.j);
                    tinyxml2::XMLError error4 = element->QueryIntAttribute(CNS_TAG_ATTR_FINY, &agent.goal.i);

                    if (error1 != tinyxml2::XMLError::XML_SUCCESS || error2 != tinyxml2::XMLError::XML_SUCCESS ||
                        error3 != tinyxml2::XMLError::XML_SUCCESS || error4 != tinyxml2::XMLError::XML_SUCCESS) {
                        std::cout << "Error! No start or finish position of agent " << agents.size() + 1
                                  << "!"
                                  << std::endl;
                        return false;
                    }

                    agents.push_back(agent);
                    element = element->NextSiblingElement();
                }
            }
        } else if (value == CNS_TAG_GRID) {
            hasGrid = true;
            if (!(hasHeight && hasWidth)) {
                std::cout << "Error! No '" << CNS_TAG_WIDTH << "' tag or '" << CNS_TAG_HEIGHT << "' tag before '"
                          << CNS_TAG_GRID << "'tag encountered!" << std::endl;
                return false;
            }
            element = mapnode->FirstChildElement();
            while (grid_i < height) {
                if (!element) {
                    std::cout << "Error! Not enough '" << CNS_TAG_ROW << "' tags inside '" << CNS_TAG_GRID
                              << "' tag."
                              << std::endl;
                    std::cout << "Number of '" << CNS_TAG_ROW
                              << "' tags should be equal (or greater) than the value of '" << CNS_TAG_HEIGHT
                              << "' tag which is " << height << std::endl;
                    return false;
                }
                std::string str = element->GetText();
                std::vector<std::string> elems;
                std::stringstream ss(str);
                std::string item;
                while (std::getline(ss, item, ' '))
                    elems.push_back(item);
                rowiter = grid_j = 0;
                int val;
                if (elems.size() > 0)
                    for (grid_j = 0; grid_j < width; ++grid_j) {
                        if (grid_j == elems.size())
                            break;
                        stream.str("");
                        stream.clear();
                        stream << elems[grid_j];
                        stream >> val;
                        Grid[grid_i][grid_j] = val;
                    }

                if (grid_j != width) {
                    std::cout << "Invalid value on " << CNS_TAG_GRID << " in the " << grid_i + 1 << " "
                              << CNS_TAG_ROW
                              << std::endl;
                    return false;
                }
                ++grid_i;

                element = element->NextSiblingElement();
            }
        }
    }
    //some additional checks
    if (!hasGrid) {
        std::cout << "Error! There is no tag 'grid' in xml-file!\n";
        return false;
    }

    if (agents.empty()) {
        std::cout << "Error! There is no agents in xml-file!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < agents.size(); i++) {
        if (!CellOnGrid(agents[i].start)) {
            std::cout << "Error! Start cell of agent " << i + 1 << " is not on grid!" << std::endl;
            return false;
        }
        if (!CellOnGrid(agents[i].goal)) {
            std::cout << "Error! Finish cell of agent " << i + 1 << " is not on grid!" << std::endl;
            return false;
        }

        if (getValue(agents[i].start) != CN_GC_NOOBS) {
            std::cout << "Error! Start cell of agent " << i + 1 << " is not traversable!" << std::endl;
            return false;
        }
        if (getValue(agents[i].goal) != CN_GC_NOOBS) {
            std::cout << "Error! Finish cell of agent " << i + 1 << " is not traversable!" << std::endl;
            return false;
        }
    }


    return true;
}


int Map::getValue(const Point &p) const
{
    if (p.i < 0 || p.i >= height)
        return -1;

    if (p.j < 0 || p.j >= width)
        return -1;

    return Grid[p.i][p.j];
}

int Map::getMapHeight() const
{
    return height;
}

int Map::getMapWidth() const
{
    return width;
}

double Map::getCellSize() const
{
    return cellSize;
}

const std::vector<Agent> &Map::getAgents() const
{
    return agents;
}

size_t Map::getAgentsCount() const
{
    return agents.size();
}

const Agent &Map::getAgent(int ind) const{
    return agents[ind];
}

