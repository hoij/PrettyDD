#include "write_stats_to_file.h"

#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "affected_player.h"
#include "damage.h"
#include "logger.h"


// Will probably have to switch spaces to tabs where possible to save bytes
// as AO can only handle 1 kB scripts.

void writeDamageDealtOverview(PlayerVector<Player>& pv) {
    std::ofstream file("damage_dealt_overview");
    if (file.is_open()) {
        unsigned int longestNameLength;
        for (const Player& p : pv.getPlayers()) {
            if (p.getName().length() > longestNameLength) {
                longestNameLength = p.getName().length();
            }
        }

        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.getPlayers().begin(), pv.getPlayers().end(), compareTotalReceivedFromPlayer);
        for (Player& p : pv.getPlayers()) {
            file << std::left << std::setw(longestNameLength) << p.getName();
            p.getTotalDamage().writeDamageReceivedFromPlayer(file);
        }
    }
    else {
        // Write error
    }
}

void writeDamageReceivedOverview(PlayerVector<Player>& pv) {
    std::string fileName = "damage_received_overview";
    std::ofstream file(fileName);
    if (file.is_open()) {
        int longestNameLength = pv.getLongestNameLength();

        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.getPlayers().begin(), pv.getPlayers().end(), compareTotalDealtToPlayer);
        for (Player& p : pv.getPlayers()) {
            file << std::left << std::setw(longestNameLength) << p.getName();
            p.getTotalDamage().writeDamageDealtToPlayer(file);
        }
    }
    else {
        errorLog.write("Could not open" + fileName);
    }
}

bool compareTotalReceivedFromPlayer(Player& p1, Player& p2) {
    return p1.getTotalDamage().getTotalReceived() >
           p2.getTotalDamage().getTotalReceived();
}

bool compareTotalDealtToPlayer(Player& p1, Player& p2) {
    return p1.getTotalDamage().getTotalDealt() >
           p2.getTotalDamage().getTotalDealt();
}

void writeDamageDealtPerOpponent(const Player* pp) {
    if (pp != NULL) {
        std::string fileName = "damage_dealt_per_opponent_for_" + pp->getName();
        std::ofstream file(fileName);
        if (file.is_open()) {
            unsigned int longestNameLength = pp->getLongestAffectedPlayerNameLength();
            file << std::left << std::setw(longestNameLength) <<
            "Name" << Damage::writeHeadings << std::endl;

            for (auto& pair : pp->getTotalDamagePerAffectedPlayer()) {
                file << std::left << std::setw(longestNameLength) << pair.first;
                pair.second.writeDamageReceivedFromPlayer(file);
            }
        }
    }
}


//void writeDamageDetailsPerDamageType(PlayerVector& pv, std::string& playerName) {
//
//    std::string fileName = "damage_per_damage_type_for_" + playerName;
//    std::ofstream file(fileName);
//    if (file.is_open()) {
//        Player* pp = pv.getPlayer(playerName);
//        if (pp) {
//            for (auto& player : pp->getDamageMap()) {
//                for (auto& damageType : player.second) {
//                    file << player.first;
//                    Damage d = damageType.second.at(true) + damageType.second.at(false);
//                    d.writeDamageReceivedFromYou(file);
//                }
//            }
//        }
//    }
//}
