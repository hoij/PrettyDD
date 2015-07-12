#ifndef WRITE_STATS_TO_FILE_H
#define WRITE_STATS_TO_FILE_H


#include <string>
#include "player_vector.h"
#include "player.h"


void writeDamageDealtOverview(PlayerVector<Player>& pv);
void writeDamageReceivedOverview(PlayerVector<Player>& pv);
void writeDamageDealtPerOpponent(const Player* pp);
void writeDamageDealtPerDamageType(Player&);
void writeHealOverview();
void writeHealDetails(Player& p);
void writeNanoProgramOverview();
void writeNanoProgramDetails();
void writeNanoOverview();
void writeNanoDetails();

bool compareTotalReceivedFromPlayer(Player& p1, Player& p2);
bool compareTotalDealtToPlayer(Player& p1, Player& p2);


#endif  // WRITE_STATS_TO_FILE_H
