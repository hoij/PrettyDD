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
        affectedPlayer = new AffectedPlayer("Player1", std::make_shared<MyTime>());
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

    std::vector<std::pair<std::string, Damage>> dealt =
        affectedPlayer->getDamageDealtOnPlayer();
    std::vector<std::pair<std::string, Damage>> received =
        affectedPlayer->getDamageReceivedFromPlayer();

    EXPECT_EQ(1, dealt.size());
    EXPECT_EQ(li.amount, dealt[0].second.getTotal());
    EXPECT_EQ(0, received.size());
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

    std::vector<std::pair<std::string, Damage>> dealt =
        affectedPlayer->getDamageDealtOnPlayer();
    std::vector<std::pair<std::string, Damage>> received =
        affectedPlayer->getDamageReceivedFromPlayer();

    EXPECT_EQ(0, dealt.size());
    EXPECT_EQ(li.amount, received[0].second.getTotal());
    EXPECT_EQ(1, received.size());
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
              affectedPlayer->getTotalDamageDealtOnPlayer().getTotal());
}
