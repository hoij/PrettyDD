#include "damage.h"
#include "line_info.h"
#include "player_vector.h"


/* Could take a reference to the factory but needs pointer
in order for tests to work */
PlayerVector::PlayerVector(
    std::string playerRunningProgram,
    std::unique_ptr<PlayerFactoryInterface> playerFactory) :
    playerRunningProgram(playerRunningProgram),
    playerFactory(std::move(playerFactory)) {}

void PlayerVector::addToPlayers(LineInfo& lineInfo) {
    /* Adds the info found in a log line to dealer and receiver.
    If a player with the same name is not found, a new one is created. */
    bool dealerFound = false;
    bool receiverFound = false;

    if (!log) {  // Don't log when stopped.
        return;
    }

    for (const auto& player : players) {
        if (player->getName() == lineInfo.dealer_name) {
            player->add(lineInfo);
            dealerFound = true;
        }
        else if (player->getName() == lineInfo.receiver_name) {
            player->add(lineInfo);
            receiverFound = true;
        }
    }
    if (!dealerFound && lineInfo.dealer_name != "") {
        createPlayer(lineInfo.dealer_name, lineInfo);
    }
    if (!receiverFound && lineInfo.receiver_name != "") {
        createPlayer(lineInfo.receiver_name, lineInfo);
    }
}

void PlayerVector::createPlayer(std::string name, LineInfo& lineInfo) {
    std::unique_ptr<PlayerInterface> player = playerFactory->createPlayer(name);
    player->add(lineInfo);
    players.push_back(std::move(player));
}

PlayerInterface* PlayerVector::getPlayer(std::string name) {
    // Finds the player and returns it as a raw pointer.
    // Reason for using a raw pointer is that the receiving end
    // should not take ownership.
    for (const auto& player : players) {
        if (player->getName() == name) {
            return player.get();
        }
    }
    return nullptr;
}

size_t PlayerVector::getLongestNameLength() const {
    size_t longestNameLength = 0;
    for (const auto& player : players) {
        if (player->getName().length() > longestNameLength) {
            longestNameLength = player->getName().length();
        }
    }
    return longestNameLength;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getTotalDamageDealtPerPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage dealt (in the form of the Damage class). Players
    with an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const auto& p : players) {
        Damage d = p->getTotalDamageDealt();
        if (d.getCount() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getTotalDamageReceivedPerPlayer() const {
    /* Returns a vector of pairs containing the players name and their
    total damage received (in the form of the Damage class). Players
    with an empty Damage are not included. */
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const auto& p : players) {
        Damage d = p->getTotalDamageReceived();
        if (d.getCount() != 0) {
            totalDamagePerPlayer.emplace_back(p->getName(), d);
        }
    }
    return totalDamagePerPlayer;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getDamageDealtPerType(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const auto& p : players) {
        if (p->getName() == pName) {
            return p->getTotalDamageDealtPerType();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getDamageReceivedPerType(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const auto& p : players) {
        if (p->getName() == pName) {
            return p->getTotalDamageReceivedPerType();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getDamageDealtPerType(std::string playerName,
std::string opponentName) const {
    /* There is no DR version of this method as the same result
    can be achieved by swapping player and opponent when calling
    this method. */
    std::string pName = renameIfSelf(playerName);
    std::string oppName = renameIfSelf(opponentName);
    for (const auto& p : players) {
        if (p->getName() == pName) {
            return p->getDamageDealtPerType(oppName);
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getDamageDealtPerOpponent(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const auto& p : players) {
        if (p->getName() == pName) {
            return p->getTotalDamageDealtPerAffectedPlayer();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

std::vector<std::pair<std::string, Damage>>
PlayerVector::getDamageReceivedPerOpponent(std::string playerName) const {
    std::string pName = renameIfSelf(playerName);
    for (const auto& p : players) {
        if (p->getName() == pName) {
            return p->getTotalDamageReceivedPerAffectedPlayer();
        }
    }
    // Return an empty vector if the player was not found.
    std::vector<std::pair<std::string, Damage>> v;
    return v;
}

std::string PlayerVector::renameIfSelf(std::string name) const {
    return (name == playerRunningProgram ? "You" : name);
}

void PlayerVector::stopLogging() {
    if (log) { // Only stop if logging is ongoing
        log = false;
        for (const auto& player : players) {
            player->stopTimer();
        }
    }
}

void PlayerVector::startLogging() {
    if (!log) { // Only start if logging has been stopped
        log = true;
        for (const auto& player : players) {
            if (player->getStartTime() != 0) {
                player->resumeTimer();
            }
        }
    }
}

void PlayerVector::reset() {
    players.clear();
    log = true;
}
