#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>

class Player {

	// Player needs to store the last line, possible even further back in order to know if executed nano was
	// successfull and then store that info. Maybe it can store the last executed nano unless it's possible
	// to execute two nanos at once (with different cooldowns) and have second one execute first?

public:
	Player();
	Player(std::string name);
	~Player();
	Player& operator+=(const Player& p);
	
	struct nano_cast_info { // For each nano
		std::string name;
		int count = 0;
		int landed = 0;
		int resisted = 0;
		int countered = 0;
		int max = 0;
		int min = 0;
	} nano_casted;

	void add_nano_casted(nano_cast_info& nano);

	std::vector<std::string> last_nano_cast_line;  // remove
	std::string name;

	struct stats {
		// Include some info on deflects per special/normal etc?

		// temp stats just for testing
		std::string type;
		int amount = 0;

		// Damage types
		int energy_damage = 0;
		int energy_cnt = 0;
		int projectile_damage = 0;
		int projectile_cnt = 0;
		int nano_damage = 0;
		int nano_cnt = 0;
		int chemical_damage = 0;
		int chemical_cnt = 0;
		int radiation_damage = 0;
		int radiation_cnt = 0;
		int melee_damage = 0;
		int melee_cnt = 0;

		int hits_taken = 0;
		int misses = 0;  // Target
		int evasions = 0;  // Self
		int target_deflects = 0;
		int self_deflects = 0;


		// Specials:
		int aimed_shot_total = 0;
		int aimed_shot_cnt = 0;
		int aimed_shot_damage = 0;
		int aimed_shot_max = 0;
		int aimed_shot_min = 0;
		int aimed_shot_mean_time = 0;
		int backstab_total = 0;
		int backstab_cnt = 0;
		int backstab_damage = 0;
		int backstab_max = 0;
		int backstab_min = 0;
		int backstab_mean_time = 0;
		int brawling_total = 0;
		int brawling_cnt = 0;
		int brawling_misses = 0;
		int brawling_damage = 0;
		int brawling_max = 0;
		int brawling_min = 0;
		int brawling_mean_time = 0;
		int burst_total = 0;
		int burst_cnt = 0;
		int bust_misses = 0;
		int burst_damage = 0;
		int burst_max = 0;
		int burst_min = 0;
		int burst_mean_time = 0;
		int dimach_total = 0;
		int dimach_cnt = 0;
		int dimach_misses = 0;  // is this possible?
		int dimach_damage = 0;
		int dimach_max = 0;
		int dimach_min = 0;
		int dimach_mean_time = 0;
		int fling_shot_total = 0;
		int fling_shot_cnt = 0;
		int fling_shot_misses = 0;
		int fling_shot_damage = 0;
		int fling_shot_max = 0;
		int fling_shot_min = 0;
		int fling_shot_mean_time = 0;
		int fast_attack_total = 0;
		int fast_attack_cnt = 0;
		int fast_attack_misses = 0;
		int fast_attack_damage = 0;
		int fast_attack_max = 0;
		int fast_attack_min = 0;
		int fast_attack_mean_time = 0;
		int full_auto_total = 0;
		int full_auto_cnt = 0;
		//int full_auto_bullets_hit = 0;  // Not available, afaik. Or maybe it's not a damage log message.
		int full_auto_misses = 0;
		int full_auto_damage = 0;
		int full_auto_max = 0;
		int full_auto_min = 0;
		int full_auto_mean_time = 0;
		int sneak_attack_total = 0;
		int sneak_attack_cnt = 0;
		int sneak_attack_damage = 0;
		int sneak_attack_max = 0;
		int sneak_attack_min = 0;
		int sneak_attack_mean_time = 0;
		int bow_special_total = 0;  // Correct name?
		int bow_special_cnt = 0;
		int bow_special_misses = 0;  // Possible?
		int bow_special_damage = 0;
		int bow_special_max = 0;
		int bow_special_min = 0;
		int bow_special_mean_time = 0;

		// Crits
		int critical_total = 0;
		int critical_cnt = 0;
		int critical_max = 0;
		int critical_min = 0;
		int critical_hit_mean_time = 0;

		// Heal and nano
		int damage_taken = 0;
		int heal_given = 0;
		int heal_given_potential = 0;
		int heal_received = 0;
		int heal_received_potential = 0;
		int nano_given = 0;
		
		// XP
		int xp = 0;
		int xp_lost = 0;
		int axp = 0;
		int axp_lost = 0;
		int sk = 0;
		int sk_lost = 0;
		int research = 0;

		// Nano programs casted
		int nano_program_cast_cnt = 0; // Total count
		std::string last_nano_casted;
		std::vector<struct nano_cast_info> nanos_casted;

		// Nano damage from casting (nukes and dots unless I can separate them but excluding perks)
		int nano_damage_cast_total = 0;
		int nano_damage_cast_cnt = 0;
		int nano_damage_cast_damage_max = 0;
		int nano_damage_cast_damage_min = 0;

	} stats;

};


#endif
