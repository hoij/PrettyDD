#include "line_info.h"
#include "logger.h"
#include "xp.h"


void XP::add(LineInfo& li) {
    if (li.subtype == "gained") {
        XP::XpInfo& xp = stats[li.type];
        xp.total += li.amount;
        xp.totalGained += li.amount;
        xp.countGained += 1;
        if (li.amount < xp.minGained &&
            li.amount != 1) {  // 1 xp is often gotten from grey mobs.
            xp.minGained = li.amount;
        }
        if (li.amount > xp.maxGained) {
            xp.maxGained = li.amount;
        }
    }
    else if (li.subtype == "lost") {
        XP::XpInfo& xp = stats[li.type];
        xp.total -= li.amount;
        xp.totalLost += li.amount;
        xp.countLost += 1;
        if (li.amount< xp.minLost) {
            xp.minLost = li.amount;
        }
        if (li.amount> xp.maxLost) {
            xp.maxLost = li.amount;
        }
    }
    else {
        errorLog.write("Error: Unknown XP subtype: " + li.subtype);
    }
}

void XP::calcXPH(time_t timeActive) {
    /* Calculates and sets XP Per Minute */
    for (auto& xp : stats) {
        if (timeActive == 0) {
            xp.second.xph = 0;
            xp.second.xphGained = 0;
        }
        else {
            xp.second.xph =
                int(xp.second.total / ((double)timeActive / 3600));
            xp.second.xphGained =
                int(xp.second.totalGained / ((double)timeActive / 3600));
        }
    }
}

bool XP::empty() const {
    return stats.empty();
}

std::vector<LineType> XP::getTypes() const {
    std::vector<LineType> types;
    for (const auto& xp : stats) {
        types.push_back(xp.first);
    }
    return types;
}

int XP::getTotal(LineType type) const {
    auto it = stats.find(type);
    return (it != stats.end()) ? it->second.total : 0;
}

int XP::getXPH(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.xph;
}

int XP::getTotalGained(LineType type) const {
    auto it = stats.find(type);
    return (it != stats.end()) ? it->second.totalGained : 0;
}

int XP::getXPHGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.xphGained;
}

int XP::getCountGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.countGained;
}

int XP::getMaxGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? -1 : it->second.maxGained;
}

int XP::getMinGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ?
        std::numeric_limits<int>::max() : it->second.minGained;
}

int XP::getTotalLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.totalLost;
}

int XP::getCountLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.countLost;
}

int XP::getMaxLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? -1 : it->second.maxLost;
}

int XP::getMinLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ?
        std::numeric_limits<int>::max() : it->second.minLost;
}
