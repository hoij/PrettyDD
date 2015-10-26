#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "base_vector.h"
#include "player.h"

#include <string>
#include <vector>

class Damage;
class Heal;
class LineInfo;

template<class C>
class PlayerVector : public BaseVector<C> {
public:
    PlayerVector(std::string playerRunningProgram) :
        playerRunningProgram(playerRunningProgram) {}
    ~PlayerVector() {};
    PlayerVector(const PlayerVector<C>& other) : BaseVector<C>(other) {}

    void addToPlayers(LineInfo& lineInfo);

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageDealtPerPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getTotalDamageReceivedPerPlayer() const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerType(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedPerType(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerType(std::string playerName,
                          std::string opponentName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageDealtPerOpponent(std::string playerName) const;

    std::vector<std::pair<std::string, Damage>>
    getDamageReceivedPerOpponent(std::string playerName) const;


    std::string renameIfSelf(std::string name) const;
    void startLogging();
    void stopLogging();
    void reset();

private:
    bool log = false;
    std::string playerRunningProgram;
};


template<class C>
void PlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    if (!log) {  // Don't log when stopped.
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
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getTotalDamageDealtPerPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage dealt (in the form of the Damage class). Players
    with an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : this->players) {
        Damage d = p->getTotalDamageDealt();
        if (d.getCount() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getTotalDamageReceivedPerPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage received (in the form of the Damage class). Players
    with an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : this->players) {
        Damage d = p->getTotalDamageReceived();
        if (d.getCount() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getDamageDealtPerType(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const C p : this->players) {
        if (p->getName() == pName) {
            return p->getTotalDamageDealtPerType();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getDamageReceivedPerType(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const C p : this->players) {
        if (p->getName() == pName) {
            return p->getTotalDamageReceivedPerType();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getDamageDealtPerType(std::string playerName,
                                       std::string opponentName) const {
    /* There is no DR version of this method as the same result
    can be achieved by swapping player and opponent when calling
    this method. */
    std::string pName = renameIfSelf(playerName);
    std::string oppName = renameIfSelf(opponentName);
    for (const C p : this->players) {
        if (p->getName() == pName) {
            return p->getDamageDealtPerType(oppName);
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getDamageDealtPerOpponent(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const C p : this->players) {
        if (p->getName() == pName) {
            return p->getTotalDamageDealtPerAffectedPlayer();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

template<class C>
std::vector<std::pair<std::string, Damage>>
PlayerVector<C>::getDamageReceivedPerOpponent(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const C p : this->players) {
        if (p->getName() == pName) {
            return p->getTotalDamageReceivedPerAffectedPlayer();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

template<class C>
std::string PlayerVector<C>::renameIfSelf(std::string name) const {
    return (name == playerRunningProgram ? "You" : name);
}

template<class C>
void PlayerVector<C>::stopLogging() {
    if (log) { // Only stop if logging is ongoing
        log = false;
        for (const auto& player : this->players) {
            player->stopTimer();
        }
    }
}

template<class C>
void PlayerVector<C>::startLogging() {
    if (!log) { // Only start if logging has been stopped
        log = true;
        for (const auto& player : this->players) {
            if (player->getStartTime() != 0) {
                player->resumeTimer();
            }
        }
    }
}

template<class C>
void PlayerVector<C>::reset() {
    for (C player : this->players) {
        delete player;
    }
    this->players.clear();
    log = true;
}


#endif // PLAYER_VECTOR_H
