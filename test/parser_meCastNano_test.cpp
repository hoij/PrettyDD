#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meCastNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Executing Nano Program: Improved Mongo Crush!."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "execute",
                             0,
                             false,
                             false,
                             false,
                             false,
                             true,
                             "Improved Mongo Crush!")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Nano program executed successfully."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "land")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Executing Nano Program: Thorough Overhaul."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "execute",
                             0,
                             false,
                             false,
                             false,
                             false,
                             true,
                             "Thorough Overhaul")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Executing Nano Program: Beauty of Life."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "execute",
                             0,
                             false,
                             false,
                             false,
                             false,
                             true,
                             "Beauty of Life")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Executing Nano Program: Uncontrollable Body Tremors."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "execute",
                             0,
                             false,
                             false,
                             false,
                             false,
                             true,
                             "Uncontrollable Body Tremors")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Nano program aborted."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "abort")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Target resisted."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "resist")),
        std::make_tuple(
            FormattedLineStub("Me Cast Nano",
                              "Your target countered the nano program."),
            ExtendedLineInfo("You",
                             "",
                             LineType::nanoCast,
                             "counter"))));
