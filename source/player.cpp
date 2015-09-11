#include "affected_player.h"
#include "affected_player_vector.h"
#include "line_info.h"
#include "player.h"

#include <algorithm>
#include <stdexcept>
#include <utility>


Player::Player(std::string name) : name(name) {
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>();
}

Player::Player(std::string name, AffectedPlayerVector<AffectedPlayer*>* pv) :
    name(name),
    affectedPlayers(pv) {}

Player::~Player() {
    delete affectedPlayers;
}

Player::Player(const Player& other) {
    name = other.name;
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>;
    *affectedPlayers = *other.affectedPlayers;
    nameOfLastNanoProgramCasted = other.nameOfLastNanoProgramCasted;
	nanoPrograms = other.nanoPrograms;
    xp = other.xp;
}

Player::Player(Player&& other) noexcept : Player(other.getName()) {
    swap(*this, other);
}

Player& Player::operator=(Player rhs) {
    swap(*this, rhs);
    return *this;
}

void swap(Player& first, Player& second) {
    std::swap(first.name, second.name);
    std::swap(first.affectedPlayers, second.affectedPlayers);
    std::swap(first.nameOfLastNanoProgramCasted, second.nameOfLastNanoProgramCasted);
    std::swap(first.xp, second.xp);
    std::swap(first.nanoPrograms, second.nanoPrograms);
}

void Player::add(LineInfo& lineInfo) {
    if (lineInfo.type == "damage" || lineInfo.type == "heal" || lineInfo.type == "nano") {
        affectedPlayers->addToPlayers(lineInfo);
    }
    else if (lineInfo.type == "nano cast") {
        if (lineInfo.nanoProgramName != "") {
            // When a nano program is casted there is no message about the
            // success of the cast. That success info arrives in the next
            // message. So in this case, the name is only saved.
            nameOfLastNanoProgramCasted = lineInfo.nanoProgramName;
        }
        else {
            // If nano casted successfully/resisted/countered/aborted/fumble,
            // the message won't contain a name. The previously saved name
            // is added with the new subtype info (the success info).
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
    for (auto& nanoProgram : nanoPrograms) {
        if (nanoProgram.getName() == name) {
            nanoProgram.addAction(subtype);
            break;
        }
    }
    nanoPrograms.emplace_back(name, subtype);
}

Damage Player::getTotalDamage() const {
    return affectedPlayers->getTotalDamage(getName());
}

Damage Player::getTotalDamagePerDamageType(std::string damageType) const {
    return affectedPlayers->getTotalDamagePerDamageType(getName(), damageType);
}

std::vector<std::pair<std::string, Damage>> Player::getTotalDamageForEveryDamageType() const {
    return affectedPlayers->getTotalDamageForEveryDamageType(getName());
}

std::vector<std::pair<std::string, Damage>> Player::getTotalDamageForAllAffectedPlayers() const {
    return affectedPlayers->getTotalDamageForAllAffectedPlayers(getName());
}

//bool Player::compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
//                                       std::pair<std::string, Damage>& p2) {
//    return p1.second.getTotalDealt() >
//           p2.second.getTotalDealt();
//}

size_t Player::getLongestAffectedPlayerNameLength() const {
    return affectedPlayers->getLongestNameLength();
}

Heal Player::getTotalHeals() const {
    return affectedPlayers->getTotalHeals(getName());
}

std::vector<std::pair<std::string, Damage>> Player::getAllDamageFromAffectedPlayer(std::string name) const {
    return affectedPlayers->getAllDamageFromAffectedPlayer(name);
}

std::vector<std::pair<std::string, Heal>> Player::getHealsForAllAffectedPlayers() const {
    return affectedPlayers->getHealsForAllAffectedPlayers();
}

const Heal& Player::getHealFromAffectedPlayer(std::string name) const {
    return affectedPlayers->getHealFromAffectedPlayer(name);
}

const std::vector<NanoProgram>& Player::getNanoPrograms() const {
    return nanoPrograms;
}

std::vector<std::pair<std::string, Nano>> Player::getNanoForAllAffectedPlayers() const {
    return affectedPlayers->getNanoForAllAffectedPlayers();
}

const Nano& Player::getNanoFromAffectedPlayer(std::string name) const {
    return affectedPlayers->getNanoFromAffectedPlayer(name);
}

const XP& Player::getXp() {
    return xp;
}

Nano Player::getTotalNano() const {
    return affectedPlayers->getTotalNano(getName());
}
