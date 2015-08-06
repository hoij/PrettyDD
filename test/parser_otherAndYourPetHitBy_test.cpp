#include "parser_param_test.h"

#include <gtest/gtest.h>


INSTANTIATE_TEST_CASE_P(otherAndYourPetHitByOther, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Sheila Marlene hit Predator Rogue for 461 points of melee damage."),
            ExtendedLineInfo("Sheila Marlene",
                             "Predator Rogue",
                             "damage",
                             "melee",
                             461)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Sgtcuddle hit Predator Rogue for 4302 points of Burst damage."),
            ExtendedLineInfo("Sgtcuddle",
                             "Predator Rogue",
                             "damage",
                             "Burst",
                             4302)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Sgtcuddle hit Predator Rogue for 15000 points of Full Auto damage."),
            ExtendedLineInfo("Sgtcuddle",
                             "Predator Rogue",
                             "damage",
                             "Full Auto",
                             15000)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Balas's reflect shield hit Junebop for 161 points of damage."),
            ExtendedLineInfo("Balas",
                             "Junebop",
                             "damage",
                             "reflect shield",
                             161)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Letter hit Reet of Paradise for 1586 points of melee damage.Critical hit!"),
            ExtendedLineInfo("Letter",
                             "Reet of Paradise",
                             "damage",
                             "melee",
                             1586,
                             true)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Cratdat hit Ensign - Ilari'Ra for 801 points of projectile damage. Glancing hit."),
            ExtendedLineInfo("Cratdat",
                             "Ensign - Ilari'Ra",
                             "damage",
                             "projectile",
                             801,
                             false,
                             true)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Someone absorbed 8198 points of energy damage."),
            ExtendedLineInfo("Unknown",
                             "Someone",
                             "damage",
                             "energy",
                             8198)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Something hit Addicted2 for 49 points of damage by reflect shield."),
            ExtendedLineInfo("Unknown",
                             "Addicted2",
                             "damage",
                             "reflect shield",
                             49)),
        std::make_tuple(
            FormattedLineStub("Other hit by other",
                              "Something hit Enfodruid for 1 points of damage by damage shield."),
            ExtendedLineInfo("Unknown",
                             "Enfodruid",
                             "damage",
                             "damage shield",
                             1)),
        std::make_tuple(
            FormattedLineStub("Your pet hit by other",
                              "Letter hit Guard for 623 points of melee damage."),
            ExtendedLineInfo("Letter",
                             "Guard",
                             "damage",
                             "melee",
                             623)),
        std::make_tuple(
            FormattedLineStub("Your pet hit by other",
                              "Letter hit Guard for 1268 points of melee damage.Critical hit!"),
            ExtendedLineInfo("Letter",
                             "Guard",
                             "damage",
                             "melee",
                             1268,
                             true))));

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

INSTANTIATE_TEST_CASE_P(yourPetHitByMonster, ParseTest,
    testing::Values(
        std::make_tuple(
            FormattedLineStub("Your pet hit by monster",
                              "Your pet Vios was damaged by a toxic substance for 25 points of damage."),
            ExtendedLineInfo("Environment",
                             "Vios",
                             "damage",
                             "a toxic substance",
                             25))));

//TODO:
//INSTANTIATE_TEST_CASE_P(yourPetHitByNano, ParseTest,
//    testing::Values(
//        std::make_tuple(
//            FormattedLineStub("",
//                              ""),
//            ExtendedLineInfo("Need example line",
//                             "",
//                             "",
//                             "",
//                             "",
//                             0,
//                             false,
//                             false,
//                             false,
//                             true))));
