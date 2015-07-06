#ifndef WRITE_STATS_TO_FILE_H
#define WRITE_STATS_TO_FILE_H


#include <string>
#include "player_vector.h"


void writeDamageOverview(PlayerVector& pv);
void writeDamageDetails(std::string playerName);
void writeHealOverview();
void writeHealDetails(std::string playerName);
void writeNanoProgramOverview();
void writeNanoProgramDetails();
void writeNanoOverview();
void writeNanoDetails();


#endif  // WRITE_STATS_TO_FILE_H
