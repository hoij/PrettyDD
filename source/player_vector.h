#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "base_vector.h"
#include "logger.h"
#include "my_time.h"
#include "player.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

class Damage;
class Heal;
class LineInfo;

template<class C>
class PlayerVector : public BaseVector<C> {
public:
    PlayerVector() {}
    virtual ~PlayerVector() {};
    PlayerVector(const PlayerVector<C>& other) : BaseVector<C>(other) {}

    virtual void addToPlayers(LineInfo& lineInfo);

    virtual Damage getTotalDamage() const;
    virtual Damage getTotalDamagePerDamageType(std::string damageType) const;
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageDealtForEachPlayer() const;
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageReceivedForEachPlayer() const;
    virtual Heal getTotalHeals() const;
    virtual void startLogging();
    virtual void stopLogging();
    virtual void reset();

private:
    bool log = false;
};


template<class C>
void PlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    if (!log) {
        // Don't log when stopped.
        return;
    }

    for (C player : this->players) {
        if (player->getName() == lineInfo.dealer_name) {
            player->add(lineInfo);
            dealerFound = true;
        }
        else if (player->getName() == lineInfo.receiver_name) {
            player->add(lineInfo);
            receiverFound = true;
        }
    }
    if(!dealerFound && lineInfo.dealer_name != "") {
        this->createPlayer(lineInfo.dealer_name, lineInfo);
    }
    if(!receiverFound && lineInfo.receiver_name != "") {
        this->createPlayer(lineInfo.receiver_name, lineInfo);
    }
}

template<class C>
Damage PlayerVector<C>::getTotalDamage() const {
    Damage d;
    for (const C p : this->players) {
        d += p->getTotalDamage();
    }
    return d;
}

template<class C>
Damage PlayerVector<C>::getTotalDamagePerDamageType(std::string damageType) const {
    Damage d;
    for (const C p : this->players) {
        d += p->getTotalDamagePerDamageType(damageType);
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>> PlayerVector<C>::getTotalDamageDealtForEachPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : this->players) {
        Damage d = p->getTotalDamage();
        if (d.getCountReceivedFromPlayer() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>> PlayerVector<C>::getTotalDamageReceivedForEachPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage (in the form of the Damage class). */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : this->players) {
        Damage d = p->getTotalDamage();
        if (d.getCountDealtOnPlayer() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
Heal PlayerVector<C>::getTotalHeals() const {
    Heal h;
    for (const C p : this->players) {
        h += p->getTotalHeals();
    }
    return h;
}

template<class C>
void PlayerVector<C>::stopLogging() {
    log = false;
    for (const auto& player : this->players) {
        player->stopTimer();
    }
}

template<class C>
void PlayerVector<C>::startLogging() {
    log = true;
    for (const auto& player : this->players) {
        if (player->getStartTime() != 0) {
            player->resumeTimer();
        }
    }
}

template<class C>
void PlayerVector<C>::reset() {
    for (C player : this->players) {
        delete player;
    }
    this->players.clear();
}


#endif // PLAYER_VECTOR_H
