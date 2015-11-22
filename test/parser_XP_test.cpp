#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(meGotXP, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You received 247 xp."),
            ExtendedLineInfo("",
                             "You",
                             XP,
                             "gained",
                             247)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 2562 new Alien Experience Points."),
            ExtendedLineInfo("",
                             "You",
                             AIXP,
                             "gained",
                             2562)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You lost 9822 xp."),
            ExtendedLineInfo("",
                             "You",
                             XP,
                             "lost",
                             9822)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 100 PVP Solo Score."),
            ExtendedLineInfo("",
                             "You",
                             PVP_SOLO_SCORE,
                             "gained",
                             100)),
        std::make_tuple(
            FormattedLineStub("Me got XP",
                              "You gained 101 PVP Team Score."),
            ExtendedLineInfo("",
                             "You",
                             PVP_TEAM_SCORE,
                             "gained",
                             101))));

INSTANTIATE_TEST_CASE_P(meGotSK, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You gained 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             SK,
                             "gained",
                             200)),
        std::make_tuple(
            FormattedLineStub("Me got SK",
                              "You lost 200 points of Shadowknowledge."),
            ExtendedLineInfo("",
                             "You",
                             SK,
                             "lost",
                             200))));

INSTANTIATE_TEST_CASE_P(research, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Research",
                              "139139 of your XP were allocated to your personal research.<br>"),
            ExtendedLineInfo("",
                             "You",
                             RESEARCH,
                             "gained",
                             139139)),
        std::make_tuple(
            FormattedLineStub("Research",
                              "You have completed your research on \"Force Recon 9\"."),
            ExtendedLineInfo("",
                             "",
                             NA,
                             "",
                             0))));
