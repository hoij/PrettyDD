#ifndef PARSER_H
#define PARSER_H


#include "definitions.h"

#include <gtest/gtest_prod.h>
#include <map>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>


class FormattedLineInterface;
class LineInfo;


class Parser {
public:
    Parser(std::string playerRunningProgram);

    LineInfo parse(FormattedLineInterface& formattedLine);

private:
    FRIEND_TEST(RenameSpecialTest, renameSpecials);
    FRIEND_TEST(ParseChatTest, parseChat);

    void createFunctionMap();

    bool isCrit(const std::string& message);
    bool isDeflect(const std::string& message);
    bool isShield(std::string& subtype);
    bool isSpecial(std::string& subtype);
    int findAmount(const std::string& message);
    std::string findSubtype(const std::string& message, LineType type);
    std::string findDamageSubtype(const std::string& message);
    std::string findHealSubtype(const std::string& message);
    std::string findNanoCastSubtype(const std::string& message);
    std::string findSKSubtype(const std::string& message);
    std::string findXPSubtype(const std::string& message);
    std::string findAIXPSubtype(const std::string& message);
    std::string renameSpecial(std::string subtype);

    std::string playerRunningProgram;
    typedef LineInfo (Parser::*pfunc)(const std::string& message);
    std::map<std::string, pfunc> funcMap;

    std::unordered_set<std::string> specials;

    // Functions within the funcMap
    LineInfo otherAndYourPetHitByOther(const std::string& message);
    LineInfo otherAndYourPetHitByNano(const std::string& message);
    LineInfo youHitOther(const std::string& message);
    LineInfo youHitOtherWithNano(const std::string& message);
    LineInfo meGotHealth(const std::string& message);
    LineInfo youGaveHealth(const std::string& message);
    LineInfo meHitByMonster(const std::string& message);
    LineInfo meHitByEnvironment(const std::string& message);
    LineInfo meHitByPlayer(const std::string& message);
    LineInfo otherMisses(const std::string& message);
    LineInfo yourMisses(const std::string& message);
    LineInfo meHitByNano(const std::string& message);
    LineInfo meCastNano(const std::string& message);
    LineInfo yourPetHitByMonster(const std::string& message);
    LineInfo meGotSK(const std::string& message);
    LineInfo meGotXP(const std::string& message);
    LineInfo research(const std::string& message);
    LineInfo youGaveNano(const std::string& message);
    LineInfo meGotNano(const std::string& message);
    LineInfo victoryPoints(const std::string& message);
    LineInfo system(const std::string& message);
    LineInfo chat(const std::string& message, const std::string& sender);

    enum class regexNames {
        nr_points,
        nr_,
        colon_nr,
        hit_name_for,
        name_absorbed,
        name_shield_hit,
        crit,
        deflect
    };

    std::map<regexNames, std::regex> regexs;
};


#endif  //  PARSER_H
