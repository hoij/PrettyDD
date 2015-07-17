#include "affected_player.h"


AffectedPlayer::AffectedPlayer() {
}

AffectedPlayer::AffectedPlayer(std::string name, LineInfo& lineInfo) : BasePlayer(name) {
    add(lineInfo);
}

AffectedPlayer::~AffectedPlayer(){
}

AffectedPlayer::AffectedPlayer(const AffectedPlayer& p) : BasePlayer(p.getName()) {
    regularDamage = p.regularDamage;
    nanobotsDamage = p.nanobotsDamage;
    heal = p.heal;
    nano = p.nano;
}

AffectedPlayer& AffectedPlayer::operator=(const AffectedPlayer& p) {
    if (this != &p) {
        setName(p.getName());
        nanobotsDamage = p.nanobotsDamage;
        regularDamage = p.regularDamage;
        heal = p.heal;
        nano = p.nano;
    }
    return *this;
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

Damage AffectedPlayer::getTotalDamagePerDamageType(const std::string damageType) {
    return getTotalDamagePerDamageType(damageType, true) +
           getTotalDamagePerDamageType(damageType, false);
}

Damage AffectedPlayer::getTotalDamagePerDamageType(const std::string damageType, bool nanobots) {
    if (nanobots) {
        return nanobotsDamage[damageType];
    }
    else {
        return regularDamage[damageType];
    }
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
