#include "damage.h"
#include "line_info.h"

#include <algorithm>
#include <ostream>


Damage& Damage::operator+=(const Damage& rhs) {
    for (auto const& category : receivedFromPlayer) {
        for (auto const& damageType : category.second) {
            if (damageType.first) {
                rhs. += damageType;
            }
        }
        sum.push_back(di);
    }
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

void Damage::setDealtOnPlayerDPM(int damagePerMinute) {
    dealtOnPlayer.dpm = damagePerMinute;
}

void Damage::setReceivedFromPlayerDPM(int damagePerMinute) {
    receivedFromPlayer.dpm = damagePerMinute;
}

void Damage::addDamage(LineInfo& li, Damage::DamageInfo& di) {
    di.total += li.amount;
    di.count++;
    // TODO: Set these bools on construction only and not
    // on each addition.
    shield = li.shield;
    special = li.special;

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
    else if (li.miss) {
        di.misses++;
    }
    else if (li.deflect) {
        di.regularDeflectTotal += li.amount;
        di.regularDeflectCount++;
        if (li.amount > di.regularDeflectMax) {
            di.regularDeflectMax = li.amount;
        }
        if (li.amount < di.regularDeflectMin) {
            di.regularDeflectMin = li.amount;
        }
    }
    else if (li.nanobots) {
        di.nanobotCount++;
        di.nanobotTotal += li.amount;
        if (li.amount > di.nanobotMax) {
            di.nanobotMax = li.amount;
        }
        if (li.amount < di.nanobotMin) {
            di.nanobotMin = li.amount;
        }
    }
    else {
        di.regularCount++;
        di.regularTotal += li.amount;
        if (li.amount > di.regularMax) {
            di.regularMax = li.amount;
        }
        if (li.amount < di.regularMin) {
            di.regularMin = li.amount;
        }
    }
}

std::vector<DamageInfo> Damage::getTotalDamage() const {
    // Or return just an int for the total.

}

std::vector<std::pair<std::string, DamageInfo>>
Damage::getTotalReceivedFromPlayerPerCategory() const {

}

std::vector<std::pair<std::string, DamageInfo>>
Damage::getTotalDealtOnPlayerPerCategory() const {

}

std::vector<std::pair<std::string, DamageInfo>>
getTotalPerCategory(std::vector<std::pair<std::string, std::pair<
                        std::string, DamageInfo>>> v) {
    std::vector<std::pair<std::string, DamageInfo>> sum;
    for (auto const& category : v) {
        DamageInfo di;
        for (auto const& damageType : category) {
            di += damageType;
        }
        sum.push_back(di);
    }
}

std::vector<std::pair<std::string, DamageInfo>>
Damage::getTotalReceivedFromPlayerPerDamageType() const;

std::vector<std::pair<std::string, DamageInfo>>
Damage::getTotalDealtOnPlayerPerDamageType() const;


// Flips position of damageType and category.
std::vector<std::pair<std::string, std::pair<std::string, DamageInfo>>>
Damage::getReceivedFromPlayerPerType(const std::string damageType) const;
// Flips position of damageType and category.
std::vector<std::pair<std::string, std::pair<std::string, DamageInfo>>>
Damage::getDealtOnPlayerPerType(const std::string damageType) const;

std::vector<std::pair<std::string, std::pair<std::string, DamageInfo>>>
Damage::getDamageReceivedFromPlayer() const {
    return receivedFromPlayer;
}

std::vector<std::pair<std::string, std::pair<std::string, DamageInfo>>>
Damage::getDamageDealtOnPlayer() const {
    return dealtOnPlayer;
}

Damage::DamageInfo& Damage::DamageInfo::operator+=(const DamageInfo& rhs) {
    total += rhs.total;
    count += rhs.count;

    if (rhs.max > max) {
        max = rhs.max;
    }
    if (rhs.min < min) {
        min = rhs.min;
    }

    return *this;
}
