#include "player.h"

Player::Player() {
}

Player::Player(std::string name) : name(name) {
}

Player::Player(std::string name, LogLine& logLine) : name(name) {
    add(logLine);
}

Player::~Player(){
}

Player::Player(const Player& p) {
    name = p.name;
    last_nano_casted = p.last_nano_casted;
    nanoPrograms = p.nanoPrograms;
    damage = p.damage;
    heals = p.heals;
    xp = p.xp;
}

Player& Player::operator=(const Player& p) {
    if (this != &p) {
        name = p.name;
        //stats = p.stats;
        last_nano_casted = p.last_nano_casted;
        nanoPrograms = p.nanoPrograms;
        damage = p.damage;
        heals = p.heals;
        xp = p.xp;
    }
    return *this;
}

//Player& Player::operator+=(const Player& p) {
//    stats += p.stats;
//	return *this;
//}

std::string& Player::getName() {
    return name;
}

void Player::setName(std::string name) {
    this->name = name;
}

void Player::add(LogLine& logLine) {
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

void Player::addXp(LineInfo& li) {
    xp.add(li);
}

void Player::addHeal(LineInfo& li) {
    // There will alywas be a receiver.
    heals[li.receiver_name].add(li, "receiver");
    if (li.subtype == "potential") {
        // But only a dealer in case the subtype is "potential".
        heals[li.dealer_name].add(li, "dealer");
    }
}

void Player::addDamage(LineInfo& li) {
    // Only add the stats to the other player and never to the player
    // owning the stats. This is to avoid the player having himself as a
    // player in his damage list.

    ////////////////
    // Consequently it won't be possible to log self damage.
    // But can that be detected anyway? Test in AO.
    ////////////////

    if (li.receiver_name != getName()) {
        damage[li.receiver_name][li.subtype][li.nanobots].add(li, "receiver");
    }
    if (li.dealer_name != getName()) {
        damage[li.dealer_name][li.subtype][li.nanobots].add(li, "dealer");
    }
}

void Player::addNanoProgram(NanoProgram& np) {
    // Adds a nano program if it hasn't already been added.
    for (auto& npInVec : nanoPrograms) {
        if (npInVec.getName() == np.getName()) {
            npInVec += np;
            return;
        }
    }
    nanoPrograms.push_back(np);
}

Damage Player::getTotalDamage() {
    return getTotalRegularDamage() + getTotalNanobotsDamage();
}

Damage Player::sumDamage(bool nanobots) {
    Damage d;
    // Need to ignore self i.e. the player in damage that has the same name as the player
    // that has damage. Nice. Or stop it form being added in the first place.

    for (auto& player : damage) {
        for (auto& type : player.second) {
            d += type.second[nanobots];
        }
    }
    return d;
}

Damage Player::getTotalRegularDamage() {
    return sumDamage(false);
}

Damage Player::getTotalNanobotsDamage() {
    return sumDamage(true);
}

Damage Player::getTotalPerDamageType(const std::string damageType) {
    return getTotalRegularPerDamageType(damageType, false) +
           getTotalNanobotsPerDamageType(damageType, true);
}

Damage Player::sumDamageType(const std::string damageType, bool nanobots) {
    // Will need to ingore "You" or the sum will be wrong.
    // Best if I can stop it from being added completely?
    Damage d;
    for (auto& player : damage) {
        d += player.second[damageType][nanobots];
    }
    return d;
}

Damage Player::getTotalRegularPerDamageType(const std::string damageType, bool nanobots) {
    return sumDamageType(damageType, nanobots);
}

Damage Player::getTotalNanobotsPerDamageType(const std::string damageType, bool nanobots) {
    return sumDamageType(damageType, nanobots);
}

const std::map<std::string, std::map<std::string, std::map<bool, Damage>>>& Player::getDamageMap() const {
    return damage;
}

unsigned int Player::getLongestNameLengthInDamage() const {
    unsigned int longestNameLength = 0;
    for (auto& player : damage) {
        if (player.first.length() > longestNameLength) {
            longestNameLength = player.first.length();
        }
    }
    return longestNameLength;
}

Heal Player::getTotalHeals() {
    Heal h;
    for (auto& player : heals) {
        h += player.second;
    }
    return h;
}

const std::map<std::string, Heal>& Player::getHeals() {
    return heals;
}

const std::vector<NanoProgram>& Player::getNanoPrograms() {
    return nanoPrograms;
}

const XP& Player::getXp() {
    return xp;
}

Nano Player::getTotalNano() {
    Nano n;
    for (auto& player : nano) {
        n += player.second;
    }
    return n;
}

const std::map<std::string, Nano>& Player::getNano() {
    return nano;
}
