#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "player.h"
#include "line_info.h"

#include <gtest/gtest_prod.h>


class Parser {
public:
    Parser(std::string playerRunningProgram);

    LineInfo parse(FormattedLine& formattedLine);

private:
    //FRIEND_TEST(Parser, Constructor);
    void createFunctionMap();
    bool isCrit(FormattedLine& formattedLine);
    bool isDeflect(FormattedLine& formattedLine);
    int findAmount(FormattedLine& formattedLine);
    std::string findSubtype(FormattedLine& formattedLine);
    void renameSpecial(LineInfo& li);
    void logWhenPlayerNamesNotFound(LineInfo& lineInfo, FormattedLine& formattedLine);

    std::string playerRunningProgram;
    typedef LineInfo (Parser::*pfunc)(FormattedLine& formattedLine);
    std::map<std::string, pfunc> funcMap;

    // Functions within the funcMap
    LineInfo otherAndYourPetHitByOther(FormattedLine& formattedLine);
    LineInfo otherHitByNano(FormattedLine& formattedLine);
    LineInfo youHitOther(FormattedLine& formattedLine);
    LineInfo youHitOtherWithNano(FormattedLine& formattedLine);
    LineInfo meGotHealth(FormattedLine& formattedLine);
    LineInfo youGaveHealth(FormattedLine& formattedLine);
    LineInfo meHitByMonster(FormattedLine& formattedLine);
    LineInfo meHitByEnvironment(FormattedLine& formattedLine);
    LineInfo meHitByPlayer(FormattedLine& formattedLine);
    LineInfo otherMisses(FormattedLine& formattedLine);
    LineInfo yourMisses(FormattedLine& formattedLine);
    LineInfo meHitByNano(FormattedLine& formattedLine);
    LineInfo meCastNano(FormattedLine& formattedLine);
    LineInfo yourPetHitByNano(FormattedLine& formattedLine);
    LineInfo yourPetHitByMonster(FormattedLine& formattedLine);
    LineInfo meGotSK(FormattedLine& formattedLine);
    LineInfo megotXP(FormattedLine& formattedLine);
    LineInfo research(FormattedLine& formattedLine);
    LineInfo youGaveNano(FormattedLine& formattedLine);
    LineInfo meGotNano(FormattedLine& formattedLine);
    LineInfo victoryPoints(FormattedLine& formattedLine);
    LineInfo system(FormattedLine& formattedLine);
    LineInfo chat(FormattedLine& formattedLine);
};

#endif  //  PARSER_H
