#ifndef DAMAGE_H
#define DAMAGE_H


#include <string>
#include <limits>
#include <ostream>
#include <iomanip>
#include "line_info.h"


class Damage {
public:

Damage& operator+=(const Damage& rhs) {
    dealtOnYou += rhs.dealtOnYou;
    receivedFromYou += rhs.receivedFromYou;
    return *this;
}

friend Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}

std::ostream& writeDamageDealtToYou(std::ostream& os) {
    os << std::right <<
          std::setw(8) << getTotalDealt() <<
          std::setw(8) << getCountDealt() <<
          std::setw(8) << ((getRegularMaxDealt() == -1) ?
                            0 : getRegularMaxDealt()) <<
          std::setw(8) << ((getRegularMinDealt() == std::numeric_limits<int>::max()) ?
                            0 : getRegularMinDealt()) <<
          std::setw(8) << getCritTotalDealt() <<
          std::setw(8) << getCritCountDealt() <<
          std::setw(8) << ((getCritMaxDealt() == -1) ?
                            0 : getCritMaxDealt()) <<
          std::setw(8) << ((getCritMinDealt() == std::numeric_limits<int>::max()) ?
                            0 : getCritMinDealt()) <<
          std::setw(8) << getDeflectsDealt() <<
          std::setw(8) << getMissesDealt() << std::endl;
    return os;
}

std::ostream& writeDamageReceivedFromYou(std::ostream& os) {
    os << std::right <<
          std::setw(8) << getTotalReceived() <<
          std::setw(8) << getCountReceived() <<
          std::setw(8) << ((getRegularMaxReceived() == -1) ?
                            0 : getRegularMaxReceived()) <<
          std::setw(8) << ((getRegularMinReceived() == std::numeric_limits<int>::max()) ?
                            0 : getRegularMinReceived()) <<
          std::setw(8) << getCritTotalReceived() <<
          std::setw(8) << getCritCountReceived() <<
          std::setw(8) << ((getCritMaxReceived() == -1) ?
                            0 : getCritMaxReceived()) <<
          std::setw(8) << ((getCritMinReceived() == std::numeric_limits<int>::max()) ?
                            0 : getCritMinReceived()) <<
          std::setw(8) << getDeflectsReceived() <<
          std::setw(8) << getMissesReceived() << std::endl;
    return os;
}

static std::ostream& writeHeadings(std::ostream& os) {
    os << std::right <<
          std::setw(8) << "Total" <<
          std::setw(8) << "Count" <<
          std::setw(8) << "RegMax" <<
          std::setw(8) << "RegMin" <<
          std::setw(8) << "Crit" <<
          std::setw(8) << "Crits" <<
          std::setw(8) << "CritMax" <<
          std::setw(8) << "CritMin" <<
          std::setw(8) << "Deflects" <<
          std::setw(8) << "Misses";
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
    dealtOnYou.total += li.amount;
    dealtOnYou.count++;
    if (!li.crit) {
        if (li.amount > dealtOnYou.regularMax) {
            dealtOnYou.regularMax = li.amount;
        }
        if (li.amount < dealtOnYou.regularMin) {
            dealtOnYou.regularMin = li.amount;
        }
        if (li.deflect) {
            dealtOnYou.deflects++;
        }
        if (li.miss) {
            dealtOnYou.misses++;
        }
    }
    else {
        dealtOnYou.critCount++;
        dealtOnYou.critTotal += li.amount;
        if (li.amount > dealtOnYou.critMax) {
            dealtOnYou.critMax = li.amount;
        }
        if (li.amount < dealtOnYou.critMin) {
            dealtOnYou.critMin = li.amount;
        }
    }
}

void addDamageReceived(LineInfo& li) {
    receivedFromYou.total += li.amount;
    receivedFromYou.count++;
    if (!li.crit) {
        if (li.amount > receivedFromYou.regularMax) {
            receivedFromYou.regularMax = li.amount;
        }
        if (li.amount < receivedFromYou.regularMin) {
            receivedFromYou.regularMin = li.amount;
        }
        if (li.deflect) {
            receivedFromYou.deflects++;
        }
        if (li.miss) {
            receivedFromYou.misses++;
        }
    }
    else {
        receivedFromYou.critCount++;
        receivedFromYou.critTotal += li.amount;
        if (li.amount > receivedFromYou.critMax) {
            receivedFromYou.critMax = li.amount;
        }
        if (li.amount < receivedFromYou.critMin) {
            receivedFromYou.critMin = li.amount;
        }
    }
}

int getTotalDealt() const {return dealtOnYou.total;}
int getCountDealt() const {return dealtOnYou.count;}
int getRegularMaxDealt() const {return dealtOnYou.regularMax;}
int getRegularMinDealt() const {return dealtOnYou.regularMin;}
int getCritTotalDealt() const {return dealtOnYou.critTotal;}
int getCritCountDealt() const {return dealtOnYou.critCount;}
int getCritMaxDealt() const {return dealtOnYou.critMax;}
int getCritMinDealt() const {return dealtOnYou.critMin;}
int getDeflectsDealt() const {return dealtOnYou.deflects;}
int getMissesDealt() const {return dealtOnYou.misses;}

int getTotalReceived() const {return receivedFromYou.total;}
int getCountReceived() const {return receivedFromYou.count;}
int getRegularMaxReceived() const {return receivedFromYou.regularMax;}
int getRegularMinReceived() const {return receivedFromYou.regularMin;}
int getCritTotalReceived() const {return receivedFromYou.critTotal;}
int getCritCountReceived() const {return receivedFromYou.critCount;}
int getCritMaxReceived() const {return receivedFromYou.critMax;}
int getCritMinReceived() const {return receivedFromYou.critMin;}
int getDeflectsReceived() const {return receivedFromYou.deflects;}
int getMissesReceived() const {return receivedFromYou.misses;}

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
            critCount += rhs.critCount;
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

    damageInfo dealtOnYou;
    damageInfo receivedFromYou;
};



#endif // DAMAGE_H
