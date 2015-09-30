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

    int getExecutes(std::string& name);
    int getLands(std::string& name);
    int getResists(std::string& name);
    int getAborts(std::string& name);
    int getCounters(std::string& name);
    int getFumbles(std::string& name);

private:
    struct NanoProgram {
        int executes = 0;
        int lands = 0;
        int resists = 0;
        int aborts = 0;
        int counters = 0;
        int fumbles = 0;
    };

    void addNanoProgram(std::string& subtype);

    std::string nameOfLastNanoProgramCasted;
    std::map<std::string, NanoProgram> nanoPrograms;
};


#endif  // NANO_PROGRAMS_H
