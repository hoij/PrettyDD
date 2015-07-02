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


class Stats {
public:
    Stats();
    ~Stats();
    Stats& operator=(const Stats& s);

    void add(LogLine& logLine);

private:
    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNanoProgram(NanoProgram& nanoProgram);
    void addXp(LineInfo& li);

    NanoProgram last_nano_casted;
	std::vector<NanoProgram> nanoPrograms;

    // Damage info per target.
    //       player                damage type  stats
    std::map<std::string, std::map<std::string, Damage>> damage;

    // Heal info per target.
    std::map<std::string, Heal> heals;

    // XP info
    XP xp;

};


#endif  // STATS_H
