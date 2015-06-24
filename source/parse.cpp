#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include "parse.h"
#include "logger.h"
#include "log_line.h"


using std::cout;
using std::endl;
using std::regex_search;
using std::regex;


/*
TODO:
Make adding nano to a player work.

After some perk actions there is a line telling which perk was performed
    and if it was successfull or not. Can use that to get stats on perks?

Save all nanobots damage in an additional category?
    Separate persk from nukes if possible. (think they look the same).

*/

int parse(std::string line, std::vector<Player>& players) {
    LogLine logLine;

	// Split the line and store it in a vector
    if (logLine.format(line)) {  // If successfull format

        // find name of dealer, receiver, amount of damage/heal/nano, type and
        // subtype etc.
        find_values(logLine);

        // Debug prints
        std::cout << "Dealer: " << logLine.getInfo().dealer_name << "\t" << "Receiver: "<<
            logLine.getInfo().receiver_name << "\t" << "Amount: " <<
            logLine.getInfo().amount << "\t" << "Type: " <<
            logLine.getInfo().type << "\t" << "Subtype: " <<
            logLine.getInfo().subtype << std::endl;

        add_to_players(players, logLine);

        return 0;
    }
    else {
        return 1;
    }
}

void add_to_players(std::vector<Player>& players, LogLine& ll) {
    // Move this function to player_manager
    bool dealer_found = false;
    bool receiver_found = false;
    LineInfo& li = ll.getInfo();
	for (auto& p : players) {
		if (p.get_name() == li.dealer_name) {
			p.add(ll);
			dealer_found = true;
		}
		else if (p.get_name() == li.dealer_name) {
			p.add(ll);
			receiver_found = true;
		}
	}
	if(!dealer_found && li.dealer_name != "") {
        Player p(li.dealer_name, ll);
        players.push_back(p);
    }
	if(!receiver_found && li.receiver_name != "") {
        Player p(li.receiver_name, ll);
        players.push_back(p);
    }
}

int find_amount(LogLine& logLine) {
    const std::vector<std::string>& splitLine = logLine.getSplitLine();
	std::smatch d;
	if (regex_search(splitLine[4], d, regex("(\\d+)( points)"))) {
        return std::stoi(d[1]);
    }
    else if (regex_search(splitLine[4], d, regex("\\d+\\s"))) {
        // For XP/SK/Reserach but might match some other line I've missed.
        return std::stoi(d[0]);
    }
    else {
        errorLog.write("Amount not found in the following line: ");
        errorLog.write(logLine.getOriginalLine());
        return 0;
    }
}

std::string find_subtype(const std::vector<std::string>& splitLine) {
	// Finds damage and heal subtype.
    //  ["#0000000042000001#","Me hit by environment","",1160126071]You were damaged by a toxic substance for 123 points of damage.

	std::smatch t;
	if (regex_search(splitLine[4], t, regex("(?:points of )(.*?)(?= damage)")))	{
	 	// Looks for regular and special damage
	 	return t[1];
	}
	else if (regex_search(splitLine[4], t, regex("(?: )(.* shield)(?= hit)"))) {
        // Looks for reflect or shield damage
        return t[1];
	}
	else if (regex_search(splitLine[4], t, regex("(?: with )(.*)"))) {
        // Looks for special damage in case of a miss.
        return t[1];
	}
	else if (regex_search(splitLine[4], t, regex(" tried to hit "))) {
        return "regular miss";
	}
	else if (regex_search(splitLine[4], t, regex(" got healed "))) {
        return "potential";
	}
	else if (regex_search(splitLine[4], t, regex(" were healed "))) {
        return "actual";
	}
	else if (regex_search(splitLine[4], t, regex("(?: damaged by )(.*?)( for )"))) {
        return t[1];
	}
	else {
        return "";
    }
}

bool isCrit(const std::vector<std::string>& splitLine) {
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("damage.Critical hit!")))
        return true;
    else
        return false;
}

bool isDeflect(const std::vector<std::string>& splitLine) {
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("damage. Glancing hit.")))
        return true;
    else
        return false;
}

int find_values(LogLine& logLine) {
	// Solve each individually, merge similar?

	// Combine searches into one regex where possible.

	// Fix so all cases can use find_subtype() and find_amout()

	std::smatch m;
	const std::vector<std::string>& splitLine = logLine.getSplitLine();
	LineInfo& lineInfo = logLine.getInfo();

	if (splitLine[1] == "Other hit by other" || splitLine[1] == "Your pet hit by other") {
        /*
        ["#000000004200000a#","Other hit by other","",1425326282]Sheila Marlene hit Predator Rogue for 461 points of melee damage.
        ["#000000004200000a#","Other hit by other","",1425326285]Sgtcuddle hit Predator Rogue for 1434 points of energy damage.
        ["#000000004200000a#","Other hit by other","",1425326287]Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.
        ["#000000004200000a#","Other hit by other","",1425326293]Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.
        ["#000000004200000a#","Other hit by other","",1434406471]Rezipped hit Imgonnakillu for 4123 points of Aimed Shot damage.
        ["#000000004200000a#","Other hit by other","",1434406034]Balas's reflect shield hit Junebop for 161 points of damage.
        ["#000000004200000a#","Other hit by other","",1425997610]Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!
        ["#000000004200000a#","Other hit by other","",1434359748]Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit.
:
		["#000000004200000a#","Other hit by other","",1434406044]Someone absorbed 8198 points of energy damage.

        ["#0000000042000009#","Your pet hit by other","",1425996728]Letter hit Guard for 623 points of melee damage.
		["#0000000042000009#","Your pet hit by other","",1425996758]Letter hit Guard for 1268 points of melee damage.Critical hit!
		["#0000000042000009#","Your pet hit by other","",1425996734]Guard hit Letter for 1639 points of melee damage.Critical hit!

		// These lines still exist?
		["#000000004200000a#","Other hit by other","",1183505123]Something hit Addicted2 for 49 points of damage by reflect shield.
        ["#000000004200000a#","Other hit by other","",1183504118]Something hit Enfodruid for 1 points of damage by damage shield.
		*/
		if (regex_search(splitLine[4], m, regex("(?:hit )"	// Find "hit ", but do not include it in the results
										"(.*?)"			    // match everything following, non-greedy
                                                            // i.e. until first occurrence, of
										"(?= for)"))) {	    // " for"
            lineInfo.receiver_name = m[1];
        }
        else if (regex_search(splitLine[4], m, regex("(.*?)(?= absorbed )"))) {
            lineInfo.receiver_name = m[1];
        }
		if (regex_search(splitLine[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))){
            lineInfo.dealer_name = m[0];
            }
        else if (lineInfo.receiver_name == "Someone") {
            lineInfo.dealer_name = "Unknown";  // There will be no dealer in this case.
        }

		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		lineInfo.crit = isCrit(splitLine);
		lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Other hit by nano") {
        // Can this crit?
		// ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
		// ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
		if (regex_search(splitLine[4], m, regex("(?:from )(.*?)(?= for)")) ||
			regex_search(splitLine[4], m, regex("(?:of )(.*?)(?= damage)"))) {
			if (m[1] == "unknown") {
                lineInfo.dealer_name = "Unknown";
			}
            else {
                lineInfo.dealer_name = m[1];
            }
		}
		regex_search(splitLine[4], m, regex("(.*?)(?= was)"));
		lineInfo.receiver_name = m[0];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
	}
	else if (splitLine[1] == "You hit other") {
		/*
		["#0000000042000008#","You hit other","",1425326282]You hit Predator Rogue for 2357 points of poison damage.
		["#0000000042000008#","You hit other","",1425326284]You hit Predator Rogue for 2329 points of chemical damage.
		["#0000000042000008#","You hit other","",1425326288]You hit Predator Rogue for 2357 points of Fast Attack damage.
		["#0000000042000008#","You hit other","",1425326293]You hit Predator Rogue for 1942 points of Brawling damage.
		["#0000000042000008#","You hit other","",1434405007]You hit Kyr'Ozch Hive Technician for 3017 points of Fling Shot damage.
		["#0000000042000008#","You hit other","",1425326291]You hit Predator Rogue for 3911 points of poison damage.Critical hit!
		["#0000000042000008#","You hit other","",1425326284]Your reflect shield hit Predator Rogue for 49 points of damage.
		["#0000000042000008#","You hit other","",1425326284]Your damage shield hit Predator Rogue for 163 points of damage.
		*/
		lineInfo.dealer_name = "You";
		regex_search(splitLine[4], m, regex("(?:hit )(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		lineInfo.crit = isCrit(splitLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "You hit other with nano") {
		// ["#0000000042000005#","You hit other with nano","",1425993792]You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.
		lineInfo.dealer_name = "You";
		regex_search(splitLine[4], m, regex("(?:hit )(.*?)(?= with)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		// Need to save that it was nanobot damage. Or do I? Is nanobots always == perks?
	}
	else if (splitLine[1] == "Me got health"){
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
		lineInfo.subtype = find_subtype(splitLine);
        lineInfo.amount = find_amount(logLine);
		if (lineInfo.subtype == "potential") { // Potential heal
			regex_search(splitLine[4], m, regex("(?:by )(.*?)(?= for)"));
			lineInfo.dealer_name = m[1];
		}
    }
	else if (splitLine[1] == "You gave health") {
        /*
		["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
		*/
		lineInfo.dealer_name = "You";
		regex_search(splitLine[4], m, regex("(?:healed )(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.amount = find_amount(logLine);
		lineInfo.type = "heal";
		lineInfo.subtype = "potential"; // Find out if the subtype here is actual or potential.
	}
	else if (splitLine[1] == "Me hit by monster") {
        /*
		["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
		["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
		["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.
		["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
        */
		regex_search(splitLine[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"));  // Match as few chars as possible untill " hit".
		lineInfo.dealer_name = m[0];
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Me hit by nano") {
		/*
		["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
		*/
		regex_search(splitLine[4], m, regex("(?:from )(.*?)(?= for)"));
		lineInfo.dealer_name = m[1];
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
	}
	else if (splitLine[1] == "Me hit by player") {
		/*
		["#0000000042000007#","Me hit by player","",1434406040]Player Balas hit you for 854 points of projectile damage.
        ["#0000000042000007#","Me hit by player","",1434406024]Balas hit you for 949 points of Aimed Shot damage.
        */
		lineInfo.receiver_name = "You";
		if (regex_search(splitLine[4], m, regex("(?:Player )(.*?)(?= hit you for )")) ||
            regex_search(splitLine[4], m, regex("(.*?)(?= hit you for )"))) {
            lineInfo.dealer_name = m[1];
		}
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		lineInfo.crit = isCrit(splitLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Me hit by monster") {
        /*
        ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
        ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.
        ["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
		*/
		lineInfo.receiver_name = "You";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		lineInfo.crit = isCrit(splitLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Other misses") {
        /*
		["#0000000042000013#","Other misses","",1425326282]Predator Rogue tried to hit you, but missed!
		// Find my own version:
		["#0000000042000013#","Other misses","",1180557880]Keepone tries to attack you with FastAttack, but misses!
		*/
        // The log line is split at "," so everything following the "," is in
        // splitLine[5]

   		if (regex_search(splitLine[4], m, regex("(.*?)(?: tried to hit )(.*)")) ||
            regex_search(splitLine[4], m, regex("(.*?)(?: tries to attack )(.*?)(?: with )"))) {
            lineInfo.dealer_name = m[1];
            lineInfo.receiver_name = m[2];
            if (lineInfo.receiver_name == "you") {
                lineInfo.receiver_name = "You";
            }
        }
        lineInfo.miss = true;
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
        renameSpecial(lineInfo);  // Specials in misses have a different name
	}
	else if (splitLine[1] == "Your misses") {
        /*
		["#0000000042000012#","Your misses","",1425666157]You try to attack Peal Thunder with Brawl, but you miss!
		["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
		*/
		lineInfo.dealer_name = "You";
		if (regex_search(splitLine[4], m, regex("(?:attack )(.*?)(?= with )")) ||
            regex_search(splitLine[4], m, regex("(?:hit )(.*)"))) {
            lineInfo.receiver_name = m[1];
        }
		lineInfo.miss = true;
		lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(splitLine);
        renameSpecial(lineInfo);  // Specials in misses have a different name
	}
	else if (splitLine[1] == "Me hit by environment") {
		/* Need to get my own example line
		["#0000000042000001#","Me hit by environment","",1160126071]You were damaged by a toxic substance for 123 points of damage.
		*/
		lineInfo.receiver_name = "You";
		lineInfo.dealer_name = "Environment";
		lineInfo.type = "damage";
		lineInfo.subtype = find_subtype(splitLine);
		lineInfo.amount = find_amount(logLine);
		lineInfo.crit = isCrit(splitLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Me Cast Nano") {
        /*
        ["#0000000042000018#","Me Cast Nano","",1425326300]Executing Nano Program: Improved Mongo Crush!.
        ["#0000000042000018#","Me Cast Nano","",1425326300]Nano program executed successfully.
        ["#0000000042000018#","Me Cast Nano","",1425997797]Executing Nano Program: Thorough Overhaul.
        ["#0000000042000014#","You gave health","",1425997797]You healed Letter for 1612 points of health.
        ["#0000000042000018#","Me Cast Nano","",1425997797]Nano program executed successfully.
        ["#0000000042000018#","Me Cast Nano","",1425998481]Executing Nano Program: Beauty of Life.
        ["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
        ["#0000000042000018#","Me Cast Nano","",1425998482]Nano program executed successfully.

        ["#0000000042000018#","Me Cast Nano","",1434359463]Executing Nano Program: Uncontrollable Body Tremors.
        ["#0000000042000018#","Me Cast Nano","",1434359463]Nano program aborted.
        ["#0000000042000018#","Me Cast Nano","",1434359463]Executing Nano Program: Uncontrollable Body Tremors.
        ["#0000000042000018#","Me Cast Nano","",1434359074]Target resisted.
        ["#0000000042000018#","Me Cast Nano","",1434561648]Executing Nano Program: Uncontrollable Body Tremors.
        ["#0000000042000018#","Me Cast Nano","",1434561650]Your target countered the nano program.

        Fix:
        This nano will execute twice. It will cast the HP buff, then heal.
        When the last nano has received a status (land/resist etc.) I could clear the pointer.
        If then another status message arrives, I can ignore it.
        ["#0000000042000018#","Me Cast Nano","",1434562392]Executing Nano Program: Team Improved Life Channeler.
        ["#0000000042000018#","Me Cast Nano","",1434562393]Nano program executed successfully.
        ["#0000000042000015#","Me got health","",1434562393]You were healed for 3025 points.
        ["#0000000042000018#","Me Cast Nano","",1434562393]Nano program executed successfully.
        */
        lineInfo.type = "nano_cast";
        lineInfo.dealer_name = "You";
		if (regex_search(splitLine[4], m, regex("(?:Program:\\s)(.*?)(?=\\.)"))) {
            lineInfo.nanoProgram->setName(m[1]);
            lineInfo.subtype = "execute";
        }
        else if (splitLine[4] == "Nano program executed successfully.") {
            lineInfo.subtype = "land";
        }
        else if (splitLine[4] == "Target resisted.") {
            lineInfo.subtype = "resist";
        }
        else if (splitLine[4] == "Nano program aborted.") {
            lineInfo.subtype = "abort";
        }
        else if (splitLine[4] == "Your target countered the nano program.") {
            lineInfo.subtype = "counter";
        }
        else if (splitLine[4] == "You fumbled.") {  // Find one of these log messages
            lineInfo.subtype = "fumble";
        }
        lineInfo.nanoProgram->addStat(lineInfo.subtype, 1);
        cout << "Nano casted: " << lineInfo.nanoProgram->getName() << endl;
	}
	else if (splitLine[1] == "Your pet hit by nano") {
		lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(splitLine);
        lineInfo.amount = find_amount(logLine);
        lineInfo.crit = isCrit(splitLine);
	}
	else if (splitLine[1] == "Your pet hit by monster") {
        /* Find my own log line, also find one of these when it this "you" and "other".
        ["#0000000042000011#","Your pet hit by monster","",1191516331]Your pet Vios was damaged by a toxic substance for 25 points of damage.

        There must be other variations of this type too. Find them.
        */
        regex_search(splitLine[4], m, regex("(?:Your pet )(.*?)( was damaged by a )"));
        lineInfo.receiver_name = m[1];
        lineInfo.dealer_name = "Environment";
        lineInfo.type = "damage";
        lineInfo.subtype = find_subtype(splitLine);
        lineInfo.amount = find_amount(logLine);
        lineInfo.crit = isCrit(splitLine);
        lineInfo.deflect = isDeflect(splitLine);
	}
	else if (splitLine[1] == "Me got SK") {
		// ["#000000004200000c#","Me got SK","",1425993638]You gained 200 points of Shadowknowledge.
		// ["#000000004200000c#","Me got SK","",1425994822]You lost 200 points of Shadowknowledge
		// Can gain and lose
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
        lineInfo.amount = find_amount(logLine);
		if (regex_search(splitLine[4], m, regex("gained"))) {
            lineInfo.subtype = "sk";
        }
		else if (regex_search(splitLine[4], m, regex("lost"))) {
            lineInfo.subtype = "sk lost";
		}
	}
	else if (splitLine[1] == "Me got XP") {
		// ["#000000004200000b#","Me got XP","",1426200654]You lost 9822 xp.
		// ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
		// ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
		std::smatch xp;
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
		lineInfo.amount = find_amount(logLine);
        if (regex_search(splitLine[4], m, regex("gained"))) {
            lineInfo.subtype = "aixp";
        }
        else if (regex_search(splitLine[4], m, regex("received"))) {
            lineInfo.subtype = "xp";
		}
		else if (regex_search(splitLine[4], m, regex("lost"))) {
            // Find out what an aixp loss looks like
            lineInfo.subtype = "xp lost";
        }
	}
	else if (splitLine[1] == "Research") {
        /*
        There is also global research. Ignore it for now.
		["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
		*/
		lineInfo.receiver_name = "You";
		lineInfo.type = "xp";
        lineInfo.subtype = "research";
		lineInfo.amount = find_amount(logLine);
	}
	else if (splitLine[1] == "You gave nano") {
        /*
		["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
		*/
		lineInfo.dealer_name = "You";
		regex_search(splitLine[4], m, regex("(?:nano on )(.*?)(?= for)"));
		lineInfo.receiver_name = m[1];
		lineInfo.type = "nano";
		lineInfo.amount = find_amount(logLine);
	}
	else if (splitLine[1] == "Me got nano") {
        /* Find my own log line, remember to replace in the log file.
        ["#0000000042000016#","Me got nano","",1180555427]You got nano from Jspe80 for 288 points.
        */
        lineInfo.receiver_name = "You";
        regex_search(splitLine[4], m, regex("(?:You got nano from )(.*?)(?= for)"));
        lineInfo.dealer_name = m[1];
        lineInfo.type = "nano";
        lineInfo.amount = find_amount(logLine);
	}
    else if (splitLine[1] == "Victory Points") {
        // Check what this message looks like.
        // Maybe it's a system message.
	}
	else if (splitLine[1] == "System") {

	}
	else {
        errorLog.write("No match for the following line: ");
        errorLog.write(logLine.getOriginalLine());
    }
	return 0;
}

void renameSpecial(LineInfo& li) {
    // Misses writes specials to the log different so they have to be renamed.
    // I can either rename these here, or when I get the data from a player,
    // I can get both "Brawl" and "Brawling" for example.
    if(li.subtype == "FastAttack") {
        li.subtype = "Fast Attack";
    }
    else if (li.subtype == "FlingShot") {
        li.subtype = "Fling Shot";
    }
    else if(li.subtype == "Brawl") {
        li.subtype = "Brawling";
    }
    else if(li.subtype == "FullAuto") {
        li.subtype = "Full Auto";
    }
    else if(li.subtype == "AimedShot") {
        li.subtype = "Aimed Shot";
    }
}

