#ifndef XP_H
#define XP_H


#include <ctime>
#include <limits>
#include <map>
#include <string>
#include <vector>

class LineInfo;

class XP {
public:
    void add(LineInfo& li);

    // XP Per Minute is not calculated continuously but needs to be
    // called before retreiving the XP instance.
    void calcXPM(std::time_t timeActive);

    std::vector<std::string> getTypes() const;

    int getTotal(std::string type) const;
    int getXPM(std::string type) const;

    int getTotalGained(std::string type) const;
    int getXPMGained(std::string type) const;
    int getCountGained(std::string type) const;
    int getMaxGained(std::string type) const;
    int getMinGained(std::string type) const;

    int getTotalLost(std::string type) const;
    int getCountLost(std::string type) const;
    int getMaxLost(std::string type) const;
    int getMinLost(std::string type) const;

private:
    struct XpInfo {
        int total = 0;
        int xpm = 0;

        int totalGained = 0;
        int xpmGained = 0;
        int countGained = 0;
        int maxGained = -1;
        int minGained = std::numeric_limits<int>::max();

        int totalLost = 0;
        int countLost = 0;
        int maxLost = -1;
        int minLost = std::numeric_limits<int>::max();
    };

    std::map<std::string, XpInfo> stats;
};


#endif  // XP_H
