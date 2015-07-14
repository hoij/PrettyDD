#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include "player.h"
#include "line_info.h"


class Parser {
public:
    Parser();
    LogLine parse(std::string line);

private:
    void createFunctionMap();
    bool isCrit(const std::vector<std::string>& splitLine);
    bool isDeflect(const std::vector<std::string>& splitLine);
    int find_amount(const std::vector<std::string>& splitLine);
    std::string find_subtype(const std::vector<std::string>& splitLine);
    void renameSpecial(LineInfo& li);
    int find_values(LogLine& logLine);

    typedef LineInfo (Parser::*pfunc)(const std::vector<std::string>& splitLine);
    std::map<std::string, pfunc> funcMap;

    // Functions within the funcMap
    LineInfo otherAndYourPetHitByOther(const std::vector<std::string>& splitLine);
    LineInfo otherHitByNano(const std::vector<std::string>& splitLine);
    LineInfo youHitOther(const std::vector<std::string>& splitLine);
    LineInfo youHitOtherWithNano(const std::vector<std::string>& splitLine);
    LineInfo meGotHealth(const std::vector<std::string>& splitLine);
    LineInfo youGaveHealth(const std::vector<std::string>& splitLine);
    LineInfo meHitByMonster(const std::vector<std::string>& splitLine);
    LineInfo meHitByEnvironment(const std::vector<std::string>& splitLine);
    LineInfo meHitByPlayer(const std::vector<std::string>& splitLine);
    LineInfo otherMisses(const std::vector<std::string>& splitLine);
    LineInfo yourMisses(const std::vector<std::string>& splitLine);
    LineInfo meHitByNano(const std::vector<std::string>& splitLine);
    LineInfo meCastNano(const std::vector<std::string>& splitLine);
    LineInfo yourPetHitByNano(const std::vector<std::string>& splitLine);
    LineInfo yourPetHitByMonster(const std::vector<std::string>& splitLine);
    LineInfo meGotSK(const std::vector<std::string>& splitLine);
    LineInfo megotXP(const std::vector<std::string>& splitLine);
    LineInfo research(const std::vector<std::string>& splitLine);
    LineInfo youGaveNano(const std::vector<std::string>& splitLine);
    LineInfo meGotNano(const std::vector<std::string>& splitLine);
    LineInfo victoryPoints(const std::vector<std::string>& splitLine);
    LineInfo system(const std::vector<std::string>& splitLine);
    LineInfo chat(const std::vector<std::string>& splitLine);
};

#endif
