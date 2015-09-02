#ifndef XP_H
#define XP_H


#include <limits>
#include <map>
#include <string>

class LineInfo;

class XP {
public:
    void add(LineInfo& li);

    int getTotalGained(std::string type) const;
    int getCountGained(std::string type) const;
    int getMaxGained(std::string type) const;
    int getMinGained(std::string type) const;

    int getTotalLost(std::string type) const;
    int getCountLost(std::string type) const;
    int getMaxLost(std::string type) const;
    int getMinLost(std::string type) const;

private:
    struct XpInfo {
        XpInfo& operator+=(const XpInfo& rhs);
        int total = 0;
        int count = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

    std::map<std::string, XpInfo> gained;
    std::map<std::string, XpInfo> lost;
};


#endif  // XP_H
