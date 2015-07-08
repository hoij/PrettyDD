#include "write_stats_to_file.h"

#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "damage.h"


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
            p.stats.getTotalDamage().writeDamageReceivedFromYou(file);
        }
    }
    else {
        // Write error
    }
}

void writeDamageReceivedOverview(PlayerVector& pv) {
    std::ofstream file("damage_received_overview");
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

        std::sort(pv.getPlayers().begin(), pv.getPlayers().end(), compareTotalDealt);
        for (Player& p : pv.getPlayers()) {
            file << std::left << std::setw(longestNameLength) << p.getName();
            p.stats.getTotalDamage().writeDamageDealtToYou(file);
        }
    }
    else {
        // Write error
    }
}

bool compareTotalReceived(Player& p1, Player& p2) {
    return p1.stats.getTotalDamage().getTotalReceived() >
           p2.stats.getTotalDamage().getTotalReceived();
}

bool compareTotalDealt(Player& p1, Player& p2) {
    return p1.stats.getTotalDamage().getTotalDealt() >
           p2.stats.getTotalDamage().getTotalDealt();
}
