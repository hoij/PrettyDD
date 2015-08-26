#ifndef WRITE_STATS_TO_FILE_H
#define WRITE_STATS_TO_FILE_H


#include "player_vector.h"

#include <string>


class Player;

void writeDamageDealtOverviewUnsorted(PlayerVector<Player*>& pv);
void writeDamageDealtOverview(PlayerVector<Player*>& pv);
void writeDamageReceivedOverview(PlayerVector<Player*>& pv);
void writeDamageDealtPerOpponent(const Player* pp);
void writeDamageDealtPerDamageType(Player&);
void writeHealOverview();
void writeHealDetails(Player& p);
void writeNanoProgramOverview();
void writeNanoProgramDetails();
void writeNanoOverview();
void writeNanoDetails();

bool compareTotalReceivedFromPlayer(const Player* p1, const Player* p2);
bool compareTotalDealtToPlayer(const Player* p1, const Player* p2);


#endif  // WRITE_STATS_TO_FILE_H
