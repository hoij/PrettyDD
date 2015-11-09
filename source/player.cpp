#include "affected_player.h"
#include "affected_player_factory.h"
#include "affected_player_factory_interface.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "my_time.h"
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
               std::shared_ptr<MyTimeInterface> myTime) :
    name(name),
    affectedPlayers(affectedPlayers),
    myTime(myTime) {}

void Player::add(LineInfo& lineInfo) {
    timeOfLastAction = lineInfo.time;
    if (startTime == 0) {
        // Set the start time on the first action made.
        startTime = lineInfo.time;
    }
    if (lineInfo.type == "damage" || lineInfo.type == "heal" || lineInfo.type == "nano") {
        affectedPlayers->addToPlayers(lineInfo);
    }
    else if (lineInfo.type == "nano cast") {
        nanoPrograms.add(lineInfo);
    }
    else if (lineInfo.type == "xp" ||
             lineInfo.type == "sk" ||
             lineInfo.type == "research" ||
             lineInfo.type == "aixp" ||
             lineInfo.type == "PVP Solo Score" ||
             lineInfo.type == "PVP Team Score") {
        addXp(lineInfo);
    }
}

void Player::addXp(LineInfo& lineInfo) {
    xp.add(lineInfo);
}

Damage Player::getTotalDamageDealt() const {
    Damage total = affectedPlayers->getTotalDamageReceivedFromPlayer(getName());
    int dpm = amountPerMinute(total.getTotal());
    total.setDPM(dpm);
    return total;
}

Damage Player::getTotalDamageReceived() const {
    Damage total = affectedPlayers->getTotalDamageDealtOnPlayer(getName());
    int dpm = amountPerMinute(total.getTotal());
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

size_t Player::getLongestAffectedPlayerNameLength() const {
    return affectedPlayers->getLongestNameLength();
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
    xp.calcXPH(getTimeActive());
    return xp;
}

std::time_t Player::getTimeActive() const {
    // Start time is set the first time the add method of a new
    // Player is called.
    return timeOfLastAction - startTime - getPauseDuration();
}

std::time_t Player::getPauseDuration() const {
    std::time_t pauseDuration = 0;
    for (const Pause& pause : pauses) {
        if (timeOfLastAction >= pause.stop) {
            pauseDuration += pause.stop - pause.start;
        }
    }
    return pauseDuration;
}

void Player::stopTimer() {
    stopTime = myTime->currentTime();
}

void Player::resumeTimer() {
    Pause p = {stopTime, myTime->currentTime()};
    pauses.push_back(p);
    stopTime = 0;
}

int Player::amountPerMinute(int amount) const {
    std::time_t t = getTimeActive();
    if (t == 0) {
        return 0;
    }
    else {
        return int(amount / ((double)t / 60));
    }
}

void Player::addDPM(std::vector<std::pair<std::string, Damage>>& v) const {
    for (auto& damagePair : v) {
        int dpm = amountPerMinute(damagePair.second.getTotal());
        damagePair.second.setDPM(dpm);
    }
}
