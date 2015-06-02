#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>

class Player {

	// Player needs to store the last line, possible even further back in order to know if executed nano was
	// successfull and store that info.

public:
	Player::Player();
	Player(std::string name);
	~Player();

	std::vector<std::string> last_nano_cast_line;

	std::string name;
	struct stats {
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

		int misses = 0;
		int evasions = 0;
		int hits_taken = 0;

		// Specials:
		int aimed_shot = 0;
		int aimed_shot_cnt = 0;
		int aimed_shot_damage = 0;
		int aimed_shot_mean_time = 0;
		int backstab = 0;
		int backstab_cnt = 0;
		int backstab_damage = 0;
		int backstab_mean_time = 0;
		int brawling = 0;
		int brawling_cnt = 0;
		int brawling_misses = 0;
		int brawling_damage = 0;
		int brawling_mean_time = 0;
		int burst = 0;
		int burst_cnt = 0;
		int bust_misses = 0;
		int burst_damage = 0;
		int burst_mean_time = 0;
		int dimach = 0;
		int dimach_cnt = 0;
		int dimach_misses = 0; // is this possible?
		int dimach_damage = 0;
		int dimach_mean_time = 0;
		int fling_shot = 0;
		int fling_shot_cnt = 0;
		int fling_shot_misses = 0;
		int fling_shot_damage = 0;
		int fling_shot_mean_time = 0;
		int fast_attack = 0;
		int fast_attack_cnt = 0;
		int fast_attack_misses = 0;
		int fast_attack_damage = 0;
		int fast_attack_mean_time = 0;
		int full_auto = 0;
		int full_auto_cnt = 0;
		int full_auto_misses = 0; // Does full auto also give the nr of bullets that hit?
		int full_auto_damage = 0;
		int full_auto_mean_time = 0;
		int sneak_attack = 0;
		int sneak_attack_cnt = 0;
		int sneak_attack_damage = 0;
		int sneak_attack_mean_time = 0;
		int bow_special = 0; // Correct name?
		int bow_special_cnt = 0;
		int bow_special_misses = 0; // Possible?
		int bow_special_damage = 0;
		int bow_special_mean_time = 0;

		// Crits
		int critical_hits = 0;
		int critical_damage = 0;
		int critical_hit_mean_time = 0;

		// Heal and nano
		int dmg_taken = 0;
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
		int nano_cast_cnt = 0; // Total count
		struct nano_cast_info { // For each nano
			std::string nano;
			int count = 0;
		} nano_cast_info;
		std::vector<struct nano_cast_info> nanos_casted;
	} stats;

private:

};


#endif