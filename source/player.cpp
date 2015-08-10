#include "line_info.h"
#include "player.h"

#include <algorithm>


Player::Player(std::string name) : BasePlayer(name) {}

void Player::add(LineInfo& lineInfo) {
    if (lineInfo.type == "damage" || lineInfo.type == "heal" || lineInfo.type == "nano") {
        affectedPlayers.addToPlayers(lineInfo);
    }
    else if (lineInfo.type == "nano cast") {
    // Only add the nano when a message about the success/fail has arrived.
    // Because in that case, the nano will not be mentioned by name.
        if (lineInfo.nanoProgramName != "") {
            nameOfLastNanoProgramCasted = lineInfo.nanoProgramName;
        }
        else {
            // If nano casted successfully/resisted/countered/aborted/fumble

            // TODO: Must the last nano casted be a real nano or can it be just a name?
            addNanoProgram(nameOfLastNanoProgramCasted, lineInfo.subtype);
        }
    }
    else if (lineInfo.type == "xp") {
        addXp(lineInfo);
    }
}

void Player::addXp(LineInfo& lineInfo) {
    xp.add(lineInfo);
}

void Player::addNanoProgram(std::string name, std::string subtype) {
    // Adds a nano program if it hasn't already been added.
    for (auto& npInVec : nanoPrograms) {
        if (npInVec.getName() == name) {
            npInVec.addAction(subtype);
            return;
        }
    }
    nanoPrograms.push_back(NanoProgram(name, subtype));
}

Damage Player::getTotalDamage() const {
    return getTotalDamage(true) + getTotalDamage(false);
}

Damage Player::getTotalDamage(bool nanobots) const {
    Damage d;
    for (const AffectedPlayer& ap : affectedPlayers) {
        if (ap.getName() != getName()) {
            d += ap.getTotalDamage(nanobots);
        }
    }
    return d;
}

Damage Player::getTotalDamagePerDamageType(const std::string damageType) {
    return getTotalDamagePerDamageType(damageType, false) +
           getTotalDamagePerDamageType(damageType, true);
}

Damage Player::getTotalDamagePerDamageType(const std::string damageType, bool nanobots) {
    Damage d;
    for (const AffectedPlayer& ap : affectedPlayers) {
        d += ap.getTotalDamagePerDamageType(damageType, nanobots);
    }
    return d;
}

std::vector<std::pair<std::string, Damage>> Player::getTotalDamagePerAffectedPlayer() const {
    std::vector<std::pair<std::string, Damage>> totalDamagePerAffectedPlayer;
    for (const AffectedPlayer& ap : affectedPlayers) {
        if (ap.getName() != getName()) {
            totalDamagePerAffectedPlayer.push_back(
                std::make_pair(ap.getName(), ap.getTotalDamage()));
            }
    }
    std::sort(totalDamagePerAffectedPlayer.begin(),
              totalDamagePerAffectedPlayer.end(),
              compareTotalReceivedFromPlayer);
    return totalDamagePerAffectedPlayer;
}

bool Player::compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
                                       std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalDealt() >
           p2.second.getTotalDealt();
}

bool Player::compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                       std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceived() >
           p2.second.getTotalReceived();
}

unsigned int Player::getLongestAffectedPlayerNameLength() const {
    return affectedPlayers.getLongestNameLength();
}

Heal Player::getTotalHeals() {
    Heal h;
    for (const AffectedPlayer& ap : affectedPlayers) {
        h += ap.getHeal();
    }
    return h;
}

const std::vector<NanoProgram>& Player::getNanoPrograms() {
    return nanoPrograms;
}

const XP& Player::getXp() {
    return xp;
}

Nano Player::getTotalNano() {
    Nano n;
    for (const AffectedPlayer& ap : affectedPlayers) {
        n += ap.getNano();
    }
    return n;
}

PlayerVector<AffectedPlayer>& Player::getAffectedPlayers()  {
    return affectedPlayers;
}
