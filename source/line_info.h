#ifndef LINE_INFO_H
#define LINE_INFO_H


#include <string>


class LineInfo {
public:
    virtual ~LineInfo() {}

    std::string dealer_name;
    std::string receiver_name;
    std::string type;
    std::string subtype;
    std::string command;
    int amount = 0;
    bool crit = false;
    bool deflect = false;
    bool miss = false;
    bool nanobots = false;
    bool hasStats = true;
    std::string nanoProgramName;
};


#endif  // LINE_INFO_H
