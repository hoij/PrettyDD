#include "affected_player.h"
#include "affected_player_vector.h"
#include "line_info.h"
#include "player.h"

#include <algorithm>
#include <stdexcept>
#include <utility>


Player::Player(std::string name) : name(name) {
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>();
    myTime = new MyTime();
}

Player::Player(std::string name, MyTimeInterface* myTime) : name(name), myTime(myTime) {
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>();
}

Player::Player(std::string name,
               AffectedPlayerVector<AffectedPlayer*>* pv,
               MyTimeInterface* myTime) :
    name(name),
    affectedPlayers(pv),
    myTime(myTime) {}

Player::~Player() {
    delete affectedPlayers;
    delete myTime;
}

Player::Player(const Player& other) {
    name = other.name;
    affectedPlayers = new AffectedPlayerVector<AffectedPlayer*>;
    *affectedPlayers = *other.affectedPlayers;
	nanoPrograms = other.nanoPrograms;
    xp = other.xp;
}

Player::Player(Player&& other) NOEXCEPT : Player(other.getName()) {
    swap(*this, other);
}

Player& Player::operator=(Player rhs) {
    swap(*this, rhs);
    return *this;
}

void swap(Player& first, Player& second) {
    std::swap(first.name, second.name);
    std::swap(first.affectedPlayers, second.affectedPlayers);
    std::swap(first.xp, second.xp);
    std::swap(first.nanoPrograms, second.nanoPrograms);
}

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

Damage Player::getTotalDamage() const {
    Damage total = affectedPlayers->getTotalDamage(getName());
    int dpmR = amountPerMinute(total.getTotalReceivedFromPlayer());
    total.setReceivedFromPlayerDPM(dpmR);
    int dpmD = amountPerMinute(total.getTotalDealtOnPlayer());
    total.setDealtOnPlayerDPM(dpmD);
    return total;
}

Damage Player::getTotalDamagePerDamageType(std::string damageType) const {
    Damage total = affectedPlayers->getTotalDamagePerDamageType(getName(),
                                                                damageType);
    int dpmR = amountPerMinute(total.getTotalReceivedFromPlayer());
    total.setReceivedFromPlayerDPM(dpmR);
    int dpmD = amountPerMinute(total.getTotalDealtOnPlayer());
    total.setDealtOnPlayerDPM(dpmD);
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
Player::getTotalDamageForEveryDamageTypeReceivedFromPlayer() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageForEveryDamageTypeReceivedFromPlayer(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageForEveryDamageTypeDealtOnPlayer() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageForEveryDamageTypeDealtOnPlayer(getName());
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

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageReceivedFromPlayerForAllAffectedPlayers() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageReceivedFromPlayerForAllAffectedPlayers(getName());
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getTotalDamageDealtOnPlayerForAllAffectedPlayers() const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getTotalDamageDealtOnPlayerForAllAffectedPlayers(getName());
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

std::vector<std::pair<std::string, Damage>>
Player::getAllDamageTakenFromAffectedPlayer(std::string name) const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getAllDamageDealtOnPlayerForAffectedPlayer(name);
    addDPM(total);
    return total;
}

std::vector<std::pair<std::string, Damage>>
Player::getAllDamageDealtOnAffectedPlayer(std::string name) const {
    std::vector<std::pair<std::string, Damage>> total =
        affectedPlayers->getAllDamageReceivedFromPlayerForAffectedPlayer(name);
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

const NanoPrograms& Player::getNanoPrograms() const {
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
    xp.calcXPH(getTimeActive());
    return xp;
}

Nano Player::getTotalNano() const {
    return affectedPlayers->getTotalNano(getName());
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
        int DPMReceivedFromPlayer =
            amountPerMinute(damagePair.second.getTotalReceivedFromPlayer());
        int DPMDealtOnPlayer =
            amountPerMinute(damagePair.second.getTotalDealtOnPlayer());
        damagePair.second.setReceivedFromPlayerDPM(DPMReceivedFromPlayer);
        damagePair.second.setDealtOnPlayerDPM(DPMDealtOnPlayer);
    }
}
