#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <map>

class Player {

	// Player needs to store the last line, possible even further back in order to know if executed nano was
	// successfull and then store that info. Maybe it can store the last executed nano unless it's possible
	// to execute two nanos at once (with different cooldowns) and have second one execute first?

public:
	struct nano_cast_info { // For each nano
		std::string name;
		int count = 0;
		int landed = 0;
		int resisted = 0;
		int countered = 0;
		int max = 0;
		int min = 0;
	};

    struct line_info {
        std::string dealer_name;
        std::string receiver_name;
        std::string type;
        std::string subtype;
        int amount = 0;
        bool crit = false;
        bool deflect = false;
        bool miss = false;
        int heal_received_potential = 0;
        int heal_given_potential = 0;
        nano_cast_info nanos;
    };

	struct damage_type {
        damage_type() {}

        int total = 0;
        int count = 0;
        int regular_max = 0;
        int regular_min = -1;
        int crit_total = 0;
		int crit_count = 0;
		int crit_max = 0;
		int crit_min = -1;
        int deflects = 0;
        int misses = 0;

        // Not used at the moment:
        int mean_time = 0;
    };

	Player();
	Player(std::string name);
	Player(line_info& li, std::string player_type);
	~Player();
	Player& operator+=(const Player& p);

    std::string& get_name() {return name;}
    void set_name(std::string name) {name = name;}

    void add(line_info& li, std::string player_type);

    void add_damage_dealt(line_info& li);
    void add_damage_received(line_info& li);
    void add_nano_casted(nano_cast_info& nano);

    // Write functions for:
	// add_damage_dealt()
	// add_damage_received()
	// add_heal_given()
	// add_heal_received()
	//
	// Good idea?

    // Temp, remove
    int get_stats(std::string type) {return stats[type];}
    std::string type;

private:
    std::string name;
    std::string last_nano_casted;

	std::vector<struct nano_cast_info> nanos_casted;

    std::map<std::string, damage_type> damage_dealt; /* = {

        //Why even initialize?
        //What happens if i retreive a nonexisting key?

        {"energy", damage_type()},
		{"projectile", damage_type()},
		{"nano", damage_type()},
		{"chemical", damage_type()},
		{"poison", damage_type()},
		{"radiation", damage_type()},
		{"aimed_shot", damage_type()},
		{"backstab", damage_type()},
		{"Brawling", damage_type()},
		{"Burst", damage_type()},
		{"Dimach", damage_type()},
		{"Fling Shot", damage_type()},
		{"fast_attack", damage_type()},
		{"full_auto", damage_type()},
		{"sneak_attack", damage_type()},
		{"bow_special", damage_type()},
        {"melee_damage", damage_type()}
    };
    */
    std::map<std::string, damage_type> damage_received; /*= {
        {"energy_damage", damage_type()},
		{"projectile_damage", damage_type()},
		{"nano_damage", damage_type()},
		{"chemical_damage", damage_type()},
		{"radiation_damage", damage_type()},
		{"aimed_shot", damage_type()},
		{"backstab", damage_type()},
		{"brawling", damage_type()},
		{"burst", damage_type()},
		{"dimach", damage_type()},
		{"fling_shot", damage_type()},
		{"fast_attack", damage_type()},
		{"full_auto", damage_type()},
		{"sneak_attack", damage_type()},
		{"bow_special", damage_type()},
        {"melee_damage", damage_type()}
    };
*/
    std::map<std::string, int> xp = {
		{"xp", 0},
		{"xp_lost", 0},
		{"axp", 0},
		{"axp_lost", 0},
		{"sk", 0},
		{"sk_lost", 0},
		{"research", 0}
    };

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
        {"amount", 0}, // Temp, remove

		// Crits
		{"critical_total", 0},
		{"critical_cnt", 0},
		{"critical_max", 0},
		{"critical_min", 0},
		{"critical_hit_mean_time", 0},

		// Heal and nano
		{"damage_taken", 0},
		{"heal_given", 0},
		{"heal_given_potential", 0},
		{"heal_received", 0},
		{"heal_received_potential", 0},
		{"nano_given", 0},

		// Nano programs casted
		{"nano_program_cast_cnt", 0}, // Total count

		// Nano damage from casting (nukes and dots unless I can separate them but excluding perks)
		{"nano_damage_cast_total", 0},
		{"nano_damage_cast_cnt", 0},
		{"nano_damage_cast_damage_max", 0},
		{"nano_damage_cast_damage_min", 0}};
};


#endif
