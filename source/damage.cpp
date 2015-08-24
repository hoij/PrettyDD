#include "damage.h"
#include "line_info.h"

#include <iomanip>
#include <limits>
#include <ostream>
#include <string>


Damage& Damage::operator+=(const Damage& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    return *this;
}

Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}

void Damage::add(LineInfo& li, std::string playerType) {
    if (playerType == "dealer") {
        addDamageDealtOnPlayer(li);
    }
    else if (playerType == "receiver") {
        addDamageReceivedFromPlayer(li);
    }
}

void Damage::addDamageDealtOnPlayer(LineInfo& li) {
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

void Damage::addDamageReceivedFromPlayer(LineInfo& li) {
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

std::ostream& Damage::writeDamageDealtToPlayer(std::ostream& os) {
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

std::ostream& Damage::writeDamageReceivedFromPlayer(std::ostream& os) {
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

std::ostream& Damage::writeHeadings(std::ostream& os) {
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

Damage::damageInfo& Damage::damageInfo::operator+=(const damageInfo& rhs) {
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
