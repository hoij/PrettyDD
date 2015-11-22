#ifndef XP_H
#define XP_H


#include "definitions.h"

#include <ctime>
#include <limits>
#include <map>
#include <string>
#include <vector>

class LineInfo;

class Experience {
public:
    void add(LineInfo& li);

    // XP Per Hour is not calculated continuously but needs to be
    // called before retreiving the XP per Hour stats.
    void calcXPH(std::time_t timeActive);

    bool empty() const;

    std::vector<LineType> getTypes() const;

    int getTotal(LineType type) const;
    int getXPH(LineType type) const;

    int getTotalGained(LineType type) const;
    int getXPHGained(LineType type) const;
    int getCountGained(LineType type) const;
    int getMaxGained(LineType type) const;
    int getMinGained(LineType type) const;

    int getTotalLost(LineType type) const;
    int getCountLost(LineType type) const;
    int getMaxLost(LineType type) const;
    int getMinLost(LineType type) const;

private:
    struct XpInfo {
        int total = 0;
        int xph = 0;

        int totalGained = 0;
        int xphGained = 0;
        int countGained = 0;
        int maxGained = -1;
        int minGained = std::numeric_limits<int>::max();

        int totalLost = 0;
        int countLost = 0;
        int maxLost = -1;
        int minLost = std::numeric_limits<int>::max();
    };

    std::map<LineType, XpInfo> stats;
};


#endif  // XP_H
