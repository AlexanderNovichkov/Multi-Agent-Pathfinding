#ifndef XMLLOGGER_H
#define	XMLLOGGER_H
#include "tinyxml2.h"
#include "ilogger.h"


//That's the class that flushes the data to the output XML


class XmlLogger : public ILogger {

public:
    XmlLogger(std::string loglevel):ILogger(loglevel){}

    virtual ~XmlLogger() {};

    bool getLog(const char *FileName, const std::string *LogParams);

    void saveLog();

    // void writeToLogMap(const Map &Map, const std::list<Node> &path);

    //void writeToLogOpenClose(const typename &open, const typename &close);

    void writeToLogAgentsSresult(const SearchResult &sr);

private:
    void writeSummary(const SearchResult &sr,tinyxml2::XMLElement * section);
    void writeAgentSummary(const AgentSearchResult &asr, tinyxml2::XMLElement * agent_section);
    void writeTrajectory(const AgentSearchResult &asr, tinyxml2::XMLElement *agent_section);
private:
    std::string LogFileName;
    tinyxml2::XMLDocument doc;
};

#endif

