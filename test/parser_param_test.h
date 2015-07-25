#ifndef PARSER_PARAM_TEST_H
#define PARSER_PARAM_TEST_H

#include <gtest/gtest.h>
#include <string>
#include "parser.h"
#include "line_info.h"


class ExtendedLineInfo : public LineInfo {
// Extending LineInfo only with the string "message" in order to use
// LineInfo as the parameter passed to the test (TestWithParam only accepts
// one parameter).
//
// TODO: Write two different LineInfos for damage and command.
public:
    ExtendedLineInfo(std::string message,
                     std::string dealer_name,
                     std::string receiver_name,
                     std::string type,
                     std::string subtype,
                     int amount = 0,
                     bool crit = false,
                     bool deflect = false,
                     bool miss = false,
                     bool nanobots = false,
                     bool hasStats = true,
                     std::string nanoProgramName = "") : LineInfo(dealer_name,
                                                                  receiver_name,
                                                                  type,
                                                                  subtype,
                                                                  "",  // command
                                                                  amount,
                                                                  crit,
                                                                  deflect,
                                                                  miss,
                                                                  nanobots,
                                                                  hasStats,
                                                                  nanoProgramName),
                     message(message) {}
    std::string message;
};

class ParseTest : public ::testing::TestWithParam<ExtendedLineInfo> {
public:
    static void SetUpTestCase() {
        parser = new Parser("");
    }
    static void TearDownTestCase() {
        delete parser;
    }

static Parser* parser;
};


#endif  //PARSER_PARAM_TEST_H
