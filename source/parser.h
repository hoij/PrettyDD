#ifndef PARSER_H
#define PARSER_H


#include <gtest/gtest_prod.h>
#include <map>
#include <string>
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
    int findAmount(const std::string& message);
    std::string findSubtype(const std::string& message, const std::string type);
    std::string findDamageSubtype(const std::string& message);
    std::string findHealSubtype(const std::string& message);
    std::string findNanoCastSubtype(const std::string& message);
    std::string findSKSubtype(const std::string& message);
    std::string findXPSubtype(const std::string& message);
    std::string findAIXPSubtype(const std::string& message);
    std::string renameSpecial(std::string subtype);

    void logWhenPlayerNamesNotFound(LineInfo& lineInfo, FormattedLineInterface& formattedLine);

    std::string playerRunningProgram;
    typedef LineInfo (Parser::*pfunc)(const std::string& message);
    std::map<std::string, pfunc> funcMap;

    // Functions within the funcMap
    LineInfo otherAndYourPetHitByOther(const std::string& message);
    LineInfo otherHitByNano(const std::string& message);
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
    LineInfo yourPetHitByNano(const std::string& message);
    LineInfo yourPetHitByMonster(const std::string& message);
    LineInfo meGotSK(const std::string& message);
    LineInfo meGotXP(const std::string& message);
    LineInfo research(const std::string& message);
    LineInfo youGaveNano(const std::string& message);
    LineInfo meGotNano(const std::string& message);
    LineInfo victoryPoints(const std::string& message);
    LineInfo system(const std::string& message);
    LineInfo chat(const std::string& message, const std::string& sender);
};


#endif  //  PARSER_H
