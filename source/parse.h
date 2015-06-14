#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include "player.h"


int parse(std::string line, std::vector<Player>& players);

std::vector<std::string>& split(std::string& s, char delim, std::vector<std::string>& words);

void add_to_players(std::vector<Player>& players, Player::line_info& li);

void add_to_temp_players(Player& p, const std::string& type, const int amount);

int find_player(std::vector<Player>& players, Player& temp_player, Player* real_player);

int find_amount(const std::vector<std::string>& words);

std::string find_subtype(const std::vector<std::string>& words);

int find_values(const std::vector<std::string>& words, Player::line_info& li);

void cleanup(std::vector<std::string>& words);


#endif
