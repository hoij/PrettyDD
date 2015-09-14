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

Player::Player(std::string name, MyTime* myTime) : name(name), myTime(myTime) {
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>();
    myTime = new MyTime;
}

Player::Player(std::string name,
               AffectedPlayerVector<AffectedPlayer*>* pv,
               MyTime* myTime) :
    name(name),
    affectedPlayers(pv),
    myTime(myTime) {}

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
    if (startTime == 0) {
        // Set the start time on the first action made.
        startTime = myTime->currentTime();
    }
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
    Damage total = affectedPlayers->getTotalDamage(getName());
    int dpm = amountPerMinute(total.getTotalReceivedFromPlayer());
    total.setReceivedFromPlayerDPM(dpm);
    return total;
}

Damage Player::getTotalDamagePerDamageType(std::string damageType) const {
    Damage total = affectedPlayers->getTotalDamagePerDamageType(getName(),
                                                                damageType);
    int dpm = amountPerMinute(total.getTotalReceivedFromPlayer());
    total.setReceivedFromPlayerDPM(dpm);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageForEveryDamageType() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageForEveryDamageType(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageForAllAffectedPlayers() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageForAllAffectedPlayers(getName());
    addDPM(total);
    return total;
}

size_t Player::getLongestAffectedPlayerNameLength() const {
    return affectedPlayers->getLongestNameLength();
}

Heal Player::getTotalHeals() const {
    return affectedPlayers->getTotalHeals(getName());
}

std::vector<std::pair<std::string, Damage>>
Player::getAllDamageFromAffectedPlayer(std::string name) const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getAllDamageFromAffectedPlayer(name);
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Heal>>
Player::getHealsForAllAffectedPlayers() const {
    return affectedPlayers->getHealsForAllAffectedPlayers();
}

const Heal& Player::getHealFromAffectedPlayer(std::string name) const {
    return affectedPlayers->getHealFromAffectedPlayer(name);
}

const std::vector<NanoProgram>& Player::getNanoPrograms() const {
    return nanoPrograms;
}

std::vector<std::pair<std::string, Nano>>
Player::getNanoForAllAffectedPlayers() const {
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

std::time_t Player::getTimeActive() const {
    // Need to check that there is a start time?
    // I don't think so because a start time should be set as soon
    // as add is called which it always is for a new player.
    std::time_t timeActive;
    if (stopTime == 0) {
        timeActive = myTime->currentTime() - startTime - pauseDuration;
    }
    else {
        timeActive = stopTime - startTime - pauseDuration;
    }
    return timeActive;
}

void Player::stopTimer() {
    stopTime = myTime->currentTime();
}

void Player::resumeTimer() {
    pauseDuration += myTime->currentTime() - stopTime;
    stopTime = 0;
    affectedPlayers->incrementPauseDuration(pauseDuration);
}

int Player::amountPerMinute(int amount) const {
    std::time_t t = getTimeActive();
    if (t == 0) {
        return 0;
    }
    else {
        return amount / ((double)t / 60);
    }
}

void Player::addDPM(std::vector<std::pair<std::string, Damage>>& v) const {
    for (auto& damagePair : v) {
        int DPMReceivedFromPlayer =
            amountPerMinute(damagePair.second.getTotalReceivedFromPlayer());
        int DPMDealtOnPlayer =
            amountPerMinute(damagePair.second.getTotalDealtOnPlayer());
        damagePair.second.setReceivedFromPlayerDPM(DPMReceivedFromPlayer);
        damagePair.second.setDealtOnPlayerDPM(DPMDealtOnPlayer);
    }
}
