#ifndef DAMAGE_H
#define DAMAGE_H


#include <string>
#include <limits>
#include <ostream>
#include <iomanip>
#include "line_info.h"


class Damage {
public:

//Damage& operator=(const Damage& d) {
//    if (this != &d) {
//        d.
//    }
//    return *this;
//}

Damage& operator+=(const Damage& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    return *this;
}

friend Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}

void add(LineInfo& li, std::string playerType) {
    if (playerType == "dealer") {
        addDamageDealtOnPlayer(li);
    }
    else if (playerType == "receiver") {
        addDamageReceivedFromPlayer(li);
    }
}

void addDamageDealtOnPlayer(LineInfo& li) {
    dealtOnPlayer.total += li.amount;
    dealtOnPlayer.count++;
    if (!li.crit) {
        if (li.amount > dealtOnPlayer.regularMax) {
            dealtOnPlayer.regularMax = li.amount;
        }
        if (li.amount < dealtOnPlayer.regularMin) {
            dealtOnPlayer.regularMin = li.amount;
        }
        if (li.deflect) {
            dealtOnPlayer.deflects++;
        }
        if (li.miss) {
            dealtOnPlayer.misses++;
        }
    }
    else {
        dealtOnPlayer.critCount++;
        dealtOnPlayer.critTotal += li.amount;
        if (li.amount > dealtOnPlayer.critMax) {
            dealtOnPlayer.critMax = li.amount;
        }
        if (li.amount < dealtOnPlayer.critMin) {
            dealtOnPlayer.critMin = li.amount;
        }
    }
}

void addDamageReceivedFromPlayer(LineInfo& li) {
    receivedFromPlayer.total += li.amount;
    receivedFromPlayer.count++;
    if (!li.crit) {
        if (li.amount > receivedFromPlayer.regularMax) {
            receivedFromPlayer.regularMax = li.amount;
        }
        if (li.amount < receivedFromPlayer.regularMin) {
            receivedFromPlayer.regularMin = li.amount;
        }
        if (li.deflect) {
            receivedFromPlayer.deflects++;
        }
        if (li.miss) {
            receivedFromPlayer.misses++;
        }
    }
    else {
        receivedFromPlayer.critCount++;
        receivedFromPlayer.critTotal += li.amount;
        if (li.amount > receivedFromPlayer.critMax) {
            receivedFromPlayer.critMax = li.amount;
        }
        if (li.amount < receivedFromPlayer.critMin) {
            receivedFromPlayer.critMin = li.amount;
        }
    }
}

std::ostream& writeDamageDealtToPlayer(std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << getTotalDealt() <<
          std::setw(width) << getCountDealt() <<
          std::setw(width) << ((getRegularMaxDealt() == -1) ?
                            0 : getRegularMaxDealt()) <<
          std::setw(width) << ((getRegularMinDealt() == std::numeric_limits<int>::max()) ?
                            0 : getRegularMinDealt()) <<
          std::setw(width) << getCritTotalDealt() <<
          std::setw(width) << getCritCountDealt() <<
          std::setw(width) << ((getCritMaxDealt() == -1) ?
                            0 : getCritMaxDealt()) <<
          std::setw(width) << ((getCritMinDealt() == std::numeric_limits<int>::max()) ?
                            0 : getCritMinDealt()) <<
          std::setw(width) << getDeflectsDealt() <<
          std::setw(width) << getMissesDealt() << std::endl;
    return os;
}

std::ostream& writeDamageReceivedFromPlayer(std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << getTotalReceived() <<
          std::setw(width) << getCountReceived() <<
          std::setw(width) << ((getRegularMaxReceived() == -1) ?
                            0 : getRegularMaxReceived()) <<
          std::setw(width) << ((getRegularMinReceived() == std::numeric_limits<int>::max()) ?
                            0 : getRegularMinReceived()) <<
          std::setw(width) << getCritTotalReceived() <<
          std::setw(width) << getCritCountReceived() <<
          std::setw(width) << ((getCritMaxReceived() == -1) ?
                            0 : getCritMaxReceived()) <<
          std::setw(width) << ((getCritMinReceived() == std::numeric_limits<int>::max()) ?
                            0 : getCritMinReceived()) <<
          std::setw(width) << getDeflectsReceived() <<
          std::setw(width) << getMissesReceived() << std::endl;
    return os;
}

static std::ostream& writeHeadings(std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "Count" <<
          std::setw(width) << "RegMax" <<
          std::setw(width) << "RegMin" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Crits" <<
          std::setw(width) << "CritMax" <<
          std::setw(width) << "CritMin" <<
          std::setw(width) << "Deflects" <<
          std::setw(width) << "Misses";
    return os;
}

int getTotalDealt() const {return dealtOnPlayer.total;}
int getCountDealt() const {return dealtOnPlayer.count;}
int getRegularMaxDealt() const {return dealtOnPlayer.regularMax;}
int getRegularMinDealt() const {return dealtOnPlayer.regularMin;}
int getCritTotalDealt() const {return dealtOnPlayer.critTotal;}
int getCritCountDealt() const {return dealtOnPlayer.critCount;}
int getCritMaxDealt() const {return dealtOnPlayer.critMax;}
int getCritMinDealt() const {return dealtOnPlayer.critMin;}
int getDeflectsDealt() const {return dealtOnPlayer.deflects;}
int getMissesDealt() const {return dealtOnPlayer.misses;}

int getTotalReceived() const {return receivedFromPlayer.total;}
int getCountReceived() const {return receivedFromPlayer.count;}
int getRegularMaxReceived() const {return receivedFromPlayer.regularMax;}
int getRegularMinReceived() const {return receivedFromPlayer.regularMin;}
int getCritTotalReceived() const {return receivedFromPlayer.critTotal;}
int getCritCountReceived() const {return receivedFromPlayer.critCount;}
int getCritMaxReceived() const {return receivedFromPlayer.critMax;}
int getCritMinReceived() const {return receivedFromPlayer.critMin;}
int getDeflectsReceived() const {return receivedFromPlayer.deflects;}
int getMissesReceived() const {return receivedFromPlayer.misses;}

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

    damageInfo dealtOnPlayer;
    damageInfo receivedFromPlayer;
};


#endif // DAMAGE_H
