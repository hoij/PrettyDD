#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meGotXP, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You received 247 xp."),
            ExtendedLineInfo("",
                             "You",
                             LineType::xp,
                             "gained",
                             247)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 2562 new Alien Experience Points."),
            ExtendedLineInfo("",
                             "You",
                             LineType::aixp,
                             "gained",
                             2562)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You lost 9822 xp."),
            ExtendedLineInfo("",
                             "You",
                             LineType::xp,
                             "lost",
                             9822)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 100 PVP Solo Score."),
            ExtendedLineInfo("",
                             "You",
                             LineType::pvpSoloScore,
                             "gained",
                             100)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 101 PVP Team Score."),
            ExtendedLineInfo("",
                             "You",
                             LineType::pvpTeamScore,
                             "gained",
                             101))));

INSTANTIATE_TEST_CASE_P(meGotSK, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You gained 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             LineType::sk,
                             "gained",
                             200)),
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You lost 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             LineType::sk,
                             "lost",
                             200))));

INSTANTIATE_TEST_CASE_P(research, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Research",
                              "139139 of your XP were allocated to your personal research.<br>"),
            ExtendedLineInfo("",
                             "You",
                             LineType::research,
                             "gained",
                             139139)),
        std::make_tuple(
            FormattedLineStub("Research",
                              "You have completed your research on \"Force Recon 9\"."),
            ExtendedLineInfo("",
                             "",
                             LineType::notAvailable,
                             "",
                             0))));
