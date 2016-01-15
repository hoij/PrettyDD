#include "formatted_line_interface.h"
#include "line_info.h"
#include "logger.h"
#include "parser.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <regex>


using std::cout;
using std::endl;
using std::regex_search;
using std::regex;


Parser::Parser(std::string playerRunningProgram) :
    playerRunningProgram(playerRunningProgram) {
    createFunctionMap();

    specials.emplace("Aimed Shot");
    specials.emplace("Brawling");
    specials.emplace("Backstab");
    specials.emplace("Burst");
    specials.emplace("Dimach");
    specials.emplace("Fast Attack");
    specials.emplace("Fling Shot");
    specials.emplace("Full Auto");
    specials.emplace("Sneak Atck");
}

void Parser::createFunctionMap() {
    // TODO: Merge similar.
    // TODO: Combine multiple searches into one regex where possible.
    // TODO: Fix so all cases can use find_amout()
    funcMap["Other hit by other"] = &Parser::otherAndYourPetHitByOther;
    funcMap["Your pet hit by other"] = &Parser::otherAndYourPetHitByOther;
    funcMap["Other hit by nano"] = &Parser::otherAndYourPetHitByNano;
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
    funcMap["Your pet hit by nano"] = &Parser::otherAndYourPetHitByNano;
    funcMap["Your pet hit by monster"] = &Parser::yourPetHitByMonster;
    funcMap["Me got SK"] = &Parser::meGotSK;
    funcMap["Me got XP"] = &Parser::meGotXP;
    funcMap["Research"] = &Parser::research;
    funcMap["You gave nano"] = &Parser::youGaveNano;
    funcMap["Me got nano"] = &Parser::meGotNano;
    funcMap["System"] = &Parser::system;
}

LineInfo Parser::parse(FormattedLineInterface& formattedLine) {
    LineInfo lineInfo;

    if (formattedLine.getDescription() == "Vicinity" ||
        formattedLine.getDescription() == "Team" ||
        formattedLine.getDescriptionCode() == "00000003000011fc" ||  // Org
        formattedLine.getDescription() == "Raid") {
            lineInfo = chat(formattedLine.getMessage(), formattedLine.getSender());
        }
    else {
        // Call a matching function from the map of parsing functions.
        auto funcMapIterator = funcMap.find(formattedLine.getDescription());
        if (funcMapIterator != funcMap.end()) {
            lineInfo = (this->*funcMapIterator->second)(formattedLine.getMessage());
            // findSubtype can be called on any line
            lineInfo.subtype = findSubtype(formattedLine.getMessage(), lineInfo.type);
            // Set special or shield flag
            if (lineInfo.type == LineType::damage) {
                lineInfo.special = isSpecial(lineInfo.subtype);
                lineInfo.shield = isShield(lineInfo.subtype);
            }
        }
        else {
            // Might want to remove this error message as it could print a lot
            // if the user choses to log many other messages not found in the map
            errorLog.write("Warning: No match for description: " +
                           formattedLine.getDescription());
            errorLog.write("Warning: Full line: " + formattedLine.getOriginalLine());
        }
    }

    lineInfo.time = formattedLine.getTime();

    return lineInfo;
}

int Parser::findAmount(const std::string& message) {
    std::smatch d;
    if (regex_search(message, d, regex("(\\d+)( points)"))) {
        return std::stoi(d[1]);
    }
    else if (regex_search(message, d, regex("\\d+\\s"))) {
        // For XP/SK/Reserach/PVP Score but might match some other line
        // I've missed.
        return std::stoi(d[0]);
    }
    else if (regex_search(message, d, regex("(?::\\s)(\\d+)"))) {
        return std::stoi(d[1]);
    }
    else {
        errorLog.write("Error: Amount not found in: ");
        errorLog.write("Error: Message: " + message);
        return 0;
    }
}

std::string Parser::findSubtype(const std::string& message, LineType type) {
    if (type == LineType::damage) {
        return findDamageSubtype(message);
    }
    else if (type == LineType::nanoCast) {
        return findNanoCastSubtype(message);
    }
    else if (type == LineType::heal) {
        return findHealSubtype(message);
    }
    else if (type == LineType::sk) {
        return findSKSubtype(message);
    }
    else if (type == LineType::research) {
        return "gained"; // Can't lose research.
    }
    else if (type == LineType::xp) {
        return findXPSubtype(message);
    }
    else if (type == LineType::aixp) {
        return findAIXPSubtype(message);
    }
    else if (type == LineType::vp) {
        return "gained";
    }
    else if (type == LineType::pvpSoloScore ||
             type == LineType::pvpTeamScore) {
        return "gained";
    }
    else if (type == LineType::nano) {
        return ""; // No subtype for nano.
    }
    else if (type == LineType::notAvailable) {
        return "";
    }
    else {
        errorLog.write("Error: Could not find a type in:");
        errorLog.write("Error: Message: " + message);
        return "";
    }
}

std::string Parser::findDamageSubtype(const std::string& message) {
    /*
    // TODO: These lines still exist?
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
        return renameSpecial(m[1]);  // Some specials in misses have a different name
    }
    else if (regex_search(message, m, regex(" tried to hit "))) {
        return "Regular Miss";
    }
    else if (regex_search(message, m, regex("(?: damaged by )(.*?)( for )"))) {
        return m[1];
    }
    else if (regex_search(message, m, regex("(?: damage by )(.*?)(?=\\.)"))) {
        return m[1];
    }
    else {
        errorLog.write("Error: Could not find a damage subtype in:");
        errorLog.write("Error: Message: " + message);
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
        errorLog.write("Error: Could not find a heal subtype in:");
        errorLog.write("Error: Message: " + message);
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
    // TODO: Find one of these log messages
    else if (message.find("You fumbled.") != std::string::npos) {
        return "fumble";
    }
    // "Target does not have enough nano controlling units(NCU) left."
    else if (message.find("Target does not") != std::string::npos) {
        return "fullncu";
    }

    /* Once the nano has executed a message about the status of the
    execution will arrive. These message should therefore not be saved */
    // "Wait for current nano program execution to finish."
    else if (message.find("Wait") != std::string::npos) {
        return "";
    }
    // "Nano program failed. Already executing nanoprogram."
    else if (message.find("Nano program failed") != std::string::npos) {
        return "";
    }

    /* The messages below can be ignored (not added to stats) as they
    are not preceded by an "Executing Nano Program:" message. */
    // "NCU error: Better nano program already running."
    // "NCU error: This nano program can't automatically replace other program."
    else if (message.find("NCU error") != std::string::npos) {
        return "";
    }
    // "Unable to execute nano program. You can't execute this nano on the target."
    else if (message.find("Unable") != std::string::npos) {
        return "";
    }
    // "Executing programs is currently unavailable."
    else if (message.find("Executing") != std::string::npos) {
        return "";
    }
    // "You need at least 1983 remaining nano energy to execute this program."
    else if (message.find("You need") != std::string::npos) {
        return "";
    }
    // "Target is outside range."
    else if (message.find("Target is") != std::string::npos) {
        return "";
    }
    // "Target out of range for nano program."
    else if (message.find("Target out") != std::string::npos) {
        return "";
    }
    // "Not allowed to execute friendly nanoprogram on anyone on the opposite side of your team in the conflict."
    else if (message.find("Not") != std::string::npos) {
        return "";
    }
    // "You must be standing up to execute a nano program."
    else if (message.find("You must") != std::string::npos) {
        return "";
    }

    // "You can't execute nano programs on items."
    // This message is preceeded by "Executing Nano Program:"
    // so a "clear" subtype is passed on so that NanoPrograms
    // can clear the previous message.
    else if (message.find("You can't") != std::string::npos) {
        return "clear";
    }

    else {
        errorLog.write("Error: Could not find a nano cast subtype in:");
        errorLog.write("Error: " + message);
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
        errorLog.write("Error: Could not find a SK subtype in:");
        errorLog.write("Error: Message: " + message);
        return "";
    }
}

std::string Parser::findXPSubtype(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("received"))) {
        return "gained";
    }
    else if (regex_search(message, m, regex("lost"))) {
        // TODO: Find out what an aixp loss looks like
        return "lost";
    }
    else {
        errorLog.write("Error: Could not find an XP subtype in:");
        errorLog.write("Error: " + message);
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
        errorLog.write("Error: Could not find an AIXP subtype in:");
        errorLog.write("Error: Message: " + message);
        return "";
    }
}

bool Parser::isCrit(const std::string& message) {
    std::smatch m;
    if (regex_search(message, m, regex("damage. Critical hit!|damage.Critical hit!")))
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
    // TODO: Are there any more of these?
    // Misses writes specials to the log different so they have to be renamed.
    // I can either rename these here, or when I get the data from a player,
    // I can get both "Brawl" and "Brawling" for example.
    if (subtype == "FastAttack") {
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
        return subtype;
    }
}

bool Parser::isShield(std::string& subtype) {
    if (subtype == "reflect shield" ||
        subtype == "damage shield") {
        return true;
    }
    return false;
}

bool Parser::isSpecial(std::string& subtype) {
    return !(specials.find(subtype) == specials.end());
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
    li.type = LineType::damage;
    std::smatch m;
    // Find receiver
    if (regex_search(message, m, regex("(?:hit )"	    // Find "hit ", but do not include it in the results
                                        "(.*?)"			// match everything following, non-greedy
                                                        // i.e. until first occurrence, of
                                        "(?= for)"))) {	// " for"
        li.receiver_name = m[1];
    }
    else if (regex_search(message, m, regex("(.*?)(?= absorbed )"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
    }
    // Find dealer
    if (regex_search(message, m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))){
        if (m[0] == "Something") {
            li.dealer_name = "Unknown";
        }
        else {
            li.dealer_name = m[0];
        }
    }
    else if (li.receiver_name == "Someone") {  // There will be no dealer in this message
        li.dealer_name = "Unknown"; 
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
    }

    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::otherAndYourPetHitByNano(const std::string& message) {
    /*
    ["#0000000042000004#","Other hit by nano","",1425326284]Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.
    ["#0000000042000004#","Other hit by nano","",1425326326]Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.
    ["#0000000042000004#","Other hit by nano","",1444750507]Xan Spirit of Redemption was attacked with nanobots for 2162 points of unknown damage.

    ["#0000000042000003#", "Your pet hit by nano", "", 1447090410]Letter was attacked with nanobots from Tekory for 1339 points of chemical damage.
    ["#0000000042000003#", "Your pet hit by nano", "", 1447090411]Letter was attacked with nanobots from Tekory for 175 points of melee damage.
    Sometimes there is no dealer:
    ["#0000000042000004#","Other hit by nano","",1444741138]Blackbullet0 was attacked with nanobots for 715 points of poison damage.
    */
    LineInfo li;
    li.type = LineType::damage;
    std::smatch m;
    // Find dealer
    if (regex_search(message, m, regex("(?:from )(.*?)(?= for)"))) {
        li.dealer_name = m[1];
    }
    else if (regex_search(message, m, regex("was attacked with nanobots"))) {
        li.dealer_name = "Unknown";
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
    }
    // Find receiver
    if (regex_search(message, m, regex("(.*?)(?= was attacked)"))) {
        li.receiver_name = m[0];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::damage;
    li.dealer_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:hit )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::damage;
    li.dealer_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:hit )(.*?)(?= with)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.amount = findAmount(message);
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
    li.type = LineType::heal;
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
    li.type = LineType::heal;
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:healed )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::damage;
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(.*?)(?='s reflect shield |'s damage shield | hit)"))) {  // The order here is important.
        li.dealer_name = m[0];
        if (li.dealer_name == "Someone") {
            li.dealer_name = "Unknown";
        }
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.amount = findAmount(message);
    li.crit = isCrit(message);
    li.deflect = isDeflect(message);
    return li;
}

LineInfo Parser::meHitByNano(const std::string& message) {
    /*
    ["#0000000042000002#","Me hit by nano","",1425326283]You were attacked with nanobots from Predator Rogue for 875 points of poison damage.
    Sometimes there is no dealer:
    ["#0000000042000002#","Me hit by nano","",1444742987]You were attacked with nanobots for 9 points of unknown damage.
    ["#0000000042000002#","Me hit by nano","",1444855910]You were attacked with nanobots for 56 points of energy damage.
    */
    LineInfo li;
    li.type = LineType::damage;
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("(?:from )(.*?)(?= for)"))) {
        li.dealer_name = m[1];
    }
    else if (regex_search(message, m, regex("nanobots for"))) {
        li.dealer_name = "Unknown";
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::damage;
    std::smatch m;
    li.receiver_name = "You";
    if (regex_search(message, m, regex("(?:Player )(.*?)(?= hit you for )")) ||
        regex_search(message, m, regex("(.*?)(?= hit you for )"))) {
        li.dealer_name = m[1];
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::damage;
    std::smatch m;
    if (regex_search(message, m, regex("(.*?)(?: tried to hit )(.*)(?:, but missed!)")) ||
        regex_search(message, m, regex("(.*?)(?: tries to attack )(.*?)(?: with )"))) {
        li.dealer_name = m[1];
        li.receiver_name = m[2];
        if (li.receiver_name == "you") {
            li.receiver_name = "You";
        }
    }
    else {
        errorLog.write("Error: Neither dealer nor receiver found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.miss = true;
    return li;
}

LineInfo Parser::yourMisses(const std::string& message) {
    /*
    ["#0000000042000012#","Your misses","",1425666157]You try to attack Peal Thunder with Brawl, but you miss!
    ["#0000000042000012#","Your misses","",1444856536]You try to attack Majicninja with Burst, but you miss!
    ["#0000000042000012#","Your misses","",1426199923]You tried to hit Stim Fiend, but missed!
    */
    LineInfo li;
    li.type = LineType::damage;
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:attack )(.*?)(?= with )")) ||
        regex_search(message, m, regex("(?:hit )(.*)(?:, but missed!)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.miss = true;
    return li;
}

LineInfo Parser::meHitByEnvironment(const std::string& message) {
    /* Need to get my own example line
    ["#0000000042000001#","Me hit by environment","",1160126071]You were damaged by a toxic substance for 123 points of damage.
    */
    LineInfo li;
    li.type = LineType::damage;
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

    This nano will execute twice. It will cast the HP buff, then heal.
    ["#0000000042000018#","Me Cast Nano","",1434562392]Executing Nano Program: Team Improved Life Channeler.
    ["#0000000042000018#","Me Cast Nano","",1434562393]Nano program executed successfully.
    ["#0000000042000015#","Me got health","",1434562393]You were healed for 3025 points.
    ["#0000000042000018#","Me Cast Nano","",1434562393]Nano program executed successfully.

    Randomly this message can appear. Possibly when a proc fires.
    ["#0000000042000018#","Me Cast Nano","",1442506417]Nano program executed successfully.
    */
    LineInfo li;
    li.type = LineType::nanoCast;
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:Program:\\s)(.*?)(?=\\.)"))) {
        li.nanoProgramName = m[1];
    }
    return li;
}

LineInfo Parser::yourPetHitByMonster(const std::string& message) {
    /* TODO: Find one of these when it's "other". Toxic damage on other does not show in log.
    TODO: There must be other variations of this type too. Find them.
    ["#0000000042000011#","Your pet hit by monster","",1447094231]Your pet Letter was damaged by a toxic substance for 3 points of damage.
    */
    LineInfo li;
    li.type = LineType::damage;
    li.dealer_name = "Environment";
    std::smatch m;
    if (regex_search(message, m, regex("(?:Your pet )(.*?)( was damaged by a )"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
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
    li.type = LineType::sk;
    li.receiver_name = "You";
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::meGotXP(const std::string& message) {
    /*
    ["#000000004200000b#","Me got XP","",1426200654]You lost 9822 xp.
    ["#000000004200000b#","Me got XP","",1426199427]You received 247 xp.
    ["#000000004200000b#","Me got XP","",1425993638]You gained 2562 new Alien Experience Points.
    ["#000000004200000b#","Me got XP","",1444740647]You gained 100 PVP Solo Score.
    ["#000000004200000b#","Me got XP","",1445100355]You gained 101 PVP Team Score.
    */
    LineInfo li;
    li.receiver_name = "You";
    std::smatch m;
    if (regex_search(message, m, regex("Alien Experience"))) {
        li.type = LineType::aixp;
    }
    else if (regex_search(message, m, regex("Solo Score"))) {
        li.type = LineType::pvpSoloScore;
    }
    else if (regex_search(message, m, regex("Team Score"))) {
        li.type = LineType::pvpTeamScore;
    }
    else {
        li.type = LineType::xp;
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::research(const std::string& message) {
    /*
    ["#000000004200001c#","Research","",1425326289]139139 of your XP were allocated to your personal research.<br>
    ["#000000004200001c#","Research","",1444757023]You have completed your research on "Force Recon 9".
    */
    LineInfo li;
    std::smatch m;
    if (regex_search(message, m, regex("personal"))) {
        li.type = LineType::research;
        li.receiver_name = "You";
        li.amount = findAmount(message);
    }
    else {
        // Completed a research line or gained global research
        li.type = LineType::notAvailable;
    }
    return li;
}

LineInfo Parser::youGaveNano(const std::string& message) {
    /*
    ["#0000000042000017#","You gave nano","",1425734907]You increased nano on Sayet for 2102 points.
    */
    LineInfo li;
    li.type = LineType::nano;
    std::smatch m;
    li.dealer_name = "You";
    if (regex_search(message, m, regex("(?:nano on )(.*?)(?= for)"))) {
        li.receiver_name = m[1];
    }
    else {
        errorLog.write("Error: No receiver found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::meGotNano(const std::string& message) {
    /* Find my own log line, remember to replace in the log file.
    ["#0000000042000016#","Me got nano","",1180555427]You got nano from Jspe80 for 288 points.
    */
    LineInfo li;
    li.type = LineType::nano;
    std::smatch m;
    li.receiver_name = "You";
    if (regex_search(message, m, regex("(?:You got nano from )(.*?)(?= for)"))) {
        li.dealer_name = m[1];
    }
    else {
        errorLog.write("Error: No dealer found in: ");
        errorLog.write("Error:\t" + message);
    }
    li.amount = findAmount(message);
    return li;
}

LineInfo Parser::system(const std::string& message) {
    /*
    TODO: Fix. This currently has no use.

    ["#0000000040000001#", "System", "", 1442506644]Muwe executes Bodily Invigoration within your NCU...

    ["#0000000040000001#","System","",1442506292]You hit with 29 bullets...
    Followed by:
    ["#0000000042000008#","You hit other","",1442506292]You hit Corrupted Xan-Kuir for 15000 points of Full Auto damage.

    Note space before .
    ["#0000000040000001#","System","",1442506297]You successfully perform Supressive Horde .
    ["#0000000040000001#","System","",1442506339]You successfully perform Reinforce Slugs.
    ["#0000000040000001#","System","",1442506369]You successfully perform a Jarring Burst attack.

    ["#0000000040000001#", "System", "", 1443609378]New Victory Points gained : 95.
    */
    LineInfo li;
    li.type = LineType::notAvailable;
    std::smatch m;
    if (regex_search(message, m, regex("You hit |You Successfully "))) {
        li.dealer_name = "You";
    }
    else if (regex_search(message, m, regex("(.*?)(?: executes )(.*?)(?= within)"))) {
        li.receiver_name = "You";
        li.dealer_name = m[1];
        li.nanoProgramName = m[2];
    }

    /* TODO: Log victory points
    else if (regex_search(message, m, regex("Victory Points"))) {
    // TODO: Find out why these messages are different. Maybe it's
    // todo with quest reward, double clicking pvp vp item to pick it up,
    // pressing E to pick it up, killing notum miner or looting VP rewards
    // from AI missions.
    When clicking a VP reward, this is the message (really? with a space?):
    ["#0000000040000001#", "System", "", 1443609378]New Victory Points gained : 95.
    Other variants:
    ["#0000000040000001#","System","",1444744688]New Victory Points gained: 95.
    ["#0000000040000001#","System","",1444856587]New Victory Points gained.

        li.receiver_name = "You";
        li.type = LineType::vp;
        li.subtype = "gained";
        li.amount = findAmount(message);
        li.hasStats = true;
    }
    */
    li.hasStats = false;
    return li;
}

LineInfo Parser::chat(const std::string& message, const std::string& sender) {
    /* Reads chat for commands made by the player running the program.
    ["#0000000040000002#","Vicinity","Sgtcuddle",1436181663]test
    ["Team","Team","Sgtcuddle",1436182391]test
    ["#00000003000011fc#","Pantheon","Sgtcuddle",1436182488]test
    ["Raid","Raid","Sgtcuddle",1445781089]yolo
    */
    LineInfo li;
    li.hasStats = false;
    std::smatch m;
    if (sender == playerRunningProgram &&
           (message.compare(0, 4, "pdd ") == 0 ||  // first chars == "pdd "
            message == "pdd")) {
        li.command = message;
    }
    return li;
}
