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
    stats = p.stats;
}

Player& Player::operator=(Player& p) {
    name = p.name;
    stats = p.stats;
    return *this;
}

//Player& Player::operator+=(const Player& p) {
//    stats += p.stats;
//	return *this;
//}

std::string& Player::get_name() {
    return name;
}

void Player::set_name(std::string name) {
    name = name;
}

void Player::add(LogLine& logLine) {
    stats.add(logLine);
}
