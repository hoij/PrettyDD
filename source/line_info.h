#ifndef LINE_INFO_H
#define LINE_INFO_H


#include <string>
#include "nano_program.h"


class NanoProgram;

class LineInfo {
public:
    LineInfo();
    ~LineInfo();

    std::string dealer_name;
    std::string receiver_name;
    std::string type;
    std::string subtype;
    int amount = 0;
    bool crit = false;
    bool deflect = false;
    bool miss = false;
    NanoProgram* nanoProgram;
};


#endif  // LINE_INFO_H
