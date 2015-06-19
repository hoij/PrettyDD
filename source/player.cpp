#include "player.h"

Player::Player() {}

Player::Player(std::string name) : name(name) {}

Player::Player(std::string name, LineInfo& li) : name(name) {
    add(li);
}

Player::~Player(){}

Player& Player::operator+=(const Player& p) {
	// Add stuff from p to this
	//damage_dealt = p.damage_dealt;
	//damage_received = p.damage_received;
	//heal_dealt = p.heal_dealt;
    //heal_received = p.heal_received;
    last_nano_casted = p.last_nano_casted;

	// etc...
	return *this;
}

void Player::add(LineInfo& li) {
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
    damage[li.receiver_name][li.subtype].add(li, "receiver");
    // Instead of adding the dealer damage here, it could be calculated
    // when presenting the stats?
    // Still need to add to the dealer when something hits "You". In order to
    // track who dealt the hit.
    damage[li.dealer_name][li.subtype].add(li, "dealer");
}

void Player::addNanoProgram(NanoProgram& np) {
    // Loop through nanos_casted and check if it has already been added
    for (auto& npInVec : nanoPrograms) {
        if (npInVec.getName() == np.getName()) {
            npInVec += np;
            return;
        }
    }
    nanoPrograms.push_back(np);
}
