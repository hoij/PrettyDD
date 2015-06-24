#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include "player.h"
#include "line_info.h"

bool isCrit(const std::vector<std::string>& splitLine);

bool isDeflect(const std::vector<std::string>& splitLine);

int parse(std::string line, std::vector<Player>& players);

void add_to_players(std::vector<Player>& players, LogLine& logLine);

void add_to_temp_players(Player& p, const std::string& type, const int amount);

int find_player(std::vector<Player>& players, Player& temp_player, Player* real_player);

int find_amount(LogLine& logLine);

std::string find_subtype(const std::vector<std::string>& splitLine);

void renameSpecial(LineInfo& li);

int find_values(LogLine& logLine);


#endif
