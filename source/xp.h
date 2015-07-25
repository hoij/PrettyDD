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

    void add(LineInfo& li) {
        if (li.subtype == "gained") {
            gained[li.type].total += li.amount;
            gained[li.type].count++;
            if (li.amount < gained[li.type].min) {
                gained[li.type].min = li.amount;
            }
            if (li.amount > gained[li.type].max) {
                gained[li.type].max = li.amount;
            }
        }
        else if (li.subtype == "lost") {
            lost[li.type].total += li.amount;
            lost[li.type].count++;
            if (li.amount < lost[li.type].min) {
                lost[li.type].min = li.amount;
            }
            if (li.amount > lost[li.type].max) {
                lost[li.type].max = li.amount;
            }
        }
        else {
            // TODO: Log error.
        }
    }

private:
    std::map<std::string, info> gained;
    std::map<std::string, info> lost;
};


#endif  // XP_H
