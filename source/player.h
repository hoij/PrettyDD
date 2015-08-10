#ifndef PLAYER_H
#define PLAYER_H


#include "affected_player.h"
#include "base_player.h"
#include "damage.h"
#include "heal.h"
#include "nano.h"
#include "nano_program.h"
#include "player_vector.h"
#include "xp.h"

#include <map>
#include <string>
#include <vector>


class LineInfo;

class Player : public BasePlayer {
public:
	Player(std::string name);

    void add(LineInfo& lineInfo);

    unsigned int getLongestAffectedPlayerNameLength() const;
    Damage getTotalDamage() const;
    Damage getTotalDamage(bool nanobots) const;
    Damage getTotalDamagePerDamageType(const std::string damageType);
    Damage getTotalDamagePerDamageType(const std::string damageType, bool nanobots);
    std::vector<std::pair<std::string, Damage>> getTotalDamagePerAffectedPlayer() const;

    Heal getTotalHeals();
    const std::map<std::string, Heal>& getHeals();
    Nano getTotalNano();
    const std::map<std::string, Nano>& getNano();

    const XP& getXp();
    const std::vector<NanoProgram>& getNanoPrograms();

    PlayerVector<AffectedPlayer>& getAffectedPlayers();

private:
    static bool compareTotalDealtToPlayer(std::pair<std::string, Damage>& p1,
                                          std::pair<std::string, Damage>& p2);
    static bool compareTotalReceivedFromPlayer(std::pair<std::string, Damage>& p1,
                                               std::pair<std::string, Damage>& p2);
    void addNanoProgram(std::string name, std::string subtype);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    PlayerVector<AffectedPlayer> affectedPlayers;

    std::string nameOfLastNanoProgramCasted;
	std::vector<NanoProgram> nanoPrograms;
    XP xp;
};


#endif
