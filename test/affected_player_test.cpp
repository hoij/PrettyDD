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
        affectedPlayer = new AffectedPlayer("Player1");
    }
    virtual void TearDown() {
        delete affectedPlayer;
    }
    AffectedPlayer* affectedPlayer;
};

TEST_F(AffectedPlayerTest, addDamage_dealerNanobots) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";
    li.nanobots = true;

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getNanobotsDamage().
                             at(li.subtype).getTotalDealt());
}

TEST_F(AffectedPlayerTest, addDamage_dealerRegular) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getRegularDamage().
                             at(li.subtype).getTotalDealt());
}

TEST_F(AffectedPlayerTest, addDamage_receiverNanobots) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";
    li.nanobots = true;

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getNanobotsDamage().
                             at(li.subtype).getTotalReceived());
}

TEST_F(AffectedPlayerTest, addDamage_receiverRegular) {
    /* Verifies that the correct branch in the addDamage method is selected */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "damage";
    li.subtype = "projectile";

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getRegularDamage().
                             at(li.subtype).getTotalReceived());
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

    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialReceived());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealt());
    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getActualReceived());
    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialDealt());
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

    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getPotentialReceived());
    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialDealt());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualReceived());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealt());
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

    EXPECT_EQ(0, affectedPlayer->getHeal().getPotentialReceived());
    EXPECT_EQ(li.amount, affectedPlayer->getHeal().getPotentialDealt());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualReceived());
    EXPECT_EQ(0, affectedPlayer->getHeal().getActualDealt());
}

TEST_F(AffectedPlayerTest, addNano_receiver) {
    /* Verifies that the correct branch in the add method is selected */
    LineInfo li;
    li.receiver_name = affectedPlayer->getName();
    li.dealer_name = "SomeoneElse";
    li.amount = 100;
    li.type = "nano";

    affectedPlayer->add(li);

    EXPECT_EQ(li.amount, affectedPlayer->getNano().getTotalReceived());
    EXPECT_EQ(0, affectedPlayer->getNano().getTotalDealt());
}

TEST_F(AffectedPlayerTest, addNano_dealer) {
    /* Verifies that the correct branch in the add method is selected */
    LineInfo li;
    li.receiver_name = "SomeoneElse";
    li.dealer_name = affectedPlayer->getName();
    li.amount = 100;
    li.type = "nano";

    affectedPlayer->add(li);

    EXPECT_EQ(0, affectedPlayer->getNano().getTotalReceived());
    EXPECT_EQ(li.amount, affectedPlayer->getNano().getTotalDealt());
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
              affectedPlayer->getTotalDamage().getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalRegularDamage) {
    /* Adds one nanobots damage and two regular damage lines.
    Verifies that the summed regular damage is returned */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";
    li1.subtype = "melee";

    LineInfo li2 = li1;
    LineInfo li3 = li1;
    li1.amount = 100;
    li2.amount = 300;
    li3.amount = 900;
    li1.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);
    affectedPlayer->add(li3);

    EXPECT_EQ(li2.amount + li3.amount,
              affectedPlayer->getTotalRegularDamage().getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalNanobotsDamage) {
    /* Adds one regular damage and two nanobots damage lines.
    Verifies that the summed nanobots damage is returned */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";
    li1.subtype = "melee";

    LineInfo li2 = li1;
    LineInfo li3 = li1;
    li1.amount = 100;
    li2.amount = 300;
    li3.amount = 900;
    li2.nanobots = true;
    li3.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);
    affectedPlayer->add(li3);

    EXPECT_EQ(li2.amount + li3.amount,
              affectedPlayer->getTotalNanobotsDamage().getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalDamagePerDamageType) {
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
              affectedPlayer->getTotalDamagePerDamageType("melee").
                  getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalRegularDamagePerDamageType) {
    /* Adds four damage lines and calls getTotalDamagePerDamageType(false).
    Verifies that the the regular damage is returned for the specified
    damageType */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";

    LineInfo li2 = li1;
    LineInfo li3 = li1;
    LineInfo li4 = li1;

    li1.subtype = "melee";
    li2.subtype = "melee";
    li3.subtype = "melee";
    li4.subtype = "projectile";
    li1.amount = 100;
    li2.amount = 300;
    li3.amount = 900;
    li4.amount = 10000;
    li3.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);
    affectedPlayer->add(li3);
    affectedPlayer->add(li4);

    Damage result = affectedPlayer->getTotalRegularDamagePerDamageType(
                                        "melee");

    EXPECT_EQ(li1.amount + li2.amount, result.getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalNanobotsDamagePerDamageType) {
    /* Adds four damage lines and calls getTotalDamagePerDamageType(true).
    Verifies that the the nanobots damage is returned for the specified
    damageType */
    LineInfo li1;
    li1.dealer_name = affectedPlayer->getName();
    li1.type = "damage";

    LineInfo li2 = li1;
    LineInfo li3 = li1;
    LineInfo li4 = li1;

    li1.subtype = "melee";
    li2.subtype = "melee";
    li3.subtype = "melee";
    li4.subtype = "projectile";
    li1.amount = 100;
    li2.amount = 300;
    li3.amount = 900;
    li3.amount = 10000;
    li1.nanobots = true;
    li2.nanobots = true;
    li3.nanobots = false;
    li4.nanobots = true;

    affectedPlayer->add(li1);
    affectedPlayer->add(li2);
    affectedPlayer->add(li3);
    affectedPlayer->add(li4);

    Damage result = affectedPlayer->getTotalNanobotsDamagePerDamageType(
                                        "melee");

    EXPECT_EQ(li1.amount + li2.amount, result.getTotalDealt());
}

TEST_F(AffectedPlayerTest, getTotalDamagePerDamageType_nonexistent) {
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

    Damage result1 = affectedPlayer->getTotalDamagePerDamageType(
                         "nonexistingType");
    Damage result2 = affectedPlayer->getTotalDamagePerDamageType(
                         "nonexistingType");

    EXPECT_EQ(0, result1.getTotalDealt());
    EXPECT_EQ(0, result2.getTotalDealt());
    EXPECT_EQ(0, result1.getTotalReceived());
    EXPECT_EQ(0, result2.getTotalReceived());
}
