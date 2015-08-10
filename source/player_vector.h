/* Class is templated to hold different versions of Players */

#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include "line_info.h"
#include "logger.h"

#include <iterator>
#include <string>
#include <vector>


template<class C>
class PlayerVector {
public:
    void addToPlayers(LineInfo& lineInfo);
    const C* getPlayer(std::string name);
    unsigned int getLongestNameLength() const;

    typedef typename std::vector<C>::iterator PlayerVectorIterator;
    PlayerVectorIterator begin() {return players.begin();}
    PlayerVectorIterator end() {return players.end();}
    typedef typename std::vector<C>::const_iterator const_PlayerVectorIterator;
    const_PlayerVectorIterator begin() const {return players.begin();}
    const_PlayerVectorIterator end() const {return players.end();}

private:
    void createPlayer(std::string name, LineInfo& lineInfo);

    std::vector<C> players;
};


template<class C>
void PlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    for (C& p : players) {
        if (p.getName() == lineInfo.dealer_name) {
            p.add(lineInfo);
            dealerFound = true;
        }
        else if (p.getName() == lineInfo.receiver_name) {
            p.add(lineInfo);
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
    //C p(name, lineInfo);
    C p(name);
    p.add(lineInfo);
    players.push_back(p);
}

template<class C>
const C* PlayerVector<C>::getPlayer(std::string name) {
    for (C& p : players) {
        if (p.getName() == name) {
            return &p;
        }
    }
    errorLog.write("Could not find a player with the name " + name);
    return nullptr;
}

template<class C>
unsigned int PlayerVector<C>::getLongestNameLength() const {
    unsigned int longestNameLength = 0;
    for (const C& p : players) {
        if (p.getName().length() > longestNameLength) {
            longestNameLength = p.getName().length();
        }
    }
    return longestNameLength;
}


#endif // PLAYER_VECTOR_H
