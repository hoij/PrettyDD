#ifndef NANO_PROGRAM_H
#define NANO_PROGRAM_H


#include <string>
#include <map>
#include "line_info.h"

class LineInfo;

class NanoProgram {
public:
    //NanoProgram();
    //~NanoProgram();

    void createProgram(const LineInfo& li);

    NanoProgram& operator=(const NanoProgram& np);

    NanoProgram& operator+=(NanoProgram& np);

    const std::string& getName() const;
    void setName(std::string name);
    const std::map<std::string, int>& getStats() const;
    void addStat(std::string stat, int value);

private:
    std::string name;
    std::map<std::string, int> stats;
    // In stats: execute == number of times executed;
};


#endif  // NANO_PROGRAM_H
