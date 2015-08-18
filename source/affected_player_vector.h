#ifndef AFFECTED_PLAYER_VECTOR_H
#define AFFECTED_PLAYER_VECTOR_H


#include "affected_player.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "logger.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

template<class C>
class AffectedPlayerVector {
public:
    AffectedPlayerVector() {}
    virtual ~AffectedPlayerVector();
    AffectedPlayerVector(const AffectedPlayerVector<C>& ap);
    AffectedPlayerVector(AffectedPlayerVector<C>&& other);
    AffectedPlayerVector& operator=(AffectedPlayerVector<C> rhs);
    template<class T>
    friend void swap(AffectedPlayerVector<T>& first, AffectedPlayerVector<T>& second);

    virtual void addToPlayers(LineInfo& lineInfo);
    virtual int getLongestNameLength() const;
    virtual const C getPlayer(std::string name);

    virtual Damage getTotalDamage(std::string callersName, bool nanobots);
    virtual Damage getTotalDamagePerDamageType(std::string callersName,
                                               const std::string damageType,
                                               bool nanobots);
    virtual std::vector<std::pair<std::string, Damage>> getTotalDamageForEachPlayer(std::string callersName) const;
    const std::map<std::string, Damage>& getNanobotsDamagePerAffectedPlayer(std::string name) const;
    const std::map<std::string, Damage>& getRegularDamagePerAffectedPlayer(std::string name) const;

    virtual Heal getTotalHeals(std::string callersName);
    std::vector<std::pair<std::string, Heal>> getHealsForEachAffectedPlayer() const;

    Nano getTotalNano(std::string callersName) const;

    typedef typename std::vector<C>::iterator AffectedPlayerVectorIterator;
    virtual AffectedPlayerVectorIterator begin() {return players.begin();}
    virtual AffectedPlayerVectorIterator end() {return players.end();}
    typedef typename std::vector<C>::const_iterator const_AffectedPlayerVectorIterator;
    virtual const_AffectedPlayerVectorIterator begin() const {return players.begin();}
    virtual const_AffectedPlayerVectorIterator end() const {return players.end();}

private:
    void createPlayer(std::string name, LineInfo& lineInfo);
    static bool compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                               std::pair<std::string, Damage>& p2);

    std::vector<C> players;
};




template<class C>
AffectedPlayerVector<C>::~AffectedPlayerVector() {
    for (C player : players) {
        delete player;
    }
}

template<class C>
AffectedPlayerVector<C>::AffectedPlayerVector(const AffectedPlayerVector<C>& other) {
    for (C player : other.players) {
        typedef typename std::remove_pointer<C>::type CNoPointer;
        C p = new CNoPointer(*player);
        players.push_back(p);
    }
}

template<class C>
AffectedPlayerVector<C>::AffectedPlayerVector(AffectedPlayerVector<C>&& other) : AffectedPlayerVector<C>() {
    swap(*this, other);
}

template<class C>
AffectedPlayerVector<C>& AffectedPlayerVector<C>::operator=(AffectedPlayerVector<C> rhs) {
    swap(*this, rhs);
    return *this;
}

template<class T>
void swap(AffectedPlayerVector<T>& first, AffectedPlayerVector<T>& second) {
        std::swap(first.players, second.players);
}

template<class C>
Damage AffectedPlayerVector<C>::getTotalDamage(std::string callersName, bool nanobots) {
    Damage d;
    for (const C ap : players) {
        if (ap->getName() != callersName) {  // If not self
            d += ap->getTotalDamage(nanobots);
        }
    }
    return d;
}

template<class C>
Damage AffectedPlayerVector<C>::getTotalDamagePerDamageType(std::string callersName, const std::string damageType, bool nanobots) {
    Damage d;
    for (const C ap : players) {
        if (ap->getName() != callersName) {  // If not self
            d += ap->getTotalDamagePerDamageType(damageType, nanobots);
        }
    }
    return d;
}

template<class C>
std::vector<std::pair<std::string, Damage>> AffectedPlayerVector<C>::getTotalDamageForEachPlayer(std::string callersName) const {
    // Returns a sorted vector of pairs containing the players name and their
    // total damage (in the form of the Damage class).
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const C ap : players) {
        if (ap->getName() != callersName) {  // If not self
            totalDamagePerPlayer.push_back(
                std::make_pair(ap->getName(), ap->getTotalDamage()));
            }
    }
    std::sort(totalDamagePerPlayer.begin(),
              totalDamagePerPlayer.end(),
              compareTotalReceivedFromPlayer);
    return totalDamagePerPlayer;
}

template<class C>
bool AffectedPlayerVector<C>::compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                                          std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceived() >
           p2.second.getTotalReceived();
}

template<class C>
const std::map<std::string, Damage>& AffectedPlayerVector<C>::getNanobotsDamagePerAffectedPlayer(std::string name) const {
    for (const C ap : players) {
        if (ap->getName() == name) {
            return ap->getNanobotsDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players");
}

template<class C>
const std::map<std::string, Damage>& AffectedPlayerVector<C>::getRegularDamagePerAffectedPlayer(std::string name) const {
    for (const C ap : players) {
        if (ap->getName() == name) {
            return ap->getRegularDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players list");
}

template<class C>
Heal AffectedPlayerVector<C>::getTotalHeals(std::string callersName) {
    Heal h;
    for (const C ap : players) {
        if (ap->getName() != callersName) {
            h += ap->getHeal();
        }
    }
    return h;
}

template<class C>
std::vector<std::pair<std::string, Heal>> AffectedPlayerVector<C>::getHealsForEachAffectedPlayer() const {
    // TODO: This will include the owning player as well. Do I want that?
    std::vector<std::pair<std::string, Heal>> healsPerPlayer;
    for (const C ap : players) {
        healsPerPlayer.push_back(std::make_pair(ap->getName(), ap->getHeal()));
    }
    return healsPerPlayer;
}

template<class C>
Nano AffectedPlayerVector<C>::getTotalNano(std::string callersName) const {
    Nano n;
    for (const AffectedPlayer* ap : players) {
        if (ap->getName() != callersName) {
            n += ap->getNano();
        }
    }
    return n;
}

template<class C>
void AffectedPlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    for (AffectedPlayer* ap : players) {
        if (ap->getName() == lineInfo.dealer_name) {
            ap->add(lineInfo);
            dealerFound = true;
        }
        else if (ap->getName() == lineInfo.receiver_name) {
            ap->add(lineInfo);
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
void AffectedPlayerVector<C>::createPlayer(std::string name, LineInfo& lineInfo) {
    // Get the type pointed to by C
    typedef typename std::remove_pointer<C>::type CNoPointer;
    C p = new CNoPointer(name);
    p->add(lineInfo);
    players.push_back(p);
}

template<class C>
const C AffectedPlayerVector<C>::getPlayer(std::string name) {
    for (const C ap : players) {
        if (ap->getName() == name) {
            return ap;
        }
    }
    errorLog.write("Playerould not find a player with the name " + name);
    return nullptr;
}

template<class C>
int AffectedPlayerVector<C>::getLongestNameLength() const {
    unsigned int longestNameLength = 0;
    for (const C ap : players) {
        if (ap->getName().length() > longestNameLength) {
            longestNameLength = ap->getName().length();
        }
    }
    return longestNameLength;
}


#endif // AFFECTED_PLAYER_VECTOR_H
