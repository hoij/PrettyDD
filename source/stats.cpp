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
    // Because in that case, the nano will not be mentioned by name.
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
    damage[li.receiver_name][li.subtype][li.nanobots].add(li, "receiver");
    // Instead of adding the dealer damage here, it could be calculated
    // when presenting the stats?
    // Still need to add to the dealer when something hits "You". In order to
    // track who dealt the hit.
    damage[li.dealer_name][li.subtype][li.nanobots].add(li, "dealer");
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

Damage Stats::getTotalDamage() {
    return getTotalRegularDamage() + getTotalNanobotsDamage();
}

Damage Stats::sumDamage(bool nanobots) {
    Damage d;
    for (auto& player : damage) {
        for (auto& type : player.second) {
            d += type.second[nanobots];
        }
    }
    return d;
}

Damage Stats::getTotalRegularDamage() {
    return sumDamage(false);
}

Damage Stats::getTotalNanobotsDamage() {
    return sumDamage(true);
}

Damage Stats::getTotalPerDamageType(const std::string damageType) {
    return getTotalRegularPerDamageType(damageType, false) +
           getTotalNanobotsPerDamageType(damageType, true);
}

Damage Stats::sumDamageType(const std::string damageType, bool nanobots) {
    // Will need to ingore "You" or the sum will be wrong.
    // Best if I can stop it from being added completely?
    Damage d;
    for (auto& player : damage) {
        d += player.second[damageType][nanobots];
    }
    return d;
}

Damage Stats::getTotalRegularPerDamageType(const std::string damageType, bool nanobots) {
    return sumDamageType(damageType, nanobots);
}

Damage Stats::getTotalNanobotsPerDamageType(const std::string damageType, bool nanobots) {
    return sumDamageType(damageType, nanobots);
}

const std::map<std::string, std::map<std::string, std::map<bool, Damage>>>& Stats::getDamage() {
    return damage;
}

Heal Stats::getTotalHeals() {
    Heal h;
    for (auto& player : heals) {
        h += player.second;
    }
    return h;
}

const std::map<std::string, Heal>& Stats::getHeals() {
    return heals;
}

const std::vector<NanoProgram>& Stats::getNanoPrograms() {
    return nanoPrograms;
}

const XP& Stats::getXp() {
    return xp;
}

Nano Stats::getTotalNano() {
    Nano n;
    for (auto& player : nano) {
        n += player.second;
    }
    return n;
}

const std::map<std::string, Nano>& Stats::getNano() {
    return nano;
}
