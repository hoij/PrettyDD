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
}

void AffectedPlayer::addDamage(LineInfo& li) {
    if (li.receiver_name != getName()) {
        if (li.nanobots) {
            nanobotsDamage[li.subtype].add(li, "dealer");
        }
        else {
            regularDamage[li.subtype].add(li, "dealer");
        }
    }
    else if (li.dealer_name != getName()) {
        if (li.nanobots) {
            nanobotsDamage[li.subtype].add(li, "receiver");
        }
        else {
            regularDamage[li.subtype].add(li, "receiver");
        }
    }
}

void AffectedPlayer::addHeal(LineInfo& li) {
    // There will alywas be a receiver.
    heal.add(li, "receiver");
    if (li.subtype == "potential") {
        // But only a dealer in case the subtype is "potential".
        heal.add(li, "dealer");
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

bool AffectedPlayer::compareTotalDealt(AffectedPlayer& ap1, AffectedPlayer& ap2) {
    return ap1.getTotalDamage().getTotalDealt() >
           ap2.getTotalDamage().getTotalDealt();
}
