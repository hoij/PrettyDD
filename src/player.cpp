#include "player.h"

Player::Player() {}

Player::Player(std::string name) : name(name) {}

Player::~Player(){}

Player& Player::operator+=(const Player& p) {
	// Add stuff from p to this
	return *this;
}


void Player::add_nano_casted(nano_cast_info& nano) {
	// Loop through nanos_casted and check if it has already been added
	for (auto& nano_in_vec : stats.nanos_casted) {
		if (nano_in_vec.name == nano.name)
			nano_in_vec.count += 1;
	}
}
