#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include <iterator>
#include <vector>
#include "log_line.h"
#include "line_info.h"
#include "logger.h"


template<class C>
class PlayerVector {
public:
    PlayerVector() {}
    ~PlayerVector() {}
    PlayerVector<C>& operator=(C& pv);

    void createPlayer(std::string name, LogLine& logLine);
    const C* getPlayer(std::string name);
    std::vector<C>& getPlayers();
    void removePlayer();
    void addToPlayers(LogLine& logLine);
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
void PlayerVector<C>::addToPlayers(LogLine& logLine) {
    // Adds the info found in a log line to dealer and receiver.
    // If no player is found a new one is created.
    LineInfo& li = logLine.getInfo();
    bool dealerFound = false;
    bool receiverFound = false;

	for (C& p : players) {
		if (p.getName() == li.dealer_name) {
			p.add(logLine);
			dealerFound = true;
		}
		else if (p.getName() == li.receiver_name) {
			p.add(logLine);
			receiverFound = true;
		}
	}
	if(!dealerFound && li.dealer_name != "") {
        createPlayer(li.dealer_name, logLine);
    }
	if(!receiverFound && li.receiver_name != "") {
        createPlayer(li.receiver_name, logLine);
    }

    // For development purposes only
    // Just to capture anything I might have missed.
    if (li.dealer_name == "" && li.receiver_name == "") {
        errorLog.write("Could not find dealer and receiver name in the following line (Note: This may be normal): ");
        errorLog.write(logLine.getOriginalLine());
    }
    else if (li.dealer_name == "") {
        errorLog.write("Could not find dealer name in the following line (Note: This may be normal): ");
        errorLog.write(logLine.getOriginalLine());
    }
    else if (li.receiver_name == "") {
        errorLog.write("Could not find receiver name in the following line (Note: This may be normal): ");
        errorLog.write(logLine.getOriginalLine());
    }
}

template<class C>
void PlayerVector<C>::createPlayer(std::string name, LogLine& logLine) {
    C p(name, logLine);
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
