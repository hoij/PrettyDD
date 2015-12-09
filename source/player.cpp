#include "affected_player.h"
#include "affected_player_factory.h"
#include "affected_player_factory_interface.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "nano.h"
#include "player.h"

#include <algorithm>
#include <stdexcept>
#include <utility>


// Takes shared pointers for the only reason that the test cases must
// be able to use them at the same time. Unique pointers would
// have been better as Player is the only one owning them but they
// would've been moved and unreachable.
Player::Player(std::string name,
               std::shared_ptr<AffectedPlayerVector> affectedPlayers,
               std::unique_ptr<PlayerTime> playerTime) :
    name(name),
    affectedPlayers(affectedPlayers),
    playerTime(std::move(playerTime)) {}

void Player::add(LineInfo& lineInfo) {

    playerTime->setTimeOfLastAction(lineInfo.time);

    if (lineInfo.type == LineType::damage ||
        lineInfo.type == LineType::heal ||
        lineInfo.type == LineType::nano) {
        affectedPlayers->addToPlayers(lineInfo);
    }
    else if (lineInfo.type == LineType::nanoCast) {
        nanoPrograms.add(lineInfo);
    }
    else if (lineInfo.type == LineType::xp ||
             lineInfo.type == LineType::sk ||
             lineInfo.type == LineType::research ||
             lineInfo.type == LineType::aixp ||
             lineInfo.type == LineType::pvpSoloScore ||
             lineInfo.type == LineType::pvpTeamScore) {
        addXp(lineInfo);
    }
}

void Player::addXp(LineInfo& lineInfo) {
    xp.add(lineInfo);
}

Damage Player::getTotalDamageDealt() const {
    Damage total = affectedPlayers->getTotalDamageReceivedFromPlayer(getName());
    int dpm = playerTime->amountPerMinute(total.getTotal());
    total.setDPM(dpm);
    return total;
}

Damage Player::getTotalDamageReceived() const {
    Damage total = affectedPlayers->getTotalDamageDealtOnPlayer(getName());
    int dpm = playerTime->amountPerMinute(total.getTotal());
    total.setDPM(dpm);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageDealtPerType() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageReceivedFromPlayerPerDamageType(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageReceivedPerType() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageDealtOnPlayerPerDamageType(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageDealtPerAffectedPlayer() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageReceivedFromPlayerPerAffectedPlayer(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageReceivedPerAffectedPlayer() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageDealtOnPlayerPerAffectedPlayer(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getDamageReceivedPerType(std::string affectedPlayerName) const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getDamageDealtOnPlayer(affectedPlayerName);
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getDamageDealtPerType(std::string affectedPlayerName) const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getDamageReceivedFromPlayer(affectedPlayerName);
    addDPM(total);
    return total;
}

/* Heal */
Heal Player::getTotalHeals() const {
    return affectedPlayers->getTotalHeals(getName());
}

std::vector<std::pair<std::string, Heal>>
Player::getHealsPerAffectedPlayer() const {
    return affectedPlayers->getHealsPerAffectedPlayer();
}

Heal Player::getHeal(std::string affectedPlayerName) const {
    return affectedPlayers->getHeal(affectedPlayerName);
}

/* Nano */
Nano Player::getTotalNano() const {
    return affectedPlayers->getTotalNano(getName());
}

std::vector<std::pair<std::string, Nano>>
Player::getNanoPerAffectedPlayer() const {
    return affectedPlayers->getNanoPerAffectedPlayer();
}

Nano Player::getNano(std::string affectedPlayerName) const {
    return affectedPlayers->getNano(affectedPlayerName);
}

/* Nano Program*/
const NanoPrograms& Player::getNanoPrograms() const {
    return nanoPrograms;
}

/* XP */
const XP& Player::getXp() {
    xp.calcXPH(playerTime->getTimeActive());
    return xp;
}

std::time_t Player::getStartTime() const {
    return playerTime->getStartTime();
}

std::time_t Player::getTimeActive() const {
    return playerTime->getTimeActive();
}

void Player::stopTimer() {
    playerTime->stopTimer();
}

void Player::resumeTimer() {
    playerTime->resumeTimer();
}

size_t Player::getLongestAffectedPlayerNameLength() const {
    return affectedPlayers->getLongestNameLength();
}

std::vector<AffectedPlayer*>::size_type Player::nrOfAffectedPlayers() {
    return affectedPlayers->size();
}

void Player::addDPM(std::vector<std::pair<std::string, Damage>>& v) const {
    for (auto& damagePair : v) {
        int dpm = playerTime->amountPerMinute(damagePair.second.getTotal());
        damagePair.second.setDPM(dpm);
    }
}
