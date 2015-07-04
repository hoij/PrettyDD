#ifndef STATS_H
#define STATS_H

#include <string>
#include <map>
#include <vector>
#include "line_info.h"
#include "log_line.h"
#include "nano_program.h"
#include "heal.h"
#include "damage.h"
#include "xp.h"
#include "nano.h"


class Stats {
public:
    Stats();
    ~Stats();
    Stats& operator=(const Stats& s);

    void add(LogLine& logLine);

    ///////////////////////////////////////////////////////////////////////
    // Get the data from the stats.
    // Any processing may be done in another class or not.
    ///////////////////////////////////////////////////////////////////////

    // Return overall damage done. Unsure of return type. Maybe use the
    // damageInfo struct in Damage or (if needed) create another class (or struct)
    // that inherits damageInfo and exends on it.
    Damage getTotalDamage();
    Damage getTotalRegularDamage();
    Damage getTotalNanobotsDamage();

    // Return one specific damage type.
    Damage getTotalPerDamageType(const std::string damageType);
    Damage getTotalRegularPerDamageType(const std::string damageType, bool nanobots);
    Damage getTotalNanobotsPerDamageType(const std::string damageType, bool nanobots);

    // Return the whole map of damage done.
    // Unfortunately necessary for the file writing class to iterate over all
    // players that the current player has affected.
    const std::map<std::string, std::map<std::string, std::map<bool, Damage>>>& getDamage();

    // Return overall heals done. Unsure of return type.
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
    //       player                damage type           reg/nanobots  stats
    std::map<std::string, std::map<std::string, std::map<bool, Damage>>> damage;
    //                                                   false = regular
    //                                                   true = nanobots

    // Heal info per target.
    std::map<std::string, Heal> heals;

    // XP info
    XP xp;

    std::map<std::string, Nano> nano;

};


#endif  // STATS_H
