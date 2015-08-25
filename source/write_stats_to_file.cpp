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

void writeDamageDealtOverviewUnsorted(PlayerVector<Player*>& pv) {
    std::ofstream file("damage_dealt_overview_unsorted");
    if (file.is_open()) {
        size_t longestNameLength = pv.getLongestNameLength();

        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        //std::sort(pv.begin(), pv.end(), compareTotalReceivedFromPlayer);
        for (const Player* p : pv) {
            file << std::left << std::setw(longestNameLength) << p->getName();
            p->getTotalDamage().writeDamageReceivedFromPlayer(file);
        }
    }
    else {
        // Write error
    }
}

void writeDamageDealtOverview(PlayerVector<Player*>& pv) {
    std::ofstream file("damage_dealt_overview");
    if (file.is_open()) {
        size_t longestNameLength = pv.getLongestNameLength();

        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.begin(), pv.end(), compareTotalReceivedFromPlayer);
        for (const Player* p : pv) {
            file << std::left << std::setw(longestNameLength) << p->getName();
            p->getTotalDamage().writeDamageReceivedFromPlayer(file);
        }
    }
    else {
        // Write error
    }
}

void writeDamageReceivedOverview(PlayerVector<Player*>& pv) {
    std::string fileName = "damage_received_overview";
    std::ofstream file(fileName);
    if (file.is_open()) {
        size_t longestNameLength = pv.getLongestNameLength();

        file << std::left << std::setw(longestNameLength) <<
                "Name" << Damage::writeHeadings << std::endl;

        std::sort(pv.begin(), pv.end(), compareTotalDealtToPlayer);
        for (const Player* p : pv) {
            file << std::left << std::setw(longestNameLength) << p->getName();
            p->getTotalDamage().writeDamageDealtToPlayer(file);
        }
    }
    else {
        errorLog.write("Could not open" + fileName);
    }
}

bool compareTotalReceivedFromPlayer(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalReceived() >
           p2->getTotalDamage().getTotalReceived();
}

bool compareTotalDealtToPlayer(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalDealt() >
           p2->getTotalDamage().getTotalDealt();
}

void writeDamageDealtPerOpponent(const Player* pp) {
    if (pp != NULL) {
        std::string fileName = "damage_dealt_per_opponent_for_" + pp->getName();
        std::ofstream file(fileName);
        if (file.is_open()) {
            size_t longestNameLength = pp->getLongestAffectedPlayerNameLength();
            file << std::left << std::setw(longestNameLength) <<
            "Name" << Damage::writeHeadings << std::endl;

            for (auto& pair : pp->getTotalDamageForEachAffectedPlayer()) {
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
