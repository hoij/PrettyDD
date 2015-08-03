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
#include "formatted_line.h"
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

Parser::Parser(std::string playerRunningProgram) :
    playerRunningProgram(playerRunningProgram) {
    createFunctionMap();
}

void Parser::createFunctionMap() {
    // TODO: Merge similar.
    // TODO: Combine multiple searches into one regex where possible.
    // TODO: Fix so all cases can use find_amout()
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
    funcMap["Me got XP"] = &Parser::meGotXP;
    funcMap["Research"] = &Parser::research;
    funcMap["You gave nano"] = &Parser::youGaveNano;
    funcMap["Me got nano"] = &Parser::meGotNano;
    funcMap["Victory Points"] = &Parser::victoryPoints;
    funcMap["System"] = &Parser::system;
}

LineInfo Parser::parse(FormattedLineInterface& formattedLine) {
    LineInfo lineInfo;

    if (formattedLine.getDescription() == "Vicinity" ||
        formattedLine.getDescription() == "Team" ||
        formattedLine.getDescriptionCode() == "00000003000011fc") {
            // Vicinity, team or org chat messages
            lineInfo = chat(formattedLine.getMessage(), formattedLine.getSender());
        }
    else {
        // Call a matching function from the map of parsing functions.
        auto funcMapIterator = funcMap.find(formattedLine.getDescription());
        if (funcMapIterator != funcMap.end()) {
            lineInfo = (this->*funcMapIterator->second)(formattedLine.getMessage());
            // findSubtype can be called on any line
            lineInfo.subtype = findSubtype(formattedLine.getMessage(), lineInfo.type);
        }
        else {
            // Might want to remove this error message as it could print a lot
            // if the user choses to log many other messages not found in the map
            errorLog.write("No match for description: ", false);
            errorLog.write(formattedLine.getDescription());
            errorLog.write("\tFull line: " + formattedLine.getOriginalLine());
        }
    }

    logWhenPlayerNamesNotFound(lineInfo, formattedLine);

    return lineInfo;
}

int Parser::findAmount(const std::string& message) {
    //if (logLine.getDescription() == "Me Cast Nano" ||
    //    logLine.getDescription() == "System" ||
    //    logLine.getDescription() == "Vicinity" ||
    //    logLine.getDescription() == "Me Cast Nano" ||
    //    logLine.getDescription() == "00000003000011fc" ||) {
    //    return 0;
    //}
    std::smatch d;
    if (regex_search(message, d, regex("(\\d+)( points)"))) {
        return std::stoi(d[1]);
    }
    else if (regex_search(message, d, regex("\\d+\\s"))) {
        // For XP/SK/Reserach but might match some other line I've missed.
        return std::stoi(d[0]);
    }
    else {
        errorLog.write("Amount not found in the following line: ");
        errorLog.write(message);
        return 0;
    }
}

std::string Parser::findDamageSubtype(const std::string& message) {
    /*
    // These lines still exist?
    ["#000000004200000a#","Other hit by other","",1183505123]Something hit Addicted2 for 49 points of damage by reflect shield.
    ["#000000004200000a#","Other hit by other","",1183504118] Something hit Enfodruid for 1 points of damage by damage shield.
    */
    std::smatch m;
    if (regex_search(message, m, regex("(?:points of )(.*?)(?= damage\\.)"))) {
        // Looks for regular and special damage
        return m[1];
    }
    else if (regex_search(message, m, regex("(?: )(.* shield)(?= hit)"))) {
        // Looks for reflect or shield damage
        return m[1];
    }
    else if (regex_search(message, m, regex("(?: with )(.*)(?=, but )"))) {
        // Looks for special damage in case of a miss.
        return renameSpecial(m[1]);  // Specials in misses have a different name
    }
    else if (regex_search(message, m, regex(" tried to hit "))) {
        return "regular miss";
    }
    else if (regex_search(message, m, regex("(?: damaged by )(.*?)( for )"))) {
        return m[1];
    }
    else if (regex_search(message, m, regex("(?: damage by )(.*?)(?=\\.)"))) {
        return m[1];
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findHealSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex(" got healed |You healed "))) {
        return "potential";
    }
    else if (regex_search(message, m, regex(" were healed "))) {
        return "actual";
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findNanoCastSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("Executing Nano Program:"))) {
        return "execute";
    }
    else if (message == "Nano program executed successfully.") {
        return "land";
    }
    else if (message == "Target resisted.") {
        return "resist";
    }
    else if (message == "Nano program aborted.") {
        return "abort";
    }
    else if (message == "Your target countered the nano program.") {
        return "counter";
    }
    else if (message == "You fumbled.") {  // Find one of these log messages
        return "fumble";
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findSKSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("gained"))) {
        return "gained";
    }
    else if (regex_search(message, m, regex("lost"))) {
        return "lost";
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findXPSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("received"))) {
        return "gained";
    }
    else if (regex_search(message, m, regex("lost"))) {
        // Find out what an aixp loss looks like
        return "lost";
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findAIXPSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("gained"))) {
        return "gained";
    }
    else if (regex_search(message, m, regex("lost"))) {
        return "lost";
    }
    else {
        // TODO: Log error.
        return "";
    }
}

std::string Parser::findSubtype(const std::string& message, const std::string type) {
    if (type == "damage") {
        return findDamageSubtype(message);
    }
    else if (type == "nano cast") {
        return findNanoCastSubtype(message);
    }
    else if (type == "heal") {
        return findHealSubtype(message);
    }
    else if (type == "sk") {
        return findSKSubtype(message);
    }
    else if (type == "research") {
        return ""; // No subtype for research.
    }
    else if (type == "xp") {
        return findXPSubtype(message);
    }
    else if (type == "aixp") {
        return findAIXPSubtype(message);
    }
    else if (type == "vp") {
        return "";
    }
    else if (type == "nano") {
        return ""; // No subtype for nano.
    }
    else {
        // TODO: Log error.
        return "";
    }
}

bool Parser::isCrit(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("damage.Critical hit!")))
        return true;
    else
        return false;
}

bool Parser::isDeflect(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("damage. Glancing hit.")))
        return true;
    else
        return false;
}

std::string Parser::renameSpecial(std::string subtype) {
    // TODO: Are there any more of these? Bow Special?
    // Misses writes specials to the log different so they have to be renamed.
    // I can either rename these here, or when I get the data from a player,
    // I can get both "Brawl" and "Brawling" for example.
    if (subtype == "regular miss") {
        return subtype;
    }
    else if (subtype == "FastAttack") {
        return "Fast Attack";
    }
    else if (subtype == "FlingShot") {
        return "Fling Shot";
    }
    else if (subtype == "Brawl") {
        return "Brawling";
    }
    else if (subtype == "FullAuto") {
        return "Full Auto";
    }
    else {
        errorLog.write("Tried to rename special called \"" + subtype, false);
        errorLog.write("\", but no match was found.");
        return subtype;
    }
}

void Parser::logWhenPlayerNamesNotFound(LineInfo& lineInfo, FormattedLineInterface& formattedLine) {
    // TODO: Remove at some point.
    // For development purposes only.
    // Just to capture anything I might have missed.
    if (lineInfo.dealer_name == "" && lineInfo.receiver_name == "") {
        errorLog.write("Could not find dealer and receiver name in the following line (Note: This may be normal): ");
        errorLog.write("\t" + formattedLine.getOriginalLine());
    }
    else if (lineInfo.dealer_name == "") {
        errorLog.write("Could not find dealer name in the following line (Note: This may be normal): ");
        errorLog.write("\t" + formattedLine.getOriginalLine());
    }
    else if (lineInfo.receiver_name == "") {
        errorLog.write("Could not find receiver name in the following line (Note: This may be normal): ");
        errorLog.write("\t" + formattedLine.getOriginalLine());
    }
}

//////////////////////////////////////////
//                                      //
//  Functions within the function map:  //
//                                      //
//////////////////////////////////////////

LineInfo Parser::otherAndYourPetHitByOther(const std::string& message) {
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
    li.type = "damage";
    std::smatch m;
    if (regex_search(message, m, regex("(?:hit )"	    // Find "hit ", but do not include it in the results
                                        "(.*?)"			// match everything following, non-greedy
                                                        // i.e. until first occurrence, of
                                        "(?= for)"))) {	// " for"
        li.receiver_name = m[1];
    }
    else if (regex_search(message, m, regex("(.*?)(?= absorbed )"))) {
        li.receiver_name = m[1];
    }
    if (regex_search(message, m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))){
        if (m[0] == "Something") {
            li.dealer_name = "Unknown";
        }
        else {
            li.dealer_name = m[0];
        }
    }
    else if (li.receiver_name == "Someone") {
        li.dealer_name = "Unknown";  // There will be no dealer in this case.
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::otherHitByNano(const std::string& message) {
    /*
    Can this crit? Deflect?
    ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
    ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
    */
    LineInfo li;
    li.type = "damage";
    std::smatch m;
    if (regex_search(message, m, regex("(?:from )(.*?)(?= for)")) ||
        regex_search(message, m, regex("(?:of )(.*?)(?= damage)"))) {
        if (m[1] == "unknown") {
            li.dealer_name = "Unknown";
        }
        else {
            li.dealer_name = m[1];
        }
    }
    if (regex_search(message, m, regex("(.*?)(?= was attacked)"))) {
        li.receiver_name = m[0];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    li.nanobots = true;
    return li;
}

LineInfo Parser::youHitOther(const std::string& message) {
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
    li.type = "damage";
    li.dealer_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:hit )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::youHitOtherWithNano(const std::string& message) {
    /*
    ["#0000000042000005#","You hit other with nano","",1425993792]You hit Kyr'Ozch Hive Medic with nanobots for 798 points of projectile damage.
    */
    LineInfo li;
    li.type = "damage";
    li.dealer_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:hit )(.*?)(?= with)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    // TODO: Can this deflect?
    li.nanobots = true;
    return li;
}

LineInfo Parser::meGotHealth(const std::string& message) {
    /*
    ["#0000000042000015#","Me got health","",1425326282]You were healed for 193 points.
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
    li.type = "heal";
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:by )(.*?)(?= for )"))) {
        // This can only happen when the heal is a potential heal.
        li.dealer_name = m[1];
    }
    // else no dealer.
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::youGaveHealth(const std::string& message) {
    // TODO: Find out if the subtype here is actual or potential. Pretty sure it's potential.
    /*
    ["#0000000042000014#","You gave health","",1425998482]You healed Letter for 3741 points of health.
    */
    LineInfo li;
    li.type = "heal";
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:healed )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::meHitByMonster(const std::string& message) {
    /*
    ["#0000000042000006#","Me hit by monster","",1425326284]Predator Rogue hit you for 306 points of melee damage.
    ["#0000000042000006#","Me hit by monster","",1425326287]Predator Rogue hit you for 717 points of melee damage.Critical hit!
    ["#0000000042000006#","Me hit by monster","",1434406044]Someone's reflect shield hit you for 121 points of damage.
    ["#0000000042000006#","Me hit by monster","",1434406044]Someone's damage shield hit you for 132 points of damage.
    ["#0000000042000006#","Me hit by monster","",1434359357]Assembler Leader hit you for 1966 points of melee damage. Glancing hit.
    */
    LineInfo li;
    li.type = "damage";
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))) {  // The order here is important.
        li.dealer_name = m[0];
        if (li.dealer_name == "Someone") {
            li.dealer_name = "Unknown";
        }
    }
    else {
        errorLog.write("Could not find a dealer in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::meHitByNano(const std::string& message) {
    /*
    ["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
    */
    LineInfo li;
    li.type = "damage";
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:from )(.*?)(?= for)"))) {
        li.dealer_name = m[1];
    }
    else {
        errorLog.write("Could not find a dealer in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    li.nanobots = true;
    return li;
}

LineInfo Parser::meHitByPlayer(const std::string& message) {
    /*
    ["#0000000042000007#","Me hit by player","",1434406040]Player Balas hit you for 854 points of projectile damage.
    ["#0000000042000007#","Me hit by player","",1434406024]Balas hit you for 949 points of Aimed Shot damage.
    */
    LineInfo li;
    li.type = "damage";
    std::smatch m;
    li.receiver_name = "You";
    if (regex_search(message, m, regex("(?:Player )(.*?)(?= hit you for )")) ||
        regex_search(message, m, regex("(.*?)(?= hit you for )"))) {
        li.dealer_name = m[1];
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::otherMisses(const std::string& message) {
    /*
    ["#0000000042000013#","Other misses","",1425326282]Predator Rogue tried to hit you, but missed!
    // Find my own version:
    ["#0000000042000013#","Other misses","",1180557880]Keepone tries to attack you with FastAttack, but misses!
    */
    LineInfo li;
    li.type = "damage";
    std::smatch m;
    if (regex_search(message, m, regex("(.*?)(?: tried to hit )(.*)(?:, but missed!)")) ||
        regex_search(message, m, regex("(.*?)(?: tries to attack )(.*?)(?: with )"))) {
        li.dealer_name = m[1];
        li.receiver_name = m[2];
        if (li.receiver_name == "you") {
            li.receiver_name = "You";
        }
    }
    li.miss = true;
    return li;
}

LineInfo Parser::yourMisses(const std::string& message) {
    /*
    ["#0000000042000012#","Your misses","",1425666157]You try to attack Peal Thunder with Brawl, but you miss!
    ["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
    */
    LineInfo li;
    li.type = "damage";
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:attack )(.*?)(?= with )")) ||
        regex_search(message, m, regex("(?:hit )(.*)(?:, but missed!)"))) {
        li.receiver_name = m[1];
    }
    li.miss = true;
    return li;
}

LineInfo Parser::meHitByEnvironment(const std::string& message) {
    /* Need to get my own example line
    ["#0000000042000001#","Me hit by environment","",1160126071]You were damaged by a toxic substance for 123 points of damage.
    */
    LineInfo li;
    li.type = "damage";
    li.receiver_name = "You";
    li.dealer_name = "Environment";
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::meCastNano(const std::string& message) {
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
    li.type = "nano cast";
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:Program:\\s)(.*?)(?=\\.)"))) {
        li.nanoProgramName = m[1];
    }
    return li;
}

LineInfo Parser::yourPetHitByNano(const std::string& message) {
    // Find example
    LineInfo li;
    li.type = "damage";
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    return li;
}

LineInfo Parser::yourPetHitByMonster(const std::string& message) {
    /* Find my own log line, also find one of these when it this "you" and "other".
    ["#0000000042000011#","Your pet hit by monster","",1191516331]Your pet Vios was damaged by a toxic substance for 25 points of damage.

    There must be other variations of this type too. Find them.
    */
    LineInfo li;
    li.type = "damage";
    li.dealer_name = "Environment";
    std::smatch m;
    if (regex_search(message, m, regex("(?:Your pet )(.*?)( was damaged by a )"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::meGotSK(const std::string& message) {
    /*
    ["#000000004200000c#","Me got SK","",1425993638]You gained 200 points of Shadowknowledge.
    ["#000000004200000c#","Me got SK","",1425994822]You lost 200 points of Shadowknowledge
    */
    LineInfo li;
    li.type = "sk";
    li.receiver_name = "You";
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::meGotXP(const std::string& message) {
    /*
    ["#000000004200000b#","Me got XP","",1426200654]You lost 9822 xp.
    ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
    ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
    */
    LineInfo li;
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("Alien Experience"))) {
        li.type = "aixp";
    }
    else {
        li.type = "xp";
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::research(const std::string& message) {
    /*
    There is also global research. Ignore it for now.
    ["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
    */
    LineInfo li;
    li.type = "research";
    li.receiver_name = "You";
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::youGaveNano(const std::string& message) {
    /*
    ["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
    */
    LineInfo li;
    li.type = "nano";
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:nano on )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Could not find a receiver in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::meGotNano(const std::string& message) {
    /* Find my own log line, remember to replace in the log file.
    ["#0000000042000016#","Me got nano","",1180555427]You got nano from Jspe80 for 288 points.
    */
    LineInfo li;
    li.type = "nano";
    std::smatch m;
    li.receiver_name = "You";
    if (regex_search(message, m, regex("(?:You got nano from )(.*?)(?= for)"))) {
        li.dealer_name = m[1];
    }
    else {
        errorLog.write("Could not find a dealer in: ");
        errorLog.write("\t" + message);
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::victoryPoints(const std::string& message) {
    // Check what this message looks like.
    // Maybe it's a system message.
    LineInfo li;
    li.type = "vp";
    (void)message;
    return li;
}

LineInfo Parser::system(const std::string& message) {
    (void)message;
    LineInfo li;
    li.hasStats = false;
    return li;
}

LineInfo Parser::chat(const std::string& message, const std::string& sender) {
    // Reads chat for commands made by the player running the program
    /* Add raid chat?

    ["#0000000040000002#","Vicinity","Sgtcuddle",1436181663]test
    ["Team","Team","Sgtcuddle",1436182391]test
    ["#00000003000011fc#","Pantheon","Sgtcuddle",1436182488]test
    */
    LineInfo li;
    li.hasStats = false;
    std::smatch m;
    if (sender == playerRunningProgram &&
        message == "dd") {
        li.command = message;
    }
    return li;
}
