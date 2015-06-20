#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include "parse.h"

using namespace std;

// TODO:
// After some perk actions there is a line telling which perk was performed
//	and if it was successfull or not. Can use that to get stats on perks.

int parse(std::string line, vector<Player>& players) {
    LineInfo lineInfo;

	// Split the line and store it in words
	vector<string> words;
	split(line, ',', words);

	if (!words.size() == 0) {
		cleanup(words);

		// find name of dealer, receiver, amount of damage/heal/nano, type and
		// subtype etc.
		find_values(words, lineInfo);

		// Debug prints
		cout << "Dealer: " << lineInfo.dealer_name << "\t" << "Receiver: "<<
            lineInfo.receiver_name << "\t" << "Amount: " <<
			lineInfo.amount << "\t" << "Type: " <<
			lineInfo.type << "\t" << "Subtype: " << lineInfo.subtype << endl;

		add_to_players(players, lineInfo);
	}
	return 0;
}

void add_to_players(std::vector<Player>& players, LineInfo& li) {
    // Move to player_manager
    bool dealer_found = false;
    bool receiver_found = false;
	for (auto& p : players) {
		if (p.get_name() == li.dealer_name) {
			p.add(li);
			dealer_found = true;
		}
		else if (p.get_name() == li.receiver_name) {
			p.add(li);
			receiver_found = true;
		}
	}
	if(!dealer_found && li.dealer_name != "") {
        Player p(li.dealer_name, li);
        players.push_back(p);
    }
	if(!receiver_found && li.receiver_name != "") {
        Player p(li.receiver_name, li);
        players.push_back(p);
    }
}

int find_amount(const std::vector<std::string>& words) {
	std::smatch d;
	if (regex_search(words[4], d, regex("(\\d+)( points)"))) {
        return std::stoi(d[1]);
    }
    else if (regex_search(words[4], d, regex("\\d+")))
        // For XP/SK/Reserach but might match some other line I've missed.
        return std::stoi(d[0]);
    else
        return -1;
}

std::string find_subtype(const std::vector<std::string>& words) {
	// Finds damage and heal subtype.
	std::smatch t;
	if (regex_search(words[4], t, regex("(?:points of )(.*?)(?= damage)")))	{
	 	// Looks for regular and special damage
	 	return t[1];
	}
	else if (regex_search(words[4], t, regex("(?: )(.* shield)(?= hit)"))) {
        // Looks for reflect or shield damage
        return t[1];
	}
	else if (regex_search(words[4], t, regex("(?: with )(.*)"))) {
        // Looks for special damage in case of a miss.
        return t[1];
	}
	else if (regex_search(words[4], t, regex(" tried to hit "))) {
        return "regular miss";
	}
	else if (regex_search(words[4], t, regex(" got healed "))) {
        return "potential";
	}
	else if (regex_search(words[4], t, regex(" were healed "))) {
        return "actual";
	}
	else {
        return "";
    }
}

bool isCrit(const std::vector<std::string>& words) {
    std::smatch m;
    if (regex_search(words[4], m, regex("damage.Critical hit!")))
        return true;
    else
        return false;
}

bool isDeflect(const std::vector<std::string>& words) {
    std::smatch m;
    if (regex_search(words[4], m, regex("damage. Glancing hit.")))
        return true;
    else
        return false;
}

int find_values(const vector<string>& words, LineInfo& lineInfo) {
	// Solve each individually, merge similar?

	// Fix so all cases can use find_subtype() and find_amout()

	std::smatch m;

	if (words[1] == "Other hit by other" || words[1] == "Your pet hit by other") {
        /*
        ["#000000004200000a#","Other hit by other","",1425326282]Sheila Marlene hit Predator Rogue for 461 points of melee damage.
        ["#000000004200000a#","Other hit by other","",1425326285]Sgtcuddle hit Predator Rogue for 1434 points of energy damage.
        ["#000000004200000a#","Other hit by other","",1425326287]Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.
        ["#000000004200000a#","Other hit by other","",1425326293]Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.
        ["#000000004200000a#","Other hit by other","",1434406471]Rezipped hit Imgonnakillu for 4123 points of Aimed Shot damage.
        ["#000000004200000a#","Other hit by other","",1434406034]Balas's reflect shield hit Junebop for 161 points of damage.
        ["#000000004200000a#","Other hit by other","",1425997610]Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!
        ["#000000004200000a#","Other hit by other","",1434359748]Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit.
		// Make it handle absorb:
		["#000000004200000a#","Other hit by other","",1434406044]Someone absorbed 8198 points of energy damage.

		Fix so it can handle glancing hits:
        ["#0000000042000009#","Your pet hit by other","",1425996728]Letter hit Guard for 623 points of melee damage.
		["#0000000042000009#","Your pet hit by other","",1425996758]Letter hit Guard for 1268 points of melee damage.Critical hit!
		["#0000000042000009#","Your pet hit by other","",1425996734]Guard hit Letter for 1639 points of melee damage.Critical hit!
		*/
		regex_search(words[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"));
		lineInfo.dealer_name = m[0];
		if (regex_search(words[4], m, regex("(?:hit )"	    // Find "hit ", but do not include it in the results
										"(.*?)"			    // match everything following, non-greedy
                                                            // i.e. until first occurrence, of
										"(?= for)"))) {	// " for"
            lineInfo.receiver_name = m[1];
        }
        else if (regex_search(words[4], m, regex("(.*?)(?= absorbed )"))) {
            lineInfo.receiver_name = m[1];
        }
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		lineInfo.crit = isCrit(words);
		lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "Other hit by nano") {
        // Can this crit?
		// ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
		// ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
		if (!regex_search(words[4], m, regex("(?:from\\s)(.*?)(?= for)")))
			regex_search(words[4], m, regex("of\\s(.*?)(?= damage)"));
		lineInfo.dealer_name = m[1];
		regex_search(words[4], m, regex("(.*?)(?= was)"));
		lineInfo.receiver_name = m[0];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
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
		lineInfo.dealer_name = "You";
		regex_search(words[4], m, regex("(?:hit\\s)(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		lineInfo.crit = isCrit(words);
        lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "You hit other with nano") {
		// ["#0000000042000005#","You hit other with nano","",1425993792]You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.
		lineInfo.dealer_name = "You";
		regex_search(words[4], m, regex("(?:hit\\s)(.*?)(?= with)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		// Need to save that it was nanobot damage. Or do I? Is nanobots always == perks?
	}
	else if (words[1] == "Me got health"){
		/*
		["#0000000042000015#","Me got health","",1425326282]You were healed for 193 points.
		["#0000000042000015#","Me got health","",1425995902]You got healed by Starphoenix for 3359 points of health.
		["#0000000042000015#","Me got health","",1425995902]You were healed for 3359 points.
		["#0000000042000015#","Me got health","",1425996008]You got healed by Starphoenix for 3173 points of health.
		["#0000000042000015#","Me got health","",1425996008]You were healed for 1949 points.
		*/
		// In the case of a heal casted on you:
		//   The first line starting with "You got healed" will say how much the
		//   heal will potentially heal for.
		//   The second line will say how much the heal actually healed for.
		//   Maybe it's possible for another heal to sneak inbetweeen these two.
		// In the case of heal delta:
		//   There will only be a "You were healed..." message.
		lineInfo.receiver_name = "You";
		lineInfo.type = "heal";
		lineInfo.subtype = find_subtype(words);
        lineInfo.amount = find_amount(words);
		if (lineInfo.subtype == "potential") { // Potential heal
			regex_search(words[4], m, regex("(?:by )(.*?)(?= for)"));
			lineInfo.dealer_name = m[1];
		}
    }
	else if (words[1] == "You gave health") {
        /*
		["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
		*/
		lineInfo.dealer_name = "You";
		regex_search(words[4], m, regex("(?:healed )(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.amount = find_amount(words);
		lineInfo.type = "heal";
		lineInfo.subtype = "potential"; // Find out if the subtype here is actual or potential.
	}
	else if (words[1] == "Me hit by monster") {
        /*
		["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
		["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!

		["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.

		Fix so it can handle glancing hits
		["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
        */
		regex_search(words[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"));  // Match as few chars as possible untill " hit".
		lineInfo.dealer_name = m[0];
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
        lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "Me hit by nano") {
		/*
		["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
		*/
		regex_search(words[4], m, regex("(?:from\\s)(.*?)(?= for)"));
		lineInfo.dealer_name = m[1];
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
	}
	else if (words[1] == "Me hit by player") {
		lineInfo.receiver_name = "You";
		// need to get example lines
		// need to find player name
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		lineInfo.crit = isCrit(words);
        lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "Me hit by monster") {
        /*
        ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
        ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
		*/
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		lineInfo.crit = isCrit(words);
        lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "Other misses") {
        /*
		There is an extra comma in this line. Might mess something up.
		["#0000000042000013#","Other misses","",1425326282]Predator Rogue tried to hit you, but missed!
		*/
		regex_search(words[4], m, regex("(.*?)(?= tried)"));
		lineInfo.dealer_name = m[0];
		regex_search(words[4], m, regex("(?:hit\\s)(.*)"));
        // The log line is split at "," so everything following the "," is in
        // words[5]
		if (m[1] == "you")
			lineInfo.receiver_name = "You";
		else
			lineInfo.receiver_name = m[1];
        lineInfo.miss = true;
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words); // Does not work. I guess specials
                                                 // need to be looked for here too.
	}
	else if (words[1] == "Your misses") {
        /*
        Brawl is named differently here compared to when it does damage. It's then called "Brawling".
		["#0000000042000012#","Your misses","",1425666157]You try to attack Peal Thunder with Brawl, but you miss!
		["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
		*/
		lineInfo.dealer_name = "You";
		if (!regex_search(words[4], m, regex("(?:attack\\s)(.*?)(?= with)"))) {
            regex_search(words[4], m, regex("(?:hit\\s)(.*)"));
            // The log line is split at "," so everything following the "," is in
            // words[5]
        }
		lineInfo.receiver_name = m[1];
		lineInfo.miss = true;
		lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(words);  // Does not find Brawl
	}
	else if (words[1] == "Me hit by environment") {
		// need to get example line
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(words);
		lineInfo.amount = find_amount(words);
		lineInfo.crit = isCrit(words);
        lineInfo.deflect = isDeflect(words);
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

		// ["#0000000042000018#","Me Cast Nano","",1434359463]Executing Nano Program: Uncontrollable Body Tremors.
        // ["#0000000042000018#","Me Cast Nano","",1434359463]Nano program aborted.
		// ["#0000000042000018#","Me Cast Nano","",1434359463]Executing Nano Program: Uncontrollable Body Tremors.
		// ["#0000000042000018#","Me Cast Nano","",1434359074]Target resisted.

        lineInfo.type = "nano_cast";
        lineInfo.dealer_name = "You";
		if (regex_search(words[4], m, regex("(?:Program:\\s)(.*?)(?=\\.)"))) {
            lineInfo.nanoProgram->setName(m[1]);
            lineInfo.subtype = "execute";
        }
        else if (words[4] == "Nano program executed successfully.") {
            lineInfo.subtype = "land";
        }
        else if (words[4] == "Target resisted.") {
            lineInfo.subtype = "resist";
        }
        else if (words[4] == "Nano program aborted.") {
            lineInfo.subtype = "abort";
        }
        else if (words[4] == "Target countered.") {  // Find one of these log messages
            lineInfo.subtype = "counter";
        }
        else if (words[4] == "You fumbled.") {  // Find one of these log messages
            lineInfo.subtype = "fumble";
        }
        lineInfo.nanoProgram->addStat(lineInfo.subtype, 1);
        cout << "Nano casted: " << lineInfo.nanoProgram->getName() << endl;
	}
	else if (words[1] == "Your pet hit by nano") {
		lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(words);
        lineInfo.amount = find_amount(words);
        lineInfo.crit = isCrit(words);
	}
	else if (words[1] == "Your pet hit by monster") {
        lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(words);
        lineInfo.amount = find_amount(words);
        lineInfo.crit = isCrit(words);
        lineInfo.deflect = isDeflect(words);
	}
	else if (words[1] == "You gave nano") {
		// ["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
		lineInfo.dealer_name = "You";
		//regex_search(words[4], m, regex("hit\\s(.*?)(?= with)"));
		regex_search(words[4], m, regex("(?:nano on\\s)(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "nano";
		lineInfo.amount = find_amount(words);
	}
	else if (words[1] == "Me got SK") {
		// ["#000000004200000c#","Me got SK","",1425993638]You gained 200 points of Shadowknowledge.
		// Can gain and lose
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
        lineInfo.amount = find_amount(words);
		if (regex_search(words[4], m, regex("(?:gained\\s)(\\d+)"))) {
            lineInfo.subtype = "sk";
        }
		else {
			regex_search(words[4], m, regex("(?:lost\\s)(\\d+)"));
            lineInfo.subtype = "sk lost";
		}
	}
	else if (words[1] == "Me got XP") {
		// Need to handle loss
		// get log line from loss of xp
		// ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
		// ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
		std::smatch xp;
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
		lineInfo.amount = find_amount(words);
		if (regex_search(words[4], m, regex("(?:new\\s)(.*?)(?=\\.)"))) {
			lineInfo.subtype = m[1];
		}
		else {
			lineInfo.subtype = "xp";
		}
	}
	else if (words[1] == "Research") {
		// ["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
		lineInfo.amount = find_amount(words);
		lineInfo.subtype = "research";
	}
	else if (words[1] == "Victory Points") {
        // Check what this message looks like.
        // Maybe it's a system message.
	}
	else if (words[1] == "System") {

	}
	else
		cout << "No match for " << words[1] << endl;  // Write this to a log file (include the full damage log line)

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
