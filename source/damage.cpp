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

void Damage::addDamageDealtOnPlayer(LineInfo& li) {
    addDamage(li, dealtOnPlayer);
}

void Damage::addDamageReceivedFromPlayer(LineInfo& li) {
    addDamage(li, receivedFromPlayer);
}

void Damage::addDamage(LineInfo& li, Damage::DamageInfo& di) {
    di.total += li.amount;
    di.count++;
    if (li.crit) {
        di.critCount++;
        di.critTotal += li.amount;
        if (li.amount > di.critMax) {
            di.critMax = li.amount;
        }
        if (li.amount < di.critMin) {
            di.critMin = li.amount;
        }
    }
    else {
        if (li.deflect) {
            di.deflects++;
            return;
        }
        if (li.miss) {
            di.misses++;
            return;
        }
        if (li.amount > di.regularMax) {
            di.regularMax = li.amount;
        }
        if (li.amount < di.regularMin) {
            di.regularMin = li.amount;
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

Damage::DamageInfo& Damage::DamageInfo::operator+=(const DamageInfo& rhs) {
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
