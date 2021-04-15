#include "xmllogger.h"
#include <iostream>

using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

bool XmlLogger::getLog(const char *FileName, const std::string *LogParams)
{
    if (loglevel == CN_LP_LEVEL_NOPE_WORD) return true;

    if (doc.LoadFile(FileName) != tinyxml2::XMLError::XML_SUCCESS) {
        std::cout << "Error opening input XML file" << std::endl;
        return false;
    }

    if (LogParams[CN_LP_PATH] == "" && LogParams[CN_LP_NAME] == "") {
        std::string str;
        str.append(FileName);
        size_t found = str.find_last_of(".");
        if (found != std::string::npos)
            str.insert(found, "_log");
        else
            str.append("_log");
        LogFileName.append(str);
    } else if (LogParams[CN_LP_PATH] == "") {
        LogFileName.append(FileName);
        std::string::iterator it = LogFileName.end();
        while (*it != '\\')
            it--;
        ++it;
        LogFileName.erase(it, LogFileName.end());
        LogFileName.append(LogParams[CN_LP_NAME]);
    } else if (LogParams[CN_LP_NAME] == "") {
        LogFileName.append(LogParams[CN_LP_PATH]);
        if (*(--LogParams[CN_LP_PATH].end()) != '\\') LogFileName.append("\\");
        std::string lfn;
        lfn.append(FileName);
        size_t found = lfn.find_last_of("\\");
        std::string str = lfn.substr(found);
        found = str.find_last_of(".");
        if (found != std::string::npos)
            str.insert(found, "_log");
        else
            str.append("_log");
        LogFileName.append(str);
    } else {
        LogFileName.append(LogParams[CN_LP_PATH]);
        if (*(--LogParams[CN_LP_PATH].end()) != '\\') LogFileName.append("\\");
        LogFileName.append(LogParams[CN_LP_NAME]);
    }

    XMLElement *log, *root = doc.FirstChildElement(CNS_TAG_ROOT);

    if (!root) {
        std::cout << "No '" << CNS_TAG_ROOT << "' element found in XML file" << std::endl;
        std::cout << "Can not create log" << std::endl;
        return false;
    }

    root->InsertEndChild(doc.NewElement(CNS_TAG_LOG));

    root = (root->LastChild())->ToElement();

    if (loglevel != CN_LP_LEVEL_NOPE_WORD) {
        log = doc.NewElement(CNS_TAG_MAPFN);
        log->LinkEndChild(doc.NewText(FileName));
        root->InsertEndChild(log);
    }

    if (loglevel == CN_LP_LEVEL_FULL_WORD || loglevel == CN_LP_LEVEL_MEDIUM_WORD)
        root->InsertEndChild(doc.NewElement(CNS_TAG_LOWLEVEL));

    return true;
}

void XmlLogger::saveLog()
{
    if (loglevel == CN_LP_LEVEL_NOPE_WORD)
        return;
    doc.SaveFile(LogFileName.c_str());
}

/*void XmlLogger::writeToLogMap(const Map &map, const std::list<Node> &path)
{
    if (loglevel == CN_LP_LEVEL_NOPE_WORD || loglevel == CN_LP_LEVEL_TINY_WORD)
        return;

    XMLElement *mapTag = doc.FirstChildElement(CNS_TAG_ROOT);
    mapTag = mapTag->FirstChildElement(CNS_TAG_LOG)->FirstChildElement(CNS_TAG_PATH);

    int iterate = 0;
    bool inPath;
    std::string str;
    for (int i = 0; i < map.getMapHeight(); ++i) {
        XMLElement *element = doc.NewElement(CNS_TAG_ROW);
        element->SetAttribute(CNS_TAG_ATTR_NUM, iterate);

        for (int j = 0; j < map.getMapWidth(); ++j) {
            inPath = false;
            for(std::list<Node>::const_iterator it = path.begin(); it != path.end(); it++)
                if(it->pos.i == i && it->pos.j == j) {
                    inPath = true;
                    break;
                }
            if (!inPath)
                str += std::to_string(map.getValue(Point{i,j}));
            else
                str += CNS_OTHER_PATHSELECTION;
            str += CNS_OTHER_MATRIXSEPARATOR;
        }

        element->InsertEndChild(doc.NewText(str.c_str()));
        mapTag->InsertEndChild(element);
        str.clear();
        iterate++;
    }
}*/

/*void XmlLogger::writeToLogOpenClose(const typename &open, const typename &close)
{
    //need to implement
    if (loglevel != CN_LP_LEVEL_FULL_WORD  && !(loglevel == CN_LP_LEVEL_MEDIUM_WORD && last))
        return;


}*/


/*void XmlLogger::writeToLogHPpath(const std::list<Node> &hppath)
{
    if (loglevel == CN_LP_LEVEL_NOPE_WORD || loglevel == CN_LP_LEVEL_TINY_WORD || hppath.empty())
        return;
    int partnumber = 0;
    XMLElement *hplevel = doc.FirstChildElement(CNS_TAG_ROOT);
    hplevel = hplevel->FirstChildElement(CNS_TAG_LOG)->FirstChildElement(CNS_TAG_HPLEVEL);
    std::list<Node>::const_iterator iter = hppath.begin();
    std::list<Node>::const_iterator it = hppath.begin();

    while (iter != --hppath.end()) {
        XMLElement *part = doc.NewElement(CNS_TAG_SECTION);
        part->SetAttribute(CNS_TAG_ATTR_NUM, partnumber);
        part->SetAttribute(CNS_TAG_ATTR_STX, it->j);
        part->SetAttribute(CNS_TAG_ATTR_STY, it->i);
        ++iter;
        part->SetAttribute(CNS_TAG_ATTR_FINX, iter->j);
        part->SetAttribute(CNS_TAG_ATTR_FINY, iter->i);
        part->SetAttribute(CNS_TAG_ATTR_LENGTH, iter->g - it->g);
        hplevel->LinkEndChild(part);
        ++it;
        ++partnumber;
    }
}*/

void XmlLogger::writeToLogAgentsSresult(const SearchResult &sr)
{
    if (loglevel == CN_LP_LEVEL_NOPE_WORD)
        return;

    XMLElement *log = doc.FirstChildElement(CNS_TAG_ROOT)->FirstChildElement(CNS_TAG_LOG);

    writeSummary(sr, log);

    if(loglevel == CN_LP_LEVEL_TINY_WORD){
        return;
    }

    for (size_t i = 0; i < sr.agents_sresult.size(); i++) {
        const AgentSearchResult &asr = sr.agents_sresult[i];

        XMLElement *agent_section = doc.NewElement(CNS_TAG_AGENT);
        agent_section->SetAttribute(CNS_TAG_ATTR_NUM, int(i + 1));

        writeAgentSummary(asr, agent_section);
        if(loglevel == CN_LP_LEVEL_MEDIUM_WORD || loglevel == CN_LP_LEVEL_FULL_WORD) {
            writeTrajectory(asr, agent_section);
        }

        log->InsertEndChild(agent_section);
    }
}

void XmlLogger::writeSummary(const SearchResult &sr, tinyxml2::XMLElement *section)
{
    XMLElement *summary = doc.NewElement(CNS_TAG_SUM);

    summary->SetAttribute(CNS_TAG_ATTR_SOLUTION_FOUND, sr.solution_found);
    summary->SetAttribute(CNS_TAG_ATTR_COST, sr.cost);
    summary->SetAttribute(CNS_TAG_ATTR_SEARCH_RUNTIME, sr.total_time);

    section->InsertEndChild(summary);
}


void XmlLogger::writeAgentSummary(const AgentSearchResult &asr, XMLElement *agent_section)
{
    XMLElement *summary = doc.NewElement(CNS_TAG_SUM);

    summary->SetAttribute(CNS_TAG_ATTR_TRAJECTORY_FOUND, asr.trajectory_found);
    summary->SetAttribute(CNS_TAG_ATTR_NUMOFSTEPS, asr.numberofsteps);
    summary->SetAttribute(CNS_TAG_ATTR_NODESCREATED, asr.nodescreated);
    summary->SetAttribute(CNS_TAG_ATTR_TRAJECTORY_TIME, std::to_string(asr.trajectory_time).c_str());
    summary->SetAttribute(CNS_TAG_ATTR_SEARCH_RUNTIME, std::to_string(asr.search_time).c_str());

    agent_section->InsertEndChild(summary);
}

void XmlLogger::writeTrajectory(const AgentSearchResult &asr, XMLElement *agent_section)
{
    if (loglevel == CN_LP_LEVEL_TINY_WORD)
        return;

    XMLElement *trajectory = doc.NewElement(CNS_TAG_TRAJECTORY);

    for (const AgentAction &action : asr.trajectory) {
        XMLElement *element = doc.NewElement(CNS_TAG_ACTION);
        element->SetAttribute(CNS_TAG_ATTR_START_TIME, action.start_time);
        element->SetAttribute(CNS_TAG_ATTR_FINX, action.goal.j);
        element->SetAttribute(CNS_TAG_ATTR_FINY, action.goal.i);
        trajectory->InsertEndChild(element);
    }

    agent_section->InsertEndChild(trajectory);
}

