#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(otherHitByNano, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Other hit by nano",
                              "Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage."),
        ExtendedLineInfo("Sgtcuddle",
                         "Predator Rogue",
                         "damage",
                         "energy",
                         1293,
                         false,
                         false,
                         false,
                         true)),
        std::make_tuple(
            FormattedLineStub("Other hit by nano",
                              "Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage."),
        ExtendedLineInfo("Unknown",
                         "Frozen Spinetooth",
                         "damage",
                         "unknown",
                         445,
                         false,
                         false,
                         false,
                         true))));
