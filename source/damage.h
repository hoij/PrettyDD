#ifndef DAMAGE_H
#define DAMAGE_H


#include <string>
#include <limits>
#include <ostream>
#include "line_info.h"


class Damage {
public:

Damage& operator+=(const Damage& rhs) {
    dealt += rhs.dealt;
    received += rhs.received;
    return *this;
}

friend Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}

friend std::ostream& operator<<(std::ostream& os, Damage& d) {
    os << d.getTotalDealt() << " " <<
          d.getCountDealt() << " " <<
          d.getRegularMaxDealt() << " " <<
          d.getRegularMinDealt() << " " <<
          d.getCritTotalDealt() << " " <<
          d.getCritCountDealt() << " " <<
          d.getCritMaxDealt() << " " <<
          d.getCritMinDealt() << " " <<
          d.getDeflectsDealt() << " " <<
          d.getMissesDealt() << std::endl;
    return os;
}

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
        if (li.amount > dealt.regularMax) {
            dealt.regularMax = li.amount;
        }
        if (li.amount < dealt.regularMin) {
            dealt.regularMin = li.amount;
        }
        if (li.deflect) {
            dealt.deflects++;
        }
        if (li.miss) {
            dealt.misses++;
        }
    }
    else {
        dealt.critCount++;
        dealt.critTotal += li.amount;
        if (li.amount > dealt.critMax) {
            dealt.critMax = li.amount;
        }
        if (li.amount < dealt.critMin) {
            dealt.critMin = li.amount;
        }
    }
}

void addDamageReceived(LineInfo& li) {
    received.total += li.amount;
    received.count++;
    if (!li.crit) {
        if (li.amount > received.regularMax) {
            received.regularMax = li.amount;
        }
        if (li.amount < received.regularMin) {
            received.regularMin = li.amount;
        }
        if (li.deflect) {
            received.deflects++;
        }
        if (li.miss) {
            received.misses++;
        }
    }
    else {
        received.critCount++;
        received.critTotal += li.amount;
        if (li.amount > received.critMax) {
            received.critMax = li.amount;
        }
        if (li.amount < received.critMin) {
            received.critMin = li.amount;
        }
    }
}

int getTotalDealt() {return dealt.total;}
int getCountDealt() {return dealt.count;}
int getRegularMaxDealt() {return dealt.regularMax;}
int getRegularMinDealt() {return dealt.regularMin;}
int getCritTotalDealt() {return dealt.critTotal;}
int getCritCountDealt() {return dealt.critCount;}
int getCritMaxDealt() {return dealt.critMax;}
int getCritMinDealt() {return dealt.critMin;}
int getDeflectsDealt() {return dealt.deflects;}
int getMissesDealt() {return dealt.misses;}

int getTotalReceived() {return received.total;}
int getCountReceived() {return received.count;}
int getRegularMaxReceived() {return received.regularMax;}
int getRegularMinReceived() {return received.regularMin;}
int getCritTotalReceived() {return received.critTotal;}
int getCritCountReceived() {return received.critCount;}
int getCritMaxReceived() {return received.critMax;}
int getCritMinReceived() {return received.critMin;}
int getDeflectsReceived() {return received.deflects;}
int getMissesReceived() {return received.misses;}

private:
	struct damageInfo {
        // Why even have this struct?
        damageInfo& operator+=(const damageInfo& rhs) {
            total += rhs.total;
            count += rhs.count;
            if (rhs.regularMax > regularMax) {
                regularMax = rhs.regularMax;
            }
            if (rhs.regularMin < regularMin) {
                regularMin = rhs.regularMin;
            }
            critTotal += rhs.critTotal;
            if (rhs.critMax > critMax) {
                critMax = rhs.critMax;
            }
            if (rhs.critMin < critMin) {
                critMin = rhs.critMin;
            }
            deflects += rhs.deflects;
            misses += rhs.misses;
            return *this;
        }

        int total = 0;
        int count = 0;
        int regularMax = -1;
        int regularMin = std::numeric_limits<int>::max();
        int critTotal = 0;
		int critCount = 0;
		int critMax = -1;
		int critMin = std::numeric_limits<int>::max();
        int deflects = 0;
        int misses = 0;
        // Not used at the moment:
        int meanTime = 0;
    };

    damageInfo dealt;
    damageInfo received;
};



#endif // DAMAGE_H
