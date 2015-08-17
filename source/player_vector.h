#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "logger.h"
#include "player.h"

#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

class Damage;
class Heal;
class LineInfo;

template<class C>
class PlayerVector {
public:
    PlayerVector() {}
    virtual ~PlayerVector();
    PlayerVector(PlayerVector& pv) = delete;
    PlayerVector& operator=(PlayerVector rhs) = delete;

    virtual void addToPlayers(LineInfo& lineInfo);
    virtual int getLongestNameLength() const;
    virtual const C getPlayer(std::string name);

    virtual Damage getTotalDamage(bool nanobots);
    virtual Damage getTotalDamagePerDamageType(std::string damageType,
                                               bool nanobots);
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEachPlayer() const;
    virtual Heal getTotalHeals() const;

    typedef typename std::vector<C>::iterator PlayerVectorIterator;
    virtual PlayerVectorIterator begin() {return players.begin();}
    virtual PlayerVectorIterator end() {return players.end();}
    typedef typename std::vector<C>::const_iterator const_PlayerVectorIterator;
    virtual const_PlayerVectorIterator begin() const {return players.begin();}
    virtual const_PlayerVectorIterator end() const {return players.end();}

private:
    void createPlayer(std::string name, LineInfo& lineInfo);
    static bool compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                               std::pair<std::string, Damage>& p2);
    std::vector<C> players;
};


template<class C>
PlayerVector<C>::~PlayerVector() {
    for (C p : players) {
        delete p;
    }
}

template<class C>
Damage PlayerVector<C>::getTotalDamage(bool nanobots) {
    Damage d;
    for (const C p : players) {
        d += p->getTotalDamage(nanobots);
    }
    return d;
}

template<class C>
Damage PlayerVector<C>::getTotalDamagePerDamageType(std::string damageType, bool nanobots) {
    Damage d;
    for (const C p : players) {
        d += p->getTotalDamagePerDamageType(damageType, nanobots);
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>> PlayerVector<C>::getTotalDamageForEachPlayer() const {
    // Returns a sorted vector of pairs containing the players name and their
    // total damage (in the form of the Damage class).
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C p : players) {
        totalDamagePerPlayer.push_back(
            std::make_pair(p->getName(), p->getTotalDamage()));
    }
    std::sort(totalDamagePerPlayer.begin(),
              totalDamagePerPlayer.end(),
              compareTotalReceivedFromPlayer);
    return totalDamagePerPlayer;
}

template<class C>
bool PlayerVector<C>::compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                                     std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceived() >
           p2.second.getTotalReceived();
}

template<class C>
Heal PlayerVector<C>::getTotalHeals() const {
    Heal h;
    for (const C p : players) {
        h += p->getTotalHeals();
    }
    return h;
}

template<class C>
void PlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    for (C p : players) {
        if (p->getName() == lineInfo.dealer_name) {
            p->add(lineInfo);
            dealerFound = true;
        }
        else if (p->getName() == lineInfo.receiver_name) {
            p->add(lineInfo);
            receiverFound = true;
        }
    }
    if(!dealerFound && lineInfo.dealer_name != "") {
        createPlayer(lineInfo.dealer_name, lineInfo);
    }
    if(!receiverFound && lineInfo.receiver_name != "") {
        createPlayer(lineInfo.receiver_name, lineInfo);
    }
}

template<class C>
void PlayerVector<C>::createPlayer(std::string name, LineInfo& lineInfo) {
    // Get the type pointed to by C
    typedef typename std::remove_pointer<C>::type CNoPointer;

    C p = new CNoPointer(name);
    p->add(lineInfo);
    players.push_back(p);
}

template<class C>
const C PlayerVector<C>::getPlayer(std::string name) {
    for (C p : players) {
        if (p->getName() == name) {
            return p;
        }
    }
    errorLog.write("Could not find a player with the name " + name);
    return nullptr;
}

template<class C>
int PlayerVector<C>::getLongestNameLength() const {
    unsigned int longestNameLength = 0;
    for (const C p : players) {
        if (p->getName().length() > longestNameLength) {
            longestNameLength = p->getName().length();
        }
    }
    return longestNameLength;
}


#endif // PLAYER_VECTOR_H
