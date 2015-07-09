#include "player_vector.h"
#include "line_info.h"
#include "logger.h"


void PlayerVector::addToPlayers(LogLine& logLine) {
    // Adds the info found in a log line to dealer and receiver.
    // If no player is found, it creates a new one.
    LineInfo& li = logLine.getInfo();
    bool dealerFound = false;
    bool receiverFound = false;

	for (Player& p : players) {
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

void PlayerVector::createPlayer(std::string name, LogLine& logLine) {
    Player p(name, logLine);
    players.push_back(p);
}

Player* PlayerVector::getPlayer(std::string name) {
    for (Player& p : players) {
        if (p.getName() == name) {
            return &p;
        }
    }
    errorLog.write("Could not find a player with the name " + name);
    return nullptr;
}

std::vector<Player>& PlayerVector::getPlayers() {
    return players;
}

unsigned int PlayerVector::getLongestNameLength() {
    unsigned int longestNameLength = 0;
    for (Player& p : players) {
        if (p.getName().length() > longestNameLength) {
            longestNameLength = p.getName().length();
        }
    }
    return longestNameLength;
}
