#ifndef DAMAGE_H
#define DAMAGE_H


#include <string>
#include <limits>
#include "line_info.h"


class Damage {
public:
void add(LineInfo& li, std::string playerType) {
    if (playerType == "dealer") {
        addDamageDealt(li);
    }
    else if (playerType == "receiver") {
        addDamageReceived(li);
    }
}

void addDamageDealt(LineInfo& li) {
    dealt.total += li.amount;
    dealt.count++;
    if (!li.crit) {
        if (li.amount > dealt.regular_max) {
            dealt.regular_max = li.amount;
        }
        if (li.amount < dealt.regular_min) {
            dealt.regular_min = li.amount;
        }
        if (li.deflect) {
            dealt.deflects++;
        }
        if (li.miss) {
            dealt.misses++;
        }
    }
    else {
        dealt.crit_count++;
        dealt.crit_total += li.amount;
        if (li.amount > dealt.crit_max) {
            dealt.crit_max = li.amount;
        }
        if (li.amount < dealt.crit_min) {
            dealt.crit_min = li.amount;
        }
    }
}

void addDamageReceived(LineInfo& li) {
    received.total += li.amount;
    received.count++;
    if (!li.crit) {
        if (li.amount > received.regular_max) {
            received.regular_max = li.amount;
        }
        if (li.amount < received.regular_min) {
            received.regular_min = li.amount;
        }
        if (li.deflect) {
            received.deflects++;
        }
        if (li.miss) {
            received.misses++;
        }
    }
    else {
        received.crit_count++;
        received.crit_total += li.amount;
        if (li.amount > received.crit_max) {
            received.crit_max = li.amount;
        }
        if (li.amount < received.crit_min) {
            received.crit_min = li.amount;
        }
    }
}

private:
	struct damageInfo {
        damageInfo() {}

        int total = 0;
        int count = 0;
        int regular_max = -1;
        int regular_min = std::numeric_limits<int>::max();
        int crit_total = 0;
		int crit_count = 0;
		int crit_max = -1;
		int crit_min = std::numeric_limits<int>::max();
        int deflects = 0;
        int misses = 0;

        // Not used at the moment:
        int mean_time = 0;
    };

    damageInfo dealt;
    damageInfo received;
};



#endif // DAMAGE_H
