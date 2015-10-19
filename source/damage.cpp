#include "damage.h"
#include "line_info.h"

#include <algorithm>
#include <ostream>


Damage& Damage::operator+=(const Damage& rhs) {
    for (const auto& category : rhs.receivedFromPlayer) {
        for (const auto& subtype : category.second) {
            receivedFromPlayer[category.first][subtype.first] += subtype.second;
        }
    }
    for (auto const& category : rhs.dealtOnPlayer) {
        for (const auto& subtype : category.second) {
            dealtOnPlayer[category.first][subtype.first] += subtype.second;
        }
    }
    return *this;
}

void Damage::addDamageDealtOnPlayer(LineInfo& li) {
    addDamage(li, dealtOnPlayer);
}

void Damage::addDamageReceivedFromPlayer(LineInfo& li) {
    addDamage(li, receivedFromPlayer);
}

void Damage::addDamage(LineInfo& li,
    std::map<std::string, std::map<std::string, DamageInfo>>& di) {

    di[li.damageCategory][li.subtype].add(li.amount);
}

Damage::DamageInfo Damage::getTotalReceivedFromPlayer() const {
    return getTotal(receivedFromPlayer);
}

Damage::DamageInfo Damage::getTotalDealtOnPlayer() const {
    return getTotal(dealtOnPlayer);
}

Damage::DamageInfo
Damage::getTotal(const damageMap& m) const {
    Damage::DamageInfo di;
    for (const auto& category : m) {
        for (const auto& subtype : category.second) {
            di += subtype.second;
        }
    }
    return di;
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalReceivedFromPlayerPerCategory() const {
    return getTotalPerCategory(receivedFromPlayer);
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalDealtOnPlayerPerCategory() const {
    return getTotalPerCategory(dealtOnPlayer);
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalPerCategory(const damageMap& m) const {
    /* Returns a map containing the name of the category
    and it's total damage summed over all subtypes. */

    std::map<std::string, DamageInfo> summedCategories;
    for (const auto& category : m) {
        for (const auto& subtype : category.second) {
            summedCategories[category.first] += subtype.second;
        }
    }
    return summedCategories;
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalReceivedFromPlayerPerDamageType() const {
    return getTotalPerSubtype(receivedFromPlayer);
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalDealtOnPlayerPerDamageType() const {
    return getTotalPerSubtype(dealtOnPlayer);
}

std::map<std::string, Damage::DamageInfo>
Damage::getTotalPerSubtype(const damageMap& m) const {
    /* Returns a map containing the name of the subtype
    and it's total damage summed over all categories. */

    std::map<std::string, Damage::DamageInfo> summedSubtypes;
    for (const auto& category : m) {
        for (const auto& subtype : category.second) {
            summedSubtypes[subtype.first] += subtype.second;
        }
    }
    return summedSubtypes;
}

Damage::damageMap Damage::getReceivedFromPlayerPerSubtype() const {
    return getPerSubtype(receivedFromPlayer);
}

Damage::damageMap
Damage::getDealtOnPlayerPerSubtype() const {
    return getPerSubtype(dealtOnPlayer);
}

Damage::damageMap Damage::getPerSubtype(const damageMap& m) const {
    // Flips position of subtype and category.
    Damage::damageMap perSubtype;
    for (const auto& category : m) {
        for (const auto& subtype : category.second) {
            perSubtype[subtype.first][category.first] += (subtype.second);
        }
    }
    return perSubtype;
}

Damage::damageMap Damage::getReceivedFromPlayer() const {
    return receivedFromPlayer;
}

Damage::damageMap Damage::getDealtOnPlayer() const {
    return dealtOnPlayer;
}

void Damage::setReceivedFromPlayerDPM(int t) {
    setDPM(t, receivedFromPlayer);
}

void Damage::setDealtOnPlayerDPM(int t) {
    setDPM(t, dealtOnPlayer);
}

void Damage::setDPM(int t, damageMap m) {
    for (auto& category : m) {
        for (auto& subtype : category.second) {
            subtype.second.dpm = calcDPM(subtype.second.total, t);
        }
    }
}

int Damage::calcDPM(int amount, int t) {
    if (t == 0) {
        return 0;
    }
    else {
        return int(amount / ((double)t / 60));
    }
}

void Damage::DamageInfo::add(int amount) {
    total += amount;
    count += 1;

    if (amount > max) {
        max = amount;
    }
    if (amount < min) {
        min = amount;
    }
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
