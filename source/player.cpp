#include "player.h"

Player::Player() {}

Player::Player(std::string name) : name(name) {}

Player::Player(LineInfo& li, std::string player_type) {
    if (player_type == "dealer")
        name = li.dealer_name;
    else if (player_type == "receiver")
        name = li.receiver_name;
    add(li, player_type);
}

Player::~Player(){}

Player& Player::operator+=(const Player& p) {
	// Add stuff from p to this
	damage_dealt = p.damage_dealt;
    last_nano_casted = p.last_nano_casted;

	// etc...
	return *this;
}

void Player::add(LineInfo& li, std::string player_type) {
    if (li.type == "damage") {
        if (player_type == "dealer") {
            add_damage_dealt(li);
        }
        else if (player_type == "receiver") {
            add_damage_received(li);
        }
    }
    else if (li.type == "heal") {

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
    else if (li.type == "") {

    }

}

void Player::add_damage_dealt(LineInfo& li) {
    damage_dealt[li.subtype].total += li.amount;
    damage_dealt[li.subtype].count++;
    if (!li.crit) {
        if (li.amount > damage_dealt[li.subtype].regular_max) {
            damage_dealt[li.subtype].regular_max = li.amount;
        }
        if (li.amount < damage_dealt[li.subtype].regular_min) {
            damage_dealt[li.subtype].regular_min = li.amount;
        }
        else if (damage_dealt[li.subtype].regular_min == -1 &&
                 li.amount != 0) {
            damage_dealt[li.subtype].regular_min = li.amount;
        }
        if (li.deflect)
            damage_dealt[li.subtype].deflects++;
        if (li.miss)
            damage_dealt[li.subtype].misses++;
    }
    else {
        damage_dealt[li.subtype].crit_count++;
        damage_dealt[li.subtype].crit_total += li.amount;
        if (li.amount > damage_dealt[li.subtype].crit_max) {
            damage_dealt[li.subtype].crit_max = li.amount;
        }
        if (li.amount < damage_dealt[li.subtype].crit_min) {
            damage_dealt[li.subtype].crit_min = li.amount;
        }
        else if (damage_dealt[li.subtype].crit_min == -1) {
            damage_dealt[li.subtype].crit_min = li.amount;
        }
    }
}

void Player::add_damage_received(LineInfo& li) {
    damage_received[li.subtype].total += li.amount;
    damage_received[li.subtype].count++;
    if (!li.crit) {
        if (li.amount > damage_received[li.subtype].regular_max) {
            damage_received[li.subtype].regular_max = li.amount;
        }
        if (li.amount < damage_received[li.subtype].regular_min) {
            damage_received[li.subtype].regular_min = li.amount;
        }
        else if (damage_received[li.subtype].regular_min == -1) {
            damage_received[li.subtype].regular_min = li.amount;
        }
        if (li.deflect)
            damage_received[li.subtype].deflects++;
        if (li.miss)
            damage_received[li.subtype].misses++;
    }
    else {
        damage_received[li.subtype].crit_count++;
        damage_received[li.subtype].crit_total += li.amount;
        if (li.amount > damage_received[li.subtype].crit_max) {
            damage_received[li.subtype].crit_max = li.amount;
        }
        if (li.amount < damage_received[li.subtype].crit_min) {
            damage_received[li.subtype].crit_min = li.amount;
        }
        else if (damage_received[li.subtype].crit_min == -1) {
            damage_received[li.subtype].crit_min = li.amount;
        }
    }

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
