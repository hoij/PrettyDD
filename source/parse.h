#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include "player.h"
#include "line_info.h"

bool isCrit(const std::vector<std::string>& splitLine);

bool isDeflect(const std::vector<std::string>& splitLine);

LogLine parse(std::string line);

int find_amount(LogLine& logLine);

std::string find_subtype(const std::vector<std::string>& splitLine);

void renameSpecial(LineInfo& li);

int find_values(LogLine& logLine);


#endif
