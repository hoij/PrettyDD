#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "parser_param_test.h"


INSTANTIATE_TEST_CASE_P(otherHitByNano, ParseTest,
    testing::Values(
        ExtendedDamageLineInfo("Predator Rogue was attacked with nanobots from Sgtcuddle for 1293 points of energy damage.",
                               "Sgtcuddle",
                               "Predator Rogue",
                               "damage",
                               "energy",
                               1293,
                               false,
                               false,
                               false,
                               true),
        ExtendedDamageLineInfo("Frozen Spinetooth was attacked with nanobots for 445 points of unknown damage.",
                               "Sgtcuddle",
                               "Frozen Spinetooth",
                               "damage",
                               "Unknown",
                               445,
                               false,
                               false,
                               false,
                               true)));
