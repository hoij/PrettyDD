#include "player.h"
#include <algorithm>


Player::Player() {
}

Player::Player(std::string name, LogLine& logLine) : BasePlayer(name) {
    add(logLine);
}

Player::~Player() {
}

Player::Player(const Player& p) : BasePlayer(p.getName()) {
    last_nano_casted = p.last_nano_casted;
    nanoPrograms = p.nanoPrograms;
    affectedPlayers = p.affectedPlayers;
    xp = p.xp;
}

Player& Player::operator=(const Player& p) {
    if (this != &p) {
        setName(p.getName());
        last_nano_casted = p.last_nano_casted;
        nanoPrograms = p.nanoPrograms;
        affectedPlayers = p.affectedPlayers;
        xp = p.xp;
    }
    return *this;
}

void Player::add(LogLine& logLine) {
        LineInfo& li = logLine.getInfo();
    if (li.type == "damage" || li.type == "heal" || li.type == "nano") {
        affectedPlayers.addToPlayers(logLine);
    }
    else if (li.type == "nano_cast") {
    // Only add the nano when a message about the success/fail has arrived.
    // Because in that case, the nano will not be mentioned by name.
        if (li.nanoProgram->getName() != "") {
            last_nano_casted = *li.nanoProgram;
        }
        else {
            // If nano casted successfully/resisted/countered/aborted/fumble/interrupt?
            last_nano_casted.addStat(li.subtype, 1);
            addNanoProgram(last_nano_casted);
        }
    }
    else if (li.type == "xp") {
        addXp(li);
    }
}

void Player::addXp(LineInfo& li) {
    xp.add(li);
}

void Player::addNanoProgram(NanoProgram& np) {
    // Adds a nano program if it hasn't already been added.
    for (auto& npInVec : nanoPrograms) {
        if (npInVec.getName() == np.getName()) {
            npInVec += np;
            return;
        }
    }
    nanoPrograms.push_back(np);
}

Damage Player::getTotalDamage() {
    return getTotalDamage(true) + getTotalDamage(false);
}

Damage Player::getTotalDamage(bool nanobots) {
    Damage d;
    for (AffectedPlayer& ap : affectedPlayers) {
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
    for (AffectedPlayer& ap : affectedPlayers) {
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
    for (AffectedPlayer& ap : affectedPlayers) {
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
    for (AffectedPlayer& ap : affectedPlayers) {
        n += ap.getNano();
    }
    return n;
}

PlayerVector<AffectedPlayer>& Player::getAffectedPlayers()  {
    return affectedPlayers;
}
