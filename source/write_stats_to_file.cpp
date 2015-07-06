#include "write_stats_to_file.h"

#include <fstream>
#include <vector>
#include "damage.h"

void writeDamageOverview(PlayerVector& pv) {
    std::ofstream file("damage_overview");
    if (file.is_open()) {

        // Write headlines here
        file << "Name" << std::endl;

        for (Player& player : pv.getPlayers()) {
            Damage d = player.stats.getTotalDamage();
            file << player.get_name() << " " << d;
        }
    }
    else {
        // Write error
    }


}

