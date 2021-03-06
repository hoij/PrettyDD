#ifndef LINE_INFO_H
#define LINE_INFO_H


#include "definitions.h"

#include <ctime>
#include <string>


class LineInfo {
public:
    virtual ~LineInfo() {}

    std::string dealer_name;
    std::string receiver_name;
    LineType type;
    std::string subtype;
    std::string command;
    std::string nanoProgramName;
    int amount = 0;
    std::time_t time;
    bool crit = false;
    bool deflect = false;
    bool miss = false;
    bool nanobots = false;
    bool shield = false;
    bool special = false;
    bool hasStats = true;
};


#endif  // LINE_INFO_H
