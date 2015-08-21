#include "affected_player.h"
#include "line_info.h"


AffectedPlayer::AffectedPlayer(std::string name) : BasePlayer(name) {

}

void AffectedPlayer::add(LineInfo& lineInfo) {
    if (lineInfo.type == "damage") {
        addDamage(lineInfo);
    }
    else if (lineInfo.type == "heal") {
        addHeal(lineInfo);
    }
    else if (lineInfo.type == "nano") {
        addNano(lineInfo);
    }
}

void AffectedPlayer::addDamage(LineInfo& li) {
    if (li.dealer_name == getName()) {
        if (li.nanobots) {
            nanobotsDamage[li.subtype].add(li, "dealer");
        }
        else {
            regularDamage[li.subtype].add(li, "dealer");
        }
    }
    else if (li.receiver_name == getName()) {
        if (li.nanobots) {
            nanobotsDamage[li.subtype].add(li, "receiver");
        }
        else {
            regularDamage[li.subtype].add(li, "receiver");
        }
    }
    else {
        // Exception?
    }
}

void AffectedPlayer::addHeal(LineInfo& li) {
    /*
    Heals can come it three ways:
        1. "You got healed by Player for x."
        2. "You were healed for x."
        3. "You healed Player for x."
    There will always be a receiver and it will always be "You" because heals
    for other players can not be logged. This means that no AffectedPlayer
    other than "You" will have a received heal.

    The two subtypes of heals are potential and actual.
    A potential heal is a heal where the amount is what the caster rolled
    when executing the nano program. Both 1 and 3 are potential heals. An
    actual heal is the amount received. For example, if a player heals you for
    the potential amount of 3000 but you have only lost 1 HP, then the actual
    heal will be 1 point. If the subtype is not "potential", it is "actual"
    which only happens for case 2.
    */

    if (li.subtype == "actual") {
        heal.add(li, "receiver");
    }
    else if (li.subtype == "potential") {
        if (li.dealer_name == getName()) {
            heal.add(li, "dealer");
        }
        else if (li.receiver_name == getName()) {
            heal.add(li, "receiver");
        }
        else {
            // TODO: Throw exception?
        }
    }
    else {
        // TODO: Throw exception?
    }
}

void AffectedPlayer::addNano(LineInfo& li) {
    if (li.receiver_name == getName()) {
        nano.add(li, "receiver");
    }
    else if (li.dealer_name == getName()) {
        nano.add(li, "dealer");
    }
}

Damage AffectedPlayer::getTotalDamage() const {
    return getTotalDamage(true) + getTotalDamage(false);
}

Damage AffectedPlayer::getTotalDamage(bool nanobots) const {
    Damage d;
    if (nanobots)  {
        for (const auto& type : nanobotsDamage) {
            d += type.second;
        }
    }
    else {
        for (const auto& type : regularDamage) {
            d += type.second;
        }
    }
    return d;
}

Damage AffectedPlayer::getTotalDamagePerDamageType(const std::string damageType) const {
    return getTotalDamagePerDamageType(damageType, true) +
           getTotalDamagePerDamageType(damageType, false);
}

Damage AffectedPlayer::getTotalDamagePerDamageType(const std::string damageType, bool nanobots) const {
    if (nanobots) {
        auto it = nanobotsDamage.find(damageType);
        if (it != nanobotsDamage.end()) {
            return it->second;
        }
        else {
            Damage d;
            return d;  // Empty Damage
        }
    }
    else {
        auto it = regularDamage.find(damageType);
        if (it != regularDamage.end()) {
            return it->second;
        }
        else {
            Damage d;
            return d;  // Empty Damage
        }
    }
}

const std::map<std::string, Damage>& AffectedPlayer::getRegularDamage() const {
    return regularDamage;
}

const std::map<std::string, Damage>& AffectedPlayer::getNanobotsDamage() const {
    return nanobotsDamage;
}

const Heal& AffectedPlayer::getHeal() const {
    return heal;
}

const Nano& AffectedPlayer::getNano() const {
    return nano;
}
