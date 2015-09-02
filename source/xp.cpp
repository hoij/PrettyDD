#include "line_info.h"
#include "logger.h"
#include "xp.h"

void XP::add(LineInfo& li) {
    if (li.subtype == "gained") {
        XP::XpInfo incomming;
        incomming.total = li.amount;
        incomming.count = 1;
        incomming.min = li.amount;
        incomming.max = li.amount;

        gained[li.type] += incomming;
    }
    else if (li.subtype == "lost") {
        XP::XpInfo incomming;
        incomming.total = li.amount;
        incomming.count = 1;
        incomming.min = li.amount;
        incomming.max = li.amount;

        lost[li.type] += incomming;
    }
    else {
        errorLog.write("Error: Unknown XP subtype: " + li.subtype);
        errorLog.write("Error: \t type was: " + li.type);
    }
}

int XP::getTotalGained(std::string type) const {
    auto it = gained.find(type);
    if (it != gained.end()) {
        return it->second.total;
    }
    else {
        return 0;
    }
}

int XP::getCountGained(std::string type) const {
    auto it = gained.find(type);
    if (it != gained.end()) {
        return it->second.count;
    }
    else {
        return 0;
    }
}

int XP::getMaxGained(std::string type) const {
    auto it = gained.find(type);
    if (it != gained.end()) {
        return it->second.max;
    }
    else {
        return -1;
    }
}

int XP::getMinGained(std::string type) const {
    auto it = gained.find(type);
    if (it != gained.end()) {
        return it->second.min;
    }
    else {
        return std::numeric_limits<int>::max();
    }
}

int XP::getTotalLost(std::string type) const {
    auto it = lost.find(type);
    if (it != lost.end()) {
        return it->second.total;
    }
    else {
        return 0;
    }
}

int XP::getCountLost(std::string type) const {
    auto it = lost.find(type);
    if (it != lost.end()) {
        return it->second.count;
    }
    else {
        return 0;
    }
}

int XP::getMaxLost(std::string type) const {
    auto it = lost.find(type);
    if (it != lost.end()) {
        return it->second.max;
    }
    else {
        return -1;
    }
}

int XP::getMinLost(std::string type) const {
    auto it = lost.find(type);
    if (it != lost.end()) {
        return it->second.min;
    }
    else {
        return std::numeric_limits<int>::max();
    }
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
