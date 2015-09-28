#include "line_info.h"
#include "logger.h"
#include "xp.h"


void XP::add(LineInfo& li) {
    XP::XpInfo incomming;
    incomming.total = li.amount;
    incomming.count = 1;
    incomming.min = li.amount;
    incomming.max = li.amount;
    if (li.subtype == "gained") {
        gained[li.type] += incomming;
    }
    else if (li.subtype == "lost") {
        lost[li.type] += incomming;
    }
    else {
        errorLog.write("Error: Unknown XP subtype: " + li.subtype);
        errorLog.write("Error: \t type was: " + li.type);
    }
}

void XP::setXPM(std::string type, int xpPerMinute) {
    gained[type].xpm = xpPerMinute;
}

int XP::getTotalGained(std::string type) const {
    auto it = gained.find(type);
    return (it != gained.end()) ? it->second.total : 0;
}

int XP::getXPM(std::string type) const {
    auto it = gained.find(type);
    return (it == gained.end()) ? 0 : it->second.xpm;
}

int XP::getCountGained(std::string type) const {
    auto it = gained.find(type);
    return (it == gained.end()) ? 0 : it->second.count;
}

int XP::getMaxGained(std::string type) const {
    auto it = gained.find(type);
    return (it == gained.end()) ? -1 : it->second.max;
}

int XP::getMinGained(std::string type) const {
    auto it = gained.find(type);
    return (it == gained.end()) ?
        std::numeric_limits<int>::max() : it->second.min;
}

int XP::getTotalLost(std::string type) const {
    auto it = lost.find(type);
    return (it == lost.end()) ? 0 : it->second.total;
}

int XP::getCountLost(std::string type) const {
    auto it = lost.find(type);
    return (it == lost.end()) ? 0 : it->second.count;
}

int XP::getMaxLost(std::string type) const {
    auto it = lost.find(type);
    return (it == lost.end()) ? -1 : it->second.max;
}

int XP::getMinLost(std::string type) const {
    auto it = lost.find(type);
    return (it == lost.end()) ?
        std::numeric_limits<int>::max() : it->second.min;
}

XP::XpInfo& XP::XpInfo::operator+=(const XpInfo& rhs) {
    total += rhs.total;
    count += rhs.count;
    if (rhs.min < min) {
        min = rhs.min;
    }
    if (rhs.max > max) {
        max = rhs.max;
    }
    return *this;
}
