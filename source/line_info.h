#ifndef LINE_INFO_H
#define LINE_INFO_H


#include <ctime>
#include <string>


class LineInfo {
public:
    virtual ~LineInfo() {}

    bool hasStats = true;
    int amount = 0;
    std::time_t time;
    std::string dealer_name;
    std::string receiver_name;
    std::string type;
    std::string damageCategory;
    std::string subtype;
    std::string command;
    std::string nanoProgramName;
};


#endif  // LINE_INFO_H
