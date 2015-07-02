#include "stats.h"

Stats::Stats() {}
Stats::~Stats() {}

Stats& Stats::operator=(const Stats& s) {
    // Not necessary in this class as there are no pointers?
    if (this != &s) {
        last_nano_casted = s.last_nano_casted;
        nanoPrograms = s.nanoPrograms;
        damage = s.damage;
        heals = s.heals;
        xp = s.xp;
    }
    return *this;
}

void Stats::add(LogLine& logLine) {
    LineInfo& li = logLine.getInfo();
    if (li.type == "damage") {
        addDamage(li);
    }
    else if (li.type == "heal") {
        addHeal(li);
    }
    else if (li.type == "nano_cast") {
    // Only add the nano when a message about the success/fail has arrived.
    // In that case the nano will not be mentioned by name.
        if (li.nanoProgram->getName() != "") {
            last_nano_casted = *li.nanoProgram;
        }
        else {
            // If nano casted successfully/resisted/countered/aborted/fumble/interrupt?
            last_nano_casted.addStat(li.subtype, 1);
            addNanoProgram(last_nano_casted);
        }
    }
    else if (li.type == "xp") {
        addXp(li);
    }
}

void Stats::addXp(LineInfo& li) {
    xp.add(li);
}

void Stats::addHeal(LineInfo& li) {
    // There will alywas be a receiver.
    heals[li.receiver_name].add(li, "receiver");
    if (li.subtype == "potential") {
        // But only a dealer in case the subtype is "potential".
        heals[li.dealer_name].add(li, "dealer");
    }
}

void Stats::addDamage(LineInfo& li) {
    damage[li.receiver_name][li.subtype].add(li, "receiver");
    // Instead of adding the dealer damage here, it could be calculated
    // when presenting the stats?
    // Still need to add to the dealer when something hits "You". In order to
    // track who dealt the hit.
    damage[li.dealer_name][li.subtype].add(li, "dealer");
}

void Stats::addNanoProgram(NanoProgram& np) {
    // Loop through nanos_casted and check if it has already been added
    for (auto& npInVec : nanoPrograms) {
        if (npInVec.getName() == np.getName()) {
            npInVec += np;
            return;
        }
    }
    nanoPrograms.push_back(np);
}
