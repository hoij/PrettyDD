#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include <iterator>
#include <vector>
#include "formatted_line.h"
#include "line_info.h"
#include "logger.h"


template<class C>
class PlayerVector {
public:
    PlayerVector() {}
    ~PlayerVector() {}
    PlayerVector<C>& operator=(C& pv);

    void createPlayer(std::string name, LineInfo& lineInfo);
    const C* getPlayer(std::string name);
    std::vector<C>& getPlayers();
    void removePlayer();
    void addToPlayers(LineInfo& lineInfo);
    unsigned int getLongestNameLength() const;

    typedef typename std::vector<C>::iterator PlayerVectorIterator;
    typedef typename std::vector<C>::const_iterator const_PlayerVectorIterator;
    PlayerVectorIterator begin() {return players.begin();}
    PlayerVectorIterator end() {return players.end();}
    const_PlayerVectorIterator begin() const {return players.begin();}
    const_PlayerVectorIterator end() const {return players.end();}

private:
    std::vector<C> players;
};


template<class C>
PlayerVector<C>& PlayerVector<C>::operator=(C& pv) {
    if (this != &pv) {
        for (C& p : pv.players) {
            players.push_back(p);
        }
    }
    return *this;
}

template<class C>
void PlayerVector<C>::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If no player is found a new one is created.
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
    C p(name, lineInfo);
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
std::vector<C>& PlayerVector<C>::getPlayers() {
    return players;
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
