#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <map>
#include "base_player.h"
#include "affected_player.h"
#include "line_info.h"
#include "log_line.h"
#include "player_vector.h"
#include "heal.h"
#include "damage.h"
#include "nano_program.h"
#include "xp.h"
#include "nano.h"


class Player : public BasePlayer {
public:
	Player();
	Player(std::string name, LogLine& logLine);
	~Player();
	Player(const Player& p);
	Player& operator=(const Player& p);

    void add(LogLine& logLine);

    Damage getTotalDamage();
    Damage getTotalDamage(bool nanobots);
    Damage getTotalDamagePerDamageType(const std::string damageType);
    Damage getTotalDamagePerDamageType(const std::string damageType, bool nanobots);
    std::vector<std::pair<std::string, Damage>> getTotalDamagePerAffectedPlayer() const;

    Heal getTotalHeals();
    const std::map<std::string, Heal>& getHeals();
    Nano getTotalNano();
    const std::map<std::string, Nano>& getNano();

    const XP& getXp();
    const std::vector<NanoProgram>& getNanoPrograms();

    unsigned int getLongestAffectedPlayerNameLength() const;

    PlayerVector<AffectedPlayer>& getAffectedPlayers();

private:
    static bool compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
                                          std::pair<std::string, Damage>& p2);
    static bool compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                               std::pair<std::string, Damage>& p2);
    void addNanoProgram(NanoProgram& nanoProgram);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    PlayerVector<AffectedPlayer> affectedPlayers;

    NanoProgram last_nano_casted;
	std::vector<NanoProgram> nanoPrograms;

    XP xp;
};


#endif
