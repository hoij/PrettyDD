#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "affected_player.h"
#include "affected_player_vector.h"

#include <algorithm>
#include <utility>


Damage AffectedPlayerVector::getTotalDamage(std::string callersName, bool nanobots) {
    Damage d;
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() != callersName) {  // If not self
            d += ap.getTotalDamage(nanobots);
        }
    }
    return d;
}

Damage AffectedPlayerVector::getTotalDamagePerDamageType(std::string callersName, const std::string damageType, bool nanobots) {
    Damage d;
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() != callersName) {  // If not self
            d += ap.getTotalDamagePerDamageType(damageType, nanobots);
        }
    }
    return d;
}

std::vector<std::pair<std::string, Damage>> AffectedPlayerVector::getTotalDamageForEachPlayer(std::string callersName) const {
    // Returns a sorted vector of pairs containing the players name and their
    // total damage (in the form of the Damage class).
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer;
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() != callersName) {  // If not self
            totalDamagePerPlayer.push_back(
                std::make_pair(ap.getName(), ap.getTotalDamage()));
            }
    }
    std::sort(totalDamagePerPlayer.begin(),
              totalDamagePerPlayer.end(),
              compareTotalReceivedFromPlayer);
    return totalDamagePerPlayer;
}

bool AffectedPlayerVector::compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                                          std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceived() >
           p2.second.getTotalReceived();
}

const std::map<std::string, Damage>& AffectedPlayerVector::getNanobotsDamagePerAffectedPlayer(std::string name) const {
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() == name) {
            return ap.getNanobotsDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players");
}

const std::map<std::string, Damage>& AffectedPlayerVector::getRegularDamagePerAffectedPlayer(std::string name) const {
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() == name) {
            return ap.getRegularDamage();
        }
    }
    // TODO: Catch and log this error somwhere.
    throw std::invalid_argument("\"" + name + "\" was not found in the " +
                                "affected players list");
}

Heal AffectedPlayerVector::getTotalHeals(std::string callersName) {
    Heal h;
    for (const AffectedPlayer& ap : players) {
        if (ap.getName() != callersName) {
            h += ap.getHeal();
        }
    }
    return h;
}

std::vector<std::pair<std::string, Heal>> AffectedPlayerVector::getHealsForEachAffectedPlayer() const {
    std::vector<std::pair<std::string, Heal>> healsPerPlayer;
    for (const AffectedPlayer& ap : players) {
        healsPerPlayer.push_back(std::make_pair(ap.getName(), ap.getHeal()));
    }
    return healsPerPlayer;
}

void AffectedPlayerVector::addToPlayers(LineInfo& lineInfo) {
    // Adds the info found in a log line to dealer and receiver.
    // If a player with the same name is not found, a new one is created.
    bool dealerFound = false;
    bool receiverFound = false;

    for (AffectedPlayer& ap : players) {
        if (ap.getName() == lineInfo.dealer_name) {
            ap.add(lineInfo);
            dealerFound = true;
        }
        else if (ap.getName() == lineInfo.receiver_name) {
            ap.add(lineInfo);
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

void AffectedPlayerVector::createPlayer(std::string name, LineInfo& lineInfo) {
    //Player p(name, lineInfo);
    AffectedPlayer ap(name);
    ap.add(lineInfo);
    players.push_back(ap);
}

const AffectedPlayer* AffectedPlayerVector::getPlayer(std::string name) {
    for (AffectedPlayer& ap : players) {
        if (ap.getName() == name) {
            return &ap;
        }
    }
    errorLog.write("Playerould not find a player with the name " + name);
    return nullptr;
}

int AffectedPlayerVector::getLongestNameLength() const {
    unsigned int longestNameLength = 0;
    for (const AffectedPlayer& ap : players) {
        if (ap.getName().length() > longestNameLength) {
            longestNameLength = ap.getName().length();
        }
    }
    return longestNameLength;
}

