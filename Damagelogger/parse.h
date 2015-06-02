#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include "player.h"


int parse(std::vector<std::string> &previous_words, std::vector<std::string> &words, std::vector<Player> &players);

std::vector<std::string>& split(std::string &s, char delim, std::vector<std::string> &words);

int find_player(std::vector<Player> &players, Player &temp_player, Player *real_player);

int find_amount(const std::vector<std::string> &words, Player &dealer, Player &receiver);

int find_type(const std::vector<std::string> &words, Player &dealer, Player &receiver);

int find_values(const std::vector<std::string> &previous_words, const std::vector<std::string> &words, Player &dealer, Player &receiver);

void cleanup(std::vector<std::string> &words);


#endif