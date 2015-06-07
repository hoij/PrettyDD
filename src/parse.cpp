#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include "parse.h"
#include "player.h"

using namespace std;

// TODO:
// Find critical damage
// After some perk actions there is a line telling which perk was performed
//	and if it was successfull or not. Can use that to get stats on perks.

int parse(std::string line, vector<Player>& players) {
	Player dealer;
	Player receiver;

	// Split the line and store it in words
	vector<string> words;
	split(line, ',', words);

	if (!words.size() == 0) {
		cleanup(words);

		//for (auto it = words.begin(); it != words.end(); it++)
		//	cout << *it << endl;

		// find name of dealer, receiver, damage (or heal) done, and type of damage
		find_values(words, dealer, receiver);

		// Debug prints
		cout << "Dealer: " << dealer.name << "\t" << "Amount: " <<
			dealer.stats.amount << "\t" << "Type: " <<
			dealer.stats.type << endl;
		cout << "Receiver: " << receiver.name << "\t" << "Amount: " <<
			receiver.stats.amount << "\t" << "Type: " <<
			receiver.stats.type << endl << endl;

		add_to_player(players, dealer);
		add_to_player(players, receiver);
	}
	return 0;
}

void add_to_player(std::vector<Player>& players, Player& player) {
	bool found = false;
	for (auto& p : players) {
		if (p.name == player.name) {
			found = true;
			p += player;
		}
	}
	if (!found) {
		players.push_back(player);
	}
}

int find_amount(const std::vector<std::string>& words, Player& dealer, Player& receiver) {
	std::smatch d;
	if (regex_search(words[4], d, regex("(?:for )(\\d+)"))) {
		dealer.stats.amount = std::stoi(d[1]);
		receiver.stats.amount = std::stoi(d[1]);
	}
	return 0;
}

int find_type(const std::vector<std::string>& words, Player& dealer, Player& receiver) {
	// Used to find damage type. Heals handled separately.
	std::smatch t;
	if (!regex_search(words[4], t, regex("(?:points of )(.*?)(?= damage)"))) // Looks for regular and special damage
		regex_search(words[4], t, regex("(?: )(.*?)(?= hit)")); // Looks for reflect or shield damage
			// Since I know when it's been a heal I'll just set it under "Me got health" etc instead of looking for it here.
			//regex_search(words[4], t, regex("(healed)(?= )")) {// looks for "healed" followed by " ".
	dealer.stats.type = t[1];
	receiver.stats.type = t[1];
	return 0;
}

int find_values(const vector<string>& words, Player& dealer, Player& receiver) {
	// Skriv hur varje löses, merga de som är liknande?
	std::smatch m;

	if (words[1] == "Other hit by other" || words[1] == "Your pet hit by other") {
		// ["#000000004200000a#","Other hit by other","",1425326282]Sheila Marlene hit Predator Rogue for 461 points of melee damage.
		// ["#000000004200000a#","Other hit by other","",1425326285]Sgtcuddle hit Predator Rogue for 1434 points of energy damage.
		// ["#000000004200000a#","Other hit by other","",1425326287]Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.
		// ["#000000004200000a#","Other hit by other","",1425326293]Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.
		// ["#000000004200000a#","Other hit by other","",1425997610]Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!
		// ["#0000000042000009#","Your pet hit by other","",1425996728]Letter hit Guard for 623 points of melee damage.
		// ["#0000000042000009#","Your pet hit by other","",1425996758]Letter hit Guard for 1268 points of melee damage.Critical hit!
		// ["#0000000042000009#","Your pet hit by other","",1425996734]Guard hit Letter for 1639 points of melee damage.Critical hit!
		regex_search(words[4], m, regex("(.*?)(?= hit)"));
		dealer.name = m[0];
		regex_search(words[4], m, regex("hit\\s"		// Find "hit " then
										"(.*?)"			// match everything following, non-greedy
														// i.e. until first occurrence, of
										"(?= for)"));	// " for"
		receiver.name = m[1];
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Other hit by nano") {
		// ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
		// ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
		if (!regex_search(words[4], m, regex("from\\s(.*?)(?= for)")))
			regex_search(words[4], m, regex("of\\s(.*?)(?= damage)"));
		dealer.name = m[1];
		regex_search(words[4], m, regex("(.*?)(?= was)"));
		receiver.name = m[0];
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "You hit other") {
		/*
		["#0000000042000008#","You hit other","",1425326282]You hit Predator Rogue for 2357 points of poison damage.
		["#0000000042000008#","You hit other","",1425326284]You hit Predator Rogue for 2329 points of chemical damage.
		["#0000000042000008#","You hit other","",1425326288]You hit Predator Rogue for 2357 points of Fast Attack damage.
		["#0000000042000008#","You hit other","",1425326293]You hit Predator Rogue for 1942 points of Brawling damage.
		["#0000000042000008#","You hit other","",1425326291]You hit Predator Rogue for 3911 points of poison damage.Critical hit!
		["#0000000042000008#","You hit other","",1425326284]Your reflect shield hit Predator Rogue for 49 points of damage.
		["#0000000042000008#","You hit other","",1425326284]Your damage shield hit Predator Rogue for 163 points of damage.
		*/
		dealer.name = "You";
		regex_search(words[4], m, regex("hit\\s(.*?)(?= for)"));
		receiver.name = m[1];
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "You hit other with nano") {
		// ["#0000000042000005#","You hit other with nano","",1425993792]You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.
		dealer.name = "You";
		regex_search(words[4], m, regex("hit\\s(.*?)(?= with)"));
		receiver.name = m[1];
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
		// Need to save that it was nanobot damage. Or do I? Is nanobots álways == perks?
	}
	else if (words[1] == "Me got health"){
		/*
		["#0000000042000015#","Me got health","",1425326282]You were healed for 193 points.
		["#0000000042000015#","Me got health","",1425995902]You got healed by Starphoenix for 3359 points of health.
		["#0000000042000015#","Me got health","",1425995902]You were healed for 3359 points.
		["#0000000042000015#","Me got health","",1425996008]You got healed by Starphoenix for 3173 points of health.
		["#0000000042000015#","Me got health","",1425996008]You were healed for 1949 points.
		*/
		std::smatch h;
		receiver.name = "You";
		receiver.stats.type = "heal";
		regex_search(words[4], h, regex("(?:for\\s)(\\d*)"));
		receiver.stats.amount = std::stoi(h[1]);
		if (regex_search(words[4], m, regex("\\sgot(?= healed)"))) { // Potential heal
			regex_search(words[4], m, regex("by\\s(.*?)(?= for)"));
			dealer.name = m[1];
			receiver.stats.heal_received_potential += stoi(h[1]);
			dealer.stats.heal_given_potential += stoi(h[1]);
			dealer.stats.type = "heal";
		}
		else { // "were" healed and the amount corresponds to the actual amount healed.
			receiver.stats.heal_received += stoi(h[1]);
			// if the previous line was a heal from someone, can I count on the "were healed" here coming from that person?
			// Or is it possible that a heal delta tick or someone elses heal gets inbetween?
			//if (last line for this player was a heal from someone)
			//	dealer.stats.heal_given += stoi(h[1]);
		}
	}
	else if (words[1] == "You gave health") {
		// ["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
		dealer.name = "You";
		regex_search(words[4], m, regex("healed\\s(.*?)(?= for)"));
		receiver.name = m[1];
		find_amount(words, dealer, receiver);
		dealer.stats.type = "heal";
		receiver.stats.type = "heal";
	}
	else if (words[1] == "Me hit by monster") {
		// ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
		// ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
		regex_search(words[4], m, regex("(.*?)(?= hit)"));  // Match as few chars as possible untill " hit".
		dealer.name = m[0];
		receiver.name = "You";
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Me hit by nano") {
		// ["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
		regex_search(words[4], m, regex("from\\s(.*?)(?= for)"));
		dealer.name = m[1];
		receiver.name = "You";
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Me hit by player") {
		receiver.name = "You";
		// need to get example lines
		// need to find player name
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Me hit by monster") {
		// ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
		// ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
		receiver.name = "You";
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Other misses") {
		// There is an extra comma in this line. Might mess something up.
		// ["#0000000042000013#","Other misses","",1425326282]Predator Rogue tried to hit you, but missed!
		regex_search(words[4], m, regex("(.*?)(?= tried)"));
		dealer.name = m[0];
		regex_search(words[4], m, regex("(?:hit\\s)(.*)"));
		if (m[1] == "you")
			receiver.name = "You";
		else
			receiver.name = m[1];
		dealer.stats.type = "miss";
		dealer.stats.misses += 1;
		receiver.stats.type = "evasion";
		receiver.stats.evasions += 1;
	}
	else if (words[1] == "Your misses") {
		// ["#0000000042000012#", "Your misses", "", 1425666157]You try to attack Peal Thunder with Brawl, but you miss!
		// ["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
		dealer.name = "You";
		regex_search(words[4], m, regex("attack\\s(.*?)(?= with)"));
		receiver.name = m[1];
		dealer.stats.type = "miss";
		dealer.stats.misses += 1;
		receiver.stats.type = "evasion";
		receiver.stats.evasions += 1;
	}
	else if (words[1] == "Me hit by environment") {
		// need to get example line
		receiver.name = "You";
		find_type(words, dealer, receiver);
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Me Cast Nano") {
		// ["#0000000042000018#","Me Cast Nano","",1425326300]Executing Nano Program: Improved Mongo Crush!.
		// ["#0000000042000018#","Me Cast Nano","",1425326300]Nano program executed successfully.
		// ["#0000000042000018#","Me Cast Nano","",1425997797]Executing Nano Program: Thorough Overhaul.
		// ["#0000000042000014#","You gave health","",1425997797]You healed Letter for 1612 points of health.
		// ["#0000000042000018#","Me Cast Nano","",1425997797]Nano program executed successfully.
		// ["#0000000042000018#","Me Cast Nano","",1425998481]Executing Nano Program: Beauty of Life.
		// ["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
		// ["#0000000042000018#","Me Cast Nano","",1425998482]Nano program executed successfully.
		regex_search(words[4], m, regex("(?:Program:\\s)(.*?)(?=\\.)"));
		dealer.nano_casted.name = m[1];
		dealer.nano_casted.count += 1;
		cout << "Nano casted: " << dealer.nano_casted.name << ". Count: " << dealer.nano_casted.count << endl;
		
		// Need to check if the nano was executed succesfully or countered or fumbled or resisted
		// This info comes in the next line(s).
		// The check needs to be done when the player is know. So somewhere in that code.
		dealer.last_nano_cast_line = words;
	}
	else if (words[1] == "Your pet hit by nano") {
	}
	else if (words[1] == "Your pet hit by monster") {
	}
	else if (words[1] == "You gave nano") {
		// ["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
		dealer.name = "You";
		//regex_search(words[4], m, regex("hit\\s(.*?)(?= with)"));
		regex_search(words[4], m, regex("(?:nano on\\s)(.*?)(?= for)"));
		receiver.name = m[1];
		dealer.stats.type = "nano";
		receiver.stats.type = "nano";
		find_amount(words, dealer, receiver);
	}
	else if (words[1] == "Me got SK") {
		// ["#000000004200000c#","Me got SK","",1425993638]You gained 200 points of Shadowknowledge.
		// Can gain and lose
		receiver.name = "You";
		receiver.stats.type = "shadowknowledge";
		if (regex_search(words[4], m, regex("(?:gained\\s)(\\d+)")))
			receiver.stats.sk += stoi(m[1]);
		else {
			regex_search(words[4], m, regex("(?:lost\\s)(\\d+)"));
			receiver.stats.sk_lost += stoi(m[1]);
		}
		receiver.stats.amount = stoi(m[1]);
	}
	else if (words[1] == "Me got XP") {
		// Need to handle loss
		// get log line from loss of xp
		// ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
		// ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
		std::smatch xp;
		receiver.name = "You";
		regex_search(words[4], xp, regex("\\d+"));
		receiver.stats.amount = stoi(xp[0]);
		if (regex_search(words[4], m, regex("(?:new\\s)(.*?)(?=\\.)"))) {
			receiver.stats.type = m[1];
			receiver.stats.axp = stoi(xp[0]);
		}
		else {
			receiver.stats.type = "xp";
			receiver.stats.xp += stoi(xp[0]);
		}
	}
	else if (words[1] == "Research") {
		// ["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
		receiver.name = "You";
		regex_search(words[4], m, regex("\\d+"));
		receiver.stats.research = stoi(m[0]);
		receiver.stats.type = "research";
	}
	else
		cout << "No match for " << words[1] << endl;

	return 0;
}

void cleanup(vector<string>& words) {
	// Remove quotation marks etc.
	words[0].erase(0, 3);
	words[0].erase(16, 2);
	words[1].erase(0, 1);
	words[1].erase(words[1].length() - 1, words[1].length());
	words[2].erase(0, 1);
	words[2].erase(words[2].length() - 1, words[2].length());
}

vector<string>& split(string& s, char delim, vector<string>& words) {
	stringstream ss(s);
	string word;
	while (getline(ss, word, delim)) {
		if (!word.empty()) {
			if (delim != ']')
				split(word, ']', words);
			else
				words.push_back(word);
		}
	}
	return words;
}