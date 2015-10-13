#ifndef NANO_PROGRAMS_H
#define NANO_PROGRAMS_H


#include "line_info.h"

#include <map>
#include <string>
#include <vector>


class NanoPrograms {
public:
    void add(LineInfo& li);

    std::vector<std::string> getNanoProgramNames() const;

    int getExecutes(std::string& name) const;
    int getLands(std::string& name) const;
    int getResists(std::string& name) const;
    int getAborts(std::string& name) const;
    int getCounters(std::string& name) const;
    int getFumbles(std::string& name) const;
    int getFullNCUs(std::string& name) const;

private:
    struct NanoProgram {
        int executes = 0;
        int lands = 0;
        int resists = 0;
        int aborts = 0;
        int counters = 0;
        int fumbles = 0;
        int fullncus = 0;
    };

    void addNanoProgram(std::string& subtype);

    std::string nameOfLastNanoProgramCasted;
    std::map<std::string, NanoProgram> nanoPrograms;
};


#endif  // NANO_PROGRAMS_H
