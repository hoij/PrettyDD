#ifndef PLAYER_VECTOR_BASE_H
#define PLAYER_VECTOR_BASE_H

#include "damage.h"
#include "line_info.h"
#include "logger.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

class Heal;
class LineInfo;

template<class C>
class BaseVector {
public:
    BaseVector() {}
    virtual ~BaseVector();
    BaseVector(const BaseVector<C>& player);
    BaseVector(BaseVector<C>&& other);
    BaseVector& operator=(BaseVector<C> rhs);
    template<class T>
    friend void swap(BaseVector<T>& first, BaseVector<T>& second);

    virtual void addToPlayers(LineInfo& lineInfo);
    virtual size_t getLongestNameLength() const;
    virtual const C getPlayer(std::string name);

    typedef typename std::vector<C>::iterator PlayerVectorIterator;
    virtual PlayerVectorIterator begin() {return players.begin();}
    virtual PlayerVectorIterator end() {return players.end();}
    typedef typename std::vector<C>::const_iterator const_PlayerVectorIterator;
    virtual const_PlayerVectorIterator begin() const {return players.begin();}
    virtual const_PlayerVectorIterator end() const {return players.end();}

protected:
    void createPlayer(std::string name, LineInfo& lineInfo);
    static bool compareTotalReceivedFromPlayer(const std::pair<std::string, Damage>& p1,
                                               const std::pair<std::string, Damage>& p2);
    std::vector<C> players;
};

template<class C>
BaseVector<C>::~BaseVector() {
    for (C player : players) {
        delete player;
    }
}

template<class C>
BaseVector<C>::BaseVector(const BaseVector<C>& other) {
    for (C player : other.players) {
        typedef typename std::remove_pointer<C>::type CNoPointer;
        C p = new CNoPointer(*player);
        players.push_back(p);
    }
}

template<class C>
BaseVector<C>::BaseVector(BaseVector<C>&& other) : BaseVector<C>() {
    swap(*this, other);
}

template<class C>
BaseVector<C>& BaseVector<C>::operator=(BaseVector<C> rhs) {
    swap(*this, rhs);
    return *this;
}

template<class T>
void swap(BaseVector<T>& first, BaseVector<T>& second) {
        std::swap(first.players, second.players);
}


template<class C>
void BaseVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    for (C player : players) {
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
        createPlayer(lineInfo.dealer_name, lineInfo);
    }
    if(!receiverFound && lineInfo.receiver_name != "") {
        createPlayer(lineInfo.receiver_name, lineInfo);
    }
}

template<class C>
void BaseVector<C>::createPlayer(std::string name, LineInfo& lineInfo) {
    // Get the type pointed to by C
    typedef typename std::remove_pointer<C>::type CNoPointer;
    C player = new CNoPointer(name);
    player->add(lineInfo);
    players.push_back(player);
}

template<class C>
const C BaseVector<C>::getPlayer(std::string name) {
    for (const C player : players) {
        if (player->getName() == name) {
            return player;
        }
    }
    errorLog.write("Could not find a player with the name " + name);
    return nullptr;
}

template<class C>
size_t BaseVector<C>::getLongestNameLength() const {
    size_t longestNameLength = 0;
    for (const C player : players) {
        if (player->getName().length() > longestNameLength) {
            longestNameLength = player->getName().length();
        }
    }
    return longestNameLength;
}

template<class C>
bool BaseVector<C>::compareTotalReceivedFromPlayer(const std::pair<std::string, Damage>& p1,
                                                   const std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceived() >
           p2.second.getTotalReceived();
}


#endif // PLAYER_VECTOR_BASE_H
