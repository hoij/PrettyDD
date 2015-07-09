#include "write_stats_to_file.h"

#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "damage.h"
#include "logger.h"


// Will probably have to switch spaces to tabs where possible to save bytes
// as AO can only handle 1 kB scripts.

void writeDamageOverview(PlayerVector& pv) {
    std::ofstream file("damage_overview");
    if (file.is_open()) {

        unsigned int longestNameLength;
        for (Player& p : pv.getPlayers()) {
            if (p.getName().length() > longestNameLength) {
                longestNameLength = p.getName().length();
            }
        }

        // Write headlines here
        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.getPlayers().begin(), pv.getPlayers().end(), compareTotalReceived);
        for (Player& p : pv.getPlayers()) {
            file << std::left << std::setw(longestNameLength) << p.getName();
            p.getTotalDamage().writeDamageReceivedFromYou(file);
        }
    }
    else {
        // Write error
    }
}

void writeDamageReceivedOverview(PlayerVector& pv) {
    std::string fileName = "damage_received_overview";
    std::ofstream file(fileName);
    if (file.is_open()) {
        int longestNameLength = pv.getLongestNameLength();
        // Write headlines
        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.getPlayers().begin(), pv.getPlayers().end(), compareTotalDealt);
        for (Player& p : pv.getPlayers()) {
            file << std::left << std::setw(longestNameLength) << p.getName();
            p.getTotalDamage().writeDamageDealtToYou(file);
        }
    }
    else {
        errorLog.write("Could not open" + fileName);
    }
}

bool compareTotalReceived(Player& p1, Player& p2) {
    return p1.getTotalDamage().getTotalReceived() >
           p2.getTotalDamage().getTotalReceived();
}

bool compareTotalDealt(Player& p1, Player& p2) {
    return p1.getTotalDamage().getTotalDealt() >
           p2.getTotalDamage().getTotalDealt();
}

void writeDamageOverviewPerOpponent(PlayerVector& pv, std::string& playerName) {
    // This should be done in Player by a method that summarises the damage
    // and returns a vector of Players or somthing ready to print.
    std::string fileName = "damage_overview_per_opponent_for_" + playerName;
    std::ofstream file(fileName);
    if (file.is_open()) {
        const Player* pp = pv.getPlayer(playerName);
        if (pp) {
            unsigned int longestNameLength = pp->getLongestNameLengthInDamage();
            file << std::left << std::setw(longestNameLength) <<
            "Name" << Damage::writeHeadings << std::endl;

            for (auto& player : pp->getDamageMap()) {
                Damage d;
                file << std::left << std::setw(longestNameLength) << player.first;
                for (auto& damageType : player.second) {
                    d += damageType.second.at(true) + damageType.second.at(false);
                }
                d.writeDamageReceivedFromYou(file);
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
