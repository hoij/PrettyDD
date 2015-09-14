#include "affected_player.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

/*
Many of these test use the implementation of Damage, Heal and Nano. If an
Issue arises it might be a good idea to check that their tests pass.
*/

class AffectedPlayerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        MyTime* myTime = new MyTime();
        affectedPlayer = new AffectedPlayer("Player1", myTime);
    }
    virtual void TearDown() {
        delete affectedPlayer;
    }
    AffectedPlayer* affectedPlayer;
};

TEST_F(AffectedPlayerTest, addDamage_dealer) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";
    li.nanobots = true;

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getDamagePerDamageType("projectile").
                                             getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, addDamage_receiver) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";
    li.nanobots = true;

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getDamagePerDamageType("projectile").
                                             getTotalReceivedFromPlayer());
}

TEST_F(AffectedPlayerTest, addHeal_actual) {
    /* Verifies that an actual heal is added as a received heal */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.dealer_name = "SomeoneElse";
    li.amount = 100;
    li.type = "heal";
    li.subtype = "actual";

    affectedPlayer->add(li);

    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealtOnPlayer());
    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getActualReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, addHeal_potentialReceived) {
    /* Verifies that a potential heal is added as a received heal */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.dealer_name = "SomeoneElse";
    li.amount = 100;
    li.type = "heal";
    li.subtype = "potential";

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getPotentialReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialDealtOnPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, addHeal_potentialDealt) {
    /* Verifies that a potential heal is added as a dealt heal */
    LineInfo li;
    li.receiver_name = "SomeoneElse";
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "heal";
    li.subtype = "potential";

    affectedPlayer->add(li);

    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialReceivedFromPlayer());
    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getPotentialDealtOnPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, addNano_receiver) {
    /* Verifies that the correct branch in the add method is selected */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.dealer_name = "SomeoneElse";
    li.amount = 100;
    li.type = "nano";

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getNano().getTotalReceivedFromPlayer());
    EXPECT_EQ(0, affectedPlayer->getNano().getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, addNano_dealer) {
    /* Verifies that the correct branch in the add method is selected */
    LineInfo li;
    li.receiver_name = "SomeoneElse";
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "nano";

    affectedPlayer->add(li);

    EXPECT_EQ(0, affectedPlayer->getNano().getTotalReceivedFromPlayer());
    EXPECT_EQ(li.amount, affectedPlayer->getNano().getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, getTotalDamage) {
    /* Verifies that the the sum of the regular and nanobots damage is
    returned */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";

    LineInfo li2 = li1;

    li1.subtype = "melee";
    li2.subtype = "projectile";
    li1.amount = 100;
    li2.amount = 300;
    li2.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);

    EXPECT_EQ(li1.amount + li2.amount,
              affectedPlayer->getTotalDamage().getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, getDamagePerDamageType) {
    /* Adds two damages with the same subtype and one with a different.
    Verifies that the sum of the nanobots and regular damage with the same
    subtype is returned for the specified damageType */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";

    LineInfo li2 = li1;
    LineInfo li3 = li1;
    li1.subtype = "melee";
    li2.subtype = "melee";
    li3.subtype = "projectile";
    li1.amount = 100;
    li2.amount = 300;
    li3.amount = 900;
    li2.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);

    EXPECT_EQ(li1.amount + li2.amount,
              affectedPlayer->getDamagePerDamageType("melee").
                  getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerTest, getDamagePerDamageType_nonexistent) {
    /* Adds two damages of the same subtype. One with nanobots and the other
    without. Then calls getTotalDamagePerDamageType with a non-existing type.
    Verifies that an empty instance of Damage is returned when the
    damageType is not found among the nanobots damage types */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";

    LineInfo li2 = li1;

    li1.subtype = "melee";
    li2.subtype = "melee";
    li1.amount = 100;
    li2.amount = 300;
    li1.nanobots = true;
    li2.nanobots = false;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);

    Damage result1 = affectedPlayer->getDamagePerDamageType(
                         "nonexistingType");
    Damage result2 = affectedPlayer->getDamagePerDamageType(
                         "nonexistingType");

    EXPECT_EQ(0, result1.getTotalDealtOnPlayer());
    EXPECT_EQ(0, result2.getTotalDealtOnPlayer());
    EXPECT_EQ(0, result1.getTotalReceivedFromPlayer());
    EXPECT_EQ(0, result2.getTotalReceivedFromPlayer());
}
