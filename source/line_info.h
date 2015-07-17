#ifndef LINE_INFO_H
#define LINE_INFO_H


#include <string>


class NanoProgram;

class LineInfo {
public:
    LineInfo();
    ~LineInfo();

    LineInfo(const LineInfo& li);
    LineInfo& operator=(LineInfo li);
    friend std::ostream& operator<<(std::ostream& os, const LineInfo& li);

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
    bool hasCommand = false;
    bool hasStats = true;
    NanoProgram* nanoProgram;

private:
    inline std::string boolToString(bool b) const {
        return b ? "true" : "false";
    }
};


#endif  // LINE_INFO_H
