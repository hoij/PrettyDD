#include "player_manager.h"
#include "line_info.h"
#include "logger.h"


void PlayerManager::addToPlayers(LogLine& logLine) {
    // Adds the info found in a log line to dealer and receiver.
    // If no player is found, it creates a new one.
    LineInfo& li = logLine.getInfo();
    bool dealerFound = false;
    bool receiverFound = false;

	for (Player& p : players) {
		if (p.get_name() == li.dealer_name) {
			p.add(logLine);
			dealerFound = true;
		}
		else if (p.get_name() == li.receiver_name) {
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
    if (li.dealer_name == "" || li.receiver_name == "") {
        errorLog.write("Could not find dealer or receiver name in the following line (Note: This may be normal): ");
        errorLog.write(logLine.getOriginalLine());
    }
}

void PlayerManager::createPlayer(std::string name, LogLine& logLine) {
    Player p(name, logLine);
    players.push_back(p);
}

Player* PlayerManager::getPlayer(std::string name) {
    for (Player& p : players) {
        if (p.get_name() == name) {
            return &p;
        }
    }
    return nullptr;
}
