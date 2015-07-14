#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include <map>
#include <stdexcept>
#include "parser.h"
#include "logger.h"
#include "log_line.h"
#include "configuration.h"


using std::cout;
using std::endl;
using std::regex_search;
using std::regex;


/*
TODO:
Make adding nano to a player work.

Give the option to set player name so it can handle damage to self.
Like from the Rage nano or a self nuke.

After some perk actions there is a line telling which perk was performed
and if it was successfull or not. Can use that to get stats on perks?
*/

Parser::Parser() {
    createFunctionMap();
}

void Parser::createFunctionMap() {
    funcMap["Other hit by other"] = &Parser::otherAndYourPetHitByOther;
    funcMap["Your pet hit by other"] = &Parser::otherAndYourPetHitByOther;
    funcMap["Other hit by nano"] = &Parser::otherHitByNano;
    funcMap["You hit other"] = &Parser::youHitOther;
    funcMap["You hit other with nano"] = &Parser::youHitOtherWithNano;
    funcMap["Me got health"] = &Parser::meGotHealth;
    funcMap["You gave health"] = &Parser::youGaveHealth;
    funcMap["Me hit by monster"] = &Parser::meHitByMonster;
    funcMap["Me hit by nano"] = &Parser::meHitByNano;
    funcMap["Me hit by player"] = &Parser::meHitByPlayer;
    funcMap["Other misses"] = &Parser::otherMisses;
    funcMap["Your misses"] = &Parser::yourMisses;
    funcMap["Me hit by environment"] = &Parser::meHitByEnvironment;
    funcMap["Me Cast Nano"] = &Parser::meCastNano;
    funcMap["Your pet hit by nano"] = &Parser::yourPetHitByNano;
    funcMap["Your pet hit by monster"] = &Parser::yourPetHitByMonster;
    funcMap["Me got SK"] = &Parser::meGotSK;
    funcMap["Me got XP"] = &Parser::megotXP;
    funcMap["Research"] = &Parser::research;
    funcMap["You gave nano"] = &Parser::youGaveNano;
    funcMap["Me got nano"] = &Parser::meGotNano;
    funcMap["Victory Points"] = &Parser::victoryPoints;
    funcMap["System"] = &Parser::system;
    funcMap["Vicinity"] = &Parser::chat;
    funcMap["Team"] = &Parser::chat;
    funcMap["00000003000011fc"] = &Parser::chat;
}

LogLine Parser::parse(std::string line) {
    LogLine logLine;

    // Split the line and store it in a vector
    if (logLine.format(line)) {  // If successfull format
        find_values(logLine);

        // Debug prints
        std::cout << "Dealer: " << logLine.getInfo().dealer_name << "\t" <<
                     "Receiver: " << logLine.getInfo().receiver_name << "\t" <<
                     "Amount: " << logLine.getInfo().amount << "\t" <<
                     "Type: " << logLine.getInfo().type << "\t" <<
                     "Subtype: " << logLine.getInfo().subtype << std::endl;
    }
    return logLine;
}

int Parser::find_values(LogLine& logLine) {
    // Solve each individually, merge similar?
    // Combine searches into one regex where possible.
    // Fix so all cases can use find_subtype() and find_amout()

    try {
        pfunc f = funcMap.at(logLine.getDescription());
        // Write a setInfo method:
        logLine.getInfo() = (this->*f)(logLine.getSplitLine());
    }
    catch(std::out_of_range& e) {
        errorLog.write("Out of Range error: ", false);
        errorLog.write(e.what());
        errorLog.write("No match for the following line: ");
        errorLog.write(logLine.getOriginalLine());
    }
    return 0;
}

int Parser::find_amount(const std::vector<std::string>& splitLine) {
    //if (logLine.getDescription() == "Me Cast Nano" ||
    //    logLine.getDescription() == "System" ||
    //    logLine.getDescription() == "Vicinity" ||
    //    logLine.getDescription() == "Me Cast Nano" ||
    //    logLine.getDescription() == "00000003000011fc" ||) {
    //    return 0;
    //}
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
        for (const std::string& subLine : splitLine) {
            errorLog.write(subLine);
        }
        return 0;
    }
}

std::string Parser::find_subtype(const std::vector<std::string>& splitLine) {
    // Finds damage and heal subtype.
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

bool Parser::isCrit(const std::vector<std::string>& splitLine) {
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("damage.Critical hit!")))
        return true;
    else
        return false;
}

bool Parser::isDeflect(const std::vector<std::string>& splitLine) {
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("damage. Glancing hit.")))
        return true;
    else
        return false;
}

void Parser::renameSpecial(LineInfo& li) {
    // Misses writes specials to the log different so they have to be renamed.
    // I can either rename these here, or when I get the data from a player,
    // I can get both "Brawl" and "Brawling" for example.
    if (li.subtype == "FastAttack") {
        li.subtype = "Fast Attack";
    }
    else if (li.subtype == "FlingShot") {
        li.subtype = "Fling Shot";
    }
    else if (li.subtype == "Brawl") {
        li.subtype = "Brawling";
    }
    else if (li.subtype == "FullAuto") {
        li.subtype = "Full Auto";
    }
    else if (li.subtype == "AimedShot") {
        li.subtype = "Aimed Shot";
    }
}

//////////////////////////////////////////
//                                      //
//  Functions within the function map:  //
//                                      //
//////////////////////////////////////////

LineInfo Parser::otherAndYourPetHitByOther(const std::vector<std::string>& splitLine) {
    /*
    ["#000000004200000a#","Other hit by other","",1425326282]Sheila Marlene hit Predator Rogue for 461 points of melee damage.
    ["#000000004200000a#","Other hit by other","",1425326285]Sgtcuddle hit Predator Rogue for 1434 points of energy damage.
    ["#000000004200000a#","Other hit by other","",1425326287]Sgtcuddle hit Predator Rogue for 4302 points of Burst damage.
    ["#000000004200000a#","Other hit by other","",1425326293]Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage.
    ["#000000004200000a#","Other hit by other","",1434406471]Rezipped hit Imgonnakillu for 4123 points of Aimed Shot damage.
    ["#000000004200000a#","Other hit by other","",1434406034]Balas's reflect shield hit Junebop for 161 points of damage.
    ["#000000004200000a#","Other hit by other","",1425997610]Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!
    ["#000000004200000a#","Other hit by other","",1434359748]Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit.

    ["#000000004200000a#","Other hit by other","",1434406044]Someone absorbed 8198 points of energy damage.

    ["#0000000042000009#","Your pet hit by other","",1425996728]Letter hit Guard for 623 points of melee damage.
    ["#0000000042000009#","Your pet hit by other","",1425996758]Letter hit Guard for 1268 points of melee damage.Critical hit!
    ["#0000000042000009#","Your pet hit by other","",1425996734]Guard hit Letter for 1639 points of melee damage.Critical hit!

    // These lines still exist?
    ["#000000004200000a#","Other hit by other","",1183505123]Something hit Addicted2 for 49 points of damage by reflect shield.
    ["#000000004200000a#","Other hit by other","",1183504118]Something hit Enfodruid for 1 points of damage by damage shield.
    */

    LineInfo li;
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("(?:hit )"	    // Find "hit ", but do not include it in the results
                                            "(.*?)"			// match everything following, non-greedy
                                                            // i.e. until first occurrence, of
                                            "(?= for)"))) {	// " for"
        li.receiver_name = m[1];
    }
    else if (regex_search(splitLine[4], m, regex("(.*?)(?= absorbed )"))) {
        li.receiver_name = m[1];
    }
    if (regex_search(splitLine[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))){
        li.dealer_name = m[0];
        }
    else if (li.receiver_name == "Someone") {
        li.dealer_name = "Unknown";  // There will be no dealer in this case.
    }

    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::otherHitByNano(const std::vector<std::string>& splitLine) {
    /*
    Can this crit? Deflect?
    ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
    ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
    */
    LineInfo li;
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("(?:from )(.*?)(?= for)")) ||
        regex_search(splitLine[4], m, regex("(?:of )(.*?)(?= damage)"))) {
        if (m[1] == "unknown") {
            li.dealer_name = "Unknown";
        }
        else {
            li.dealer_name = m[1];
        }
    }
    regex_search(splitLine[4], m, regex("(.*?)(?= was)"));
    li.receiver_name = m[0];
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.nanobots = true;
    return li;
}

LineInfo Parser::youHitOther(const std::vector<std::string>& splitLine) {
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
    LineInfo li;
    std::smatch m;
    li.dealer_name = "You";
    regex_search(splitLine[4], m, regex("(?:hit )(.*?)(?= for)"));
    li.receiver_name = m[1];
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::youHitOtherWithNano(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000005#","You hit other with nano","",1425993792]You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.
    */
    LineInfo li;
    std::smatch m;
    li.dealer_name = "You";
    regex_search(splitLine[4], m, regex("(?:hit )(.*?)(?= with)"));
    li.receiver_name = m[1];
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.nanobots = true;
    return li;
}

LineInfo Parser::meGotHealth(const std::vector<std::string>& splitLine) {
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
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    li.type = "heal";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    if (li.subtype == "potential") { // Potential heal
        regex_search(splitLine[4], m, regex("(?:by )(.*?)(?= for)"));
        li.dealer_name = m[1];
    }
    return li;
}

LineInfo Parser::youGaveHealth(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
    */
    LineInfo li;
    std::smatch m;
    li.dealer_name = "You";
    regex_search(splitLine[4], m, regex("(?:healed )(.*?)(?= for)"));
    li.receiver_name = m[1];
    li.amount = find_amount(splitLine);
    li.type = "heal";
    li.subtype = "potential"; // Find out if the subtype here is actual or potential.
    return li;
}

LineInfo Parser::meHitByMonster(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
    ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
    ["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
    ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.
    ["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
    */
    LineInfo li;
    std::smatch m;
    regex_search(splitLine[4], m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"));  // Match as few chars as possible untill " hit".
    li.dealer_name = m[0];
    li.receiver_name = "You";
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::meHitByNano(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
    */
    LineInfo li;
    std::smatch m;
    regex_search(splitLine[4], m, regex("(?:from )(.*?)(?= for)"));
    li.dealer_name = m[1];
    li.receiver_name = "You";
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.nanobots = true;
    return li;
}

LineInfo Parser::meHitByPlayer(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000007#","Me hit by player","",1434406040]Player Balas hit you for 854 points of projectile damage.
    ["#0000000042000007#","Me hit by player","",1434406024]Balas hit you for 949 points of Aimed Shot damage.
    */
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    if (regex_search(splitLine[4], m, regex("(?:Player )(.*?)(?= hit you for )")) ||
        regex_search(splitLine[4], m, regex("(.*?)(?= hit you for )"))) {
        li.dealer_name = m[1];
    }
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::otherMisses(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000013#","Other misses","",1425326282]Predator Rogue tried to hit you, but missed!
    // Find my own version:
    ["#0000000042000013#","Other misses","",1180557880]Keepone tries to attack you with FastAttack, but misses!
    */
    // The log line is split at "," so everything following the "," is in
    // splitLine[5]
    LineInfo li;
    std::smatch m;
    if (regex_search(splitLine[4], m, regex("(.*?)(?: tried to hit )(.*)")) ||
        regex_search(splitLine[4], m, regex("(.*?)(?: tries to attack )(.*?)(?: with )"))) {
        li.dealer_name = m[1];
        li.receiver_name = m[2];
        if (li.receiver_name == "you") {
            li.receiver_name = "You";
        }
    }
    li.miss = true;
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    renameSpecial(li);  // Specials in misses have a different name
    return li;
}

LineInfo Parser::yourMisses(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000012#","Your misses","",1425666157]You try to attack Peal Thunder with Brawl, but you miss!
    ["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
    */
    LineInfo li;
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(splitLine[4], m, regex("(?:attack )(.*?)(?= with )")) ||
        regex_search(splitLine[4], m, regex("(?:hit )(.*)"))) {
        li.receiver_name = m[1];
    }
    li.miss = true;
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    renameSpecial(li);  // Specials in misses have a different name
    return li;
}

LineInfo Parser::meHitByEnvironment(const std::vector<std::string>& splitLine) {
    /* Need to get my own example line
    ["#0000000042000001#","Me hit by environment","",1160126071]You were damaged by a toxic substance for 123 points of damage.
    */
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    li.dealer_name = "Environment";
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::meCastNano(const std::vector<std::string>& splitLine) {
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
    LineInfo li;
    std::smatch m;
    li.type = "nano_cast";
    li.dealer_name = "You";
    if (regex_search(splitLine[4], m, regex("(?:Program:\\s)(.*?)(?=\\.)"))) {
        li.nanoProgram->setName(m[1]);
        li.subtype = "execute";
    }
    else if (splitLine[4] == "Nano program executed successfully.") {
        li.subtype = "land";
    }
    else if (splitLine[4] == "Target resisted.") {
        li.subtype = "resist";
    }
    else if (splitLine[4] == "Nano program aborted.") {
        li.subtype = "abort";
    }
    else if (splitLine[4] == "Your target countered the nano program.") {
        li.subtype = "counter";
    }
    else if (splitLine[4] == "You fumbled.") {  // Find one of these log messages
        li.subtype = "fumble";
    }
    li.nanoProgram->addStat(li.subtype, 1);
    cout << "Nano casted: " << li.nanoProgram->getName() << endl;
    return li;
}

LineInfo Parser::yourPetHitByNano(const std::vector<std::string>& splitLine) {
    // Find example
    LineInfo li;
    std::smatch m;
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    return li;
}

LineInfo Parser::yourPetHitByMonster(const std::vector<std::string>& splitLine) {
    /* Find my own log line, also find one of these when it this "you" and "other".
    ["#0000000042000011#","Your pet hit by monster","",1191516331]Your pet Vios was damaged by a toxic substance for 25 points of damage.

    There must be other variations of this type too. Find them.
    */
    LineInfo li;
    std::smatch m;
    regex_search(splitLine[4], m, regex("(?:Your pet )(.*?)( was damaged by a )"));
    li.receiver_name = m[1];
    li.dealer_name = "Environment";
    li.type = "damage";
    li.subtype = find_subtype(splitLine);
    li.amount = find_amount(splitLine);
    li.crit = isCrit(splitLine);
    li.deflect = isDeflect(splitLine);
    return li;
}

LineInfo Parser::meGotSK(const std::vector<std::string>& splitLine) {
    /*
    ["#000000004200000c#","Me got SK","",1425993638]You gained 200 points of Shadowknowledge.
    ["#000000004200000c#","Me got SK","",1425994822]You lost 200 points of Shadowknowledge
    */
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    li.type = "xp";
    li.amount = find_amount(splitLine);
    if (regex_search(splitLine[4], m, regex("gained"))) {
        li.subtype = "sk";
    }
    else if (regex_search(splitLine[4], m, regex("lost"))) {
        li.subtype = "sk lost";
    }
    return li;
}

LineInfo Parser::megotXP(const std::vector<std::string>& splitLine) {
    /*
    ["#000000004200000b#","Me got XP","",1426200654]You lost 9822 xp.
    ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
    ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
    */
    LineInfo li;
    std::smatch m;
    std::smatch xp;
    li.receiver_name = "You";
    li.type = "xp";
    li.amount = find_amount(splitLine);
    if (regex_search(splitLine[4], m, regex("gained"))) {
        li.subtype = "aixp";
    }
    else if (regex_search(splitLine[4], m, regex("received"))) {
        li.subtype = "xp";
    }
    else if (regex_search(splitLine[4], m, regex("lost"))) {
        // Find out what an aixp loss looks like
        li.subtype = "xp lost";
    }
    return li;
}

LineInfo Parser::research(const std::vector<std::string>& splitLine) {
    /*
    There is also global research. Ignore it for now.
    ["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
    */
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    li.type = "xp";
    li.subtype = "research";
    li.amount = find_amount(splitLine);
    return li;
}

LineInfo Parser::youGaveNano(const std::vector<std::string>& splitLine) {
    /*
    ["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
    */
    LineInfo li;
    std::smatch m;
    li.dealer_name = "You";
    regex_search(splitLine[4], m, regex("(?:nano on )(.*?)(?= for)"));
    li.receiver_name = m[1];
    li.type = "nano";
    li.amount = find_amount(splitLine);
    return li;
}

LineInfo Parser::meGotNano(const std::vector<std::string>& splitLine) {
    /* Find my own log line, remember to replace in the log file.
    ["#0000000042000016#","Me got nano","",1180555427]You got nano from Jspe80 for 288 points.
    */
    LineInfo li;
    std::smatch m;
    li.receiver_name = "You";
    regex_search(splitLine[4], m, regex("(?:You got nano from )(.*?)(?= for)"));
    li.dealer_name = m[1];
    li.type = "nano";
    li.amount = find_amount(splitLine);
    return li;
}

LineInfo Parser::victoryPoints(const std::vector<std::string>& splitLine) {
    // Check what this message looks like.
    // Maybe it's a system message.
    (void)splitLine;
    LineInfo li;
    return li;
}

LineInfo Parser::system(const std::vector<std::string>& splitLine) {
    (void)splitLine;
    LineInfo li;
    li.hasStats = false;
    return li;
}

LineInfo Parser::chat(const std::vector<std::string>& splitLine) {
    /* Add raid chat?

    ["#0000000040000002#","Vicinity","Sgtcuddle",1436181663]test
    ["Team","Team","Sgtcuddle",1436182391]test
    ["#00000003000011fc#","Pantheon","Sgtcuddle",1436182488]test
    */
    LineInfo li;
    li.hasStats = false;
    std::smatch m;
    if (splitLine[2] == config.getProgramOwnerName() &&
        splitLine[4] == "dd") {
        li.hasCommand = true;
        li.command = splitLine[4];
    }
    return li;
}
