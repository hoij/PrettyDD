#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <map>
#include "line_info.h"
#include "log_line.h"
#include "player_vector.h"
#include "heal.h"
#include "damage.h"
#include "nano_program.h"
#include "xp.h"
#include "nano.h"


class Player {
public:
	Player();
	Player(std::string name);  // Is this used? Remove otherwise
	Player(std::string name, LogLine& logLine);
	~Player();
	Player(const Player& p);
	Player& operator=(const Player& p);
	//Player& operator+=(const Player& p);

    std::string& getName();
    void setName(std::string name);
    void add(LogLine& logLine);

    //Stats stats;

    // Unsure of return type. Maybe use the
    // damageInfo struct in Damage or (if needed) create another class (or struct)
    // that inherits damageInfo and exends on it.
    Damage getTotalDamage();
    Damage getTotalRegularDamage();
    Damage getTotalNanobotsDamage();
    Damage getTotalPerDamageType(const std::string damageType);
    Damage getTotalRegularPerDamageType(const std::string damageType, bool nanobots);
    Damage getTotalNanobotsPerDamageType(const std::string damageType, bool nanobots);

    // Return the whole map of damage done.
    // Unfortunately necessary for the file writing class to iterate over all
    // players that the current player has affected.
    const std::map<std::string, std::map<std::string, std::map<bool, Damage>>>& getDamageMap() const;

    unsigned int getLongestNameLengthInDamage() const;

    Heal getTotalHeals();
    const std::map<std::string, Heal>& getHeals();

    // Return NanoProgram info or just a reference to the vector.
    const std::vector<NanoProgram>& getNanoPrograms();

    const XP& getXp();

    Nano getTotalNano();
    const std::map<std::string, Nano>& getNano();

private:
    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNanoProgram(NanoProgram& nanoProgram);
    void addXp(LineInfo& li);
    Damage sumDamage(bool nanobots);
    Damage sumDamageType(const std::string damageType, bool nanobots);

    NanoProgram last_nano_casted;
	std::vector<NanoProgram> nanoPrograms;

    // Damage info per target.
    //       player                damage type                 stats
    std::map<std::string, std::map<std::string, std::map<bool, Damage>>> damage;
    //                                                   false = regular
    //                                                   true = nanobots
    std::map<std::string, Heal> heals; // Heal info per target.
    std::map<std::string, Nano> nano;  // Nano info per target.
    XP xp;
    std::string name;
};


#endif
