#ifndef XP_H
#define XP_H


#include <string>
#include <limits>
#include <map>
#include "line_info.h"


class XP {
public:
    struct info {
        int total = 0;
        int count = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

    info& operator[](std::string subtype) {
        return stats[subtype];
    }

    void add(LineInfo& li) {
        stats[li.subtype].total += li.amount;
        stats[li.subtype].count++;
        if (li.amount < stats[li.subtype].min) {
            stats[li.subtype].min = li.amount;
        }
        if (li.amount > stats[li.subtype].max) {
            stats[li.subtype].max = li.amount;
        }
    }

private:
    std::map<std::string, info> stats;
};


#endif  // XP_H
