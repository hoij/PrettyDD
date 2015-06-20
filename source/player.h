#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <map>
#include <limits>
#include "line_info.h"
#include "nano_program.h"
#include "heal.h"
#include "damage.h"
#include "xp.h"

class Player {
public:
	Player();
	Player(std::string name);
	Player(std::string name, LineInfo& li);
	~Player();
	Player& operator+=(const Player& p);

    std::string& get_name() {return name;}
    void set_name(std::string name) {name = name;}
    void add(LineInfo& li);

private:
    void addDamage(LineInfo& li);
    void addHeal(LineInfo& li);
    void addNanoProgram(NanoProgram& nanoProgram);
    void addXp(LineInfo& li);

    std::string name;

    NanoProgram last_nano_casted;
	std::vector<NanoProgram> nanoPrograms;

    //////////
    // I could have one vector of players for each player with all
    // data related to that player.
    //////////

    // Damage info per target.
    //       player                damage type  stats
    std::map<std::string, std::map<std::string, Damage>> damage;

    // Heal info per target.
    std::map<std::string, Heal> heals;

    // XP info
    XP xp;


    std::map<std::string, int> overall = {
        {"critical_total", 0},
		{"critical_cnt", 0},
		{"critical_max", 0},
		{"critical_min", 0},
		{"critical_hit_mean_time", 0},
		{"hits", 0},
		{"hits_received", 0}
    };

    // Make separate maps for these stats?
	std::map <std::string, int> stats = {
        // Nano
        {"nano_given", 0},
        {"nano_received", 0},

		// Nano programs casted
		{"nano_program_cast_cnt", 0}, // Total count

		// Nano damage from casting (nukes and dots unless I can separate them but excluding perks)
		{"nano_damage_cast_total", 0},
		{"nano_damage_cast_cnt", 0},
		{"nano_damage_cast_damage_max", 0},
		{"nano_damage_cast_damage_min", 0}};
};


#endif
