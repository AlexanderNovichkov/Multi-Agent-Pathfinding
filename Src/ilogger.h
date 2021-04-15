#ifndef ILOGGER_H
#define	ILOGGER_H
#include "map.h"
#include "basicstructures.h"
#include "searchresult.h"
#include <unordered_map>
#include <list>

class ILogger
{
    public:
        ILogger(std::string loglevel) {this->loglevel = loglevel;}
        virtual bool getLog(const char* FileName, const std::string* LogParams) = 0;
        virtual void saveLog() = 0;
        // virtual void writeToLogMap(const Map& map, const std::list<Node>& path) = 0;
        // virtual void writeToLogOpenClose(const typename &open, const typename &close) = 0;
        virtual void writeToLogAgentsSresult(const SearchResult &sr) = 0;
        virtual ~ILogger() {};
    protected:
        std::string loglevel;
};


#endif

