#ifndef PARSER_PARAM_TEST_H
#define PARSER_PARAM_TEST_H

#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include "parser.h"
#include "line_info.h"
#include "formatted_line_stub.h"


class ExtendedLineInfo : public LineInfo {
// Extending LineInfo to create constructors for the test parameters.
public:
    ExtendedLineInfo() {}
    ExtendedLineInfo(std::string dealer_name,
                     std::string receiver_name,
                     std::string type,
                     std::string subtype,
                     int amount = 0,
                     bool crit = false,
                     bool deflect = false,
                     bool miss = false,
                     bool nanobots = false,
                     bool hasStats = true,
                     std::string nanoProgramName = "") {

                     this->dealer_name = dealer_name;
                     this->receiver_name = receiver_name;
                     this->type = type;
                     this->subtype = subtype;
                     this->command = "";  // command
                     this->amount = amount;
                     this->crit = crit;
                     this->deflect = deflect;
                     this->miss = miss;
                     this->nanobots = nanobots;
                     this->hasStats = hasStats;
                     this->nanoProgramName = nanoProgramName;
    }
};

class ParseTest : public ::testing::TestWithParam<std::tuple<FormattedLineStub, ExtendedLineInfo>> {
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
