#ifndef WRITE_STATS_TO_FILE_H
#define WRITE_STATS_TO_FILE_H


#include <string>
#include "player_vector.h"
#include "player.h"


void writeDamageOverview(PlayerVector& pv);
void writeDamageReceivedOverview(PlayerVector& pv);
void writeDamageOverviewPerOpponent(PlayerVector& pv, std::string& playerName);
void writeDamageDetailsPerDamageType(PlayerVector& pv, std::string& playerName);
void writeHealOverview();
void writeHealDetails(std::string& playerName);
void writeNanoProgramOverview();
void writeNanoProgramDetails();
void writeNanoOverview();
void writeNanoDetails();

bool compareTotalReceived(Player& p1, Player& p2);
bool compareTotalDealt(Player& p1, Player& p2);


#endif  // WRITE_STATS_TO_FILE_H
