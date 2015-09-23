#ifndef LINE_INFO_H
#define LINE_INFO_H


#include <ctime>
#include <string>


class LineInfo {
public:
    virtual ~LineInfo() {}

    std::string dealer_name;
    std::string receiver_name;
    std::string type;
    std::string subtype;
    std::string command;
    std::string nanoProgramName;
    int amount = 0;
    std::time_t time;
    bool crit = false;
    bool deflect = false;
    bool miss = false;
    bool nanobots = false;
    bool hasStats = true;
};


#endif  // LINE_INFO_H
