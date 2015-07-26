#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(meHitByNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Me hit by nano",
                              "You were attacked with nanobots from Predator Rogue for 875 points of poison damage."),
            ExtendedLineInfo("Predator Rogue",
                             "You",
                             "damage",
                             "poison",
                             875,
                             false,
                             false,
                             false,
                             true))));
