#include "line_info.h"
#include "logger.h"
#include "xp.h"


void Experience::add(LineInfo& li) {
    if (li.subtype == "gained") {
        Experience::XpInfo& xp = stats[li.type];
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
        Experience::XpInfo& xp = stats[li.type];
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
        errorLog.write("Error: \t type was: " + li.type);
    }
}

void Experience::calcXPH(time_t timeActive) {
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

bool Experience::empty() const {
    return stats.empty();
}

std::vector<LineType> Experience::getTypes() const {
    std::vector<LineType> types;
    for (const auto& xp : stats) {
        types.push_back(xp.first);
    }
    return types;
}

int Experience::getTotal(LineType type) const {
    auto it = stats.find(type);
    return (it != stats.end()) ? it->second.total : 0;
}

int Experience::getXPH(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.xph;
}

int Experience::getTotalGained(LineType type) const {
    auto it = stats.find(type);
    return (it != stats.end()) ? it->second.totalGained : 0;
}

int Experience::getXPHGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.xphGained;
}

int Experience::getCountGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.countGained;
}

int Experience::getMaxGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? -1 : it->second.maxGained;
}

int Experience::getMinGained(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ?
        std::numeric_limits<int>::max() : it->second.minGained;
}

int Experience::getTotalLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.totalLost;
}

int Experience::getCountLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? 0 : it->second.countLost;
}

int Experience::getMaxLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ? -1 : it->second.maxLost;
}

int Experience::getMinLost(LineType type) const {
    auto it = stats.find(type);
    return (it == stats.end()) ?
        std::numeric_limits<int>::max() : it->second.minLost;
}
