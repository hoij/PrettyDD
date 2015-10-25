#ifdef WIN32
#pragma warning(disable : 4250)
#endif

#include "heal.h"
#include "damage.h"
#include "line_info.h"
#include "player.h"
#include "player_interface.h"
#include "player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Many of these test cases rely on the implementation of Damage/Heal/Nano/XP.
If a test case fails it might be a good idea to verify that the tests for
these classes pass. */


/* Test Player */

class MockPlayer : public virtual PlayerInterface, public Player {
public:
    MockPlayer(std::string name, MyTime* myTime) : Player(name, myTime) {}
    MOCK_METHOD1(add, void(LineInfo& li));

    MOCK_CONST_METHOD0(getTotalDamage, Damage(void));

    MOCK_CONST_METHOD1(getTotalDamagePerDamageType,
                       Damage(std::string damageType));

    MOCK_CONST_METHOD0(getTotalDamageForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getTotalHeals, Heal(void));
};

/* Helper functions */

const MockPlayer* addPlayerToVector(std::string name,
    PlayerVector<::testing::NiceMock<MockPlayer>*>* playerVector) {

    LineInfo li;
    li.dealer_name = name;
    playerVector->addToPlayers(li);
    const MockPlayer* p = playerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return p;
}

Damage createDealerDamage(int amount) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.addDamageDealtOnPlayer(li);
    return d;
}

Damage createReceiverDamage(int amount) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.addDamageReceivedFromPlayer(li);
    return d;
}

class PlayerVectorDamageTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector<::testing::NiceMock<MockPlayer>*>;
        playerVector->startLogging();
        // Set up the return values.
        d1 = createDealerDamage(10);
        d1 = createDealerDamage(30);

        // Add players to the vector.
        p1 = addPlayerToVector("dealer1", playerVector);
        p2 = addPlayerToVector("dealer2", playerVector);
    }
    virtual void TearDown() {
        delete playerVector;
    }

    PlayerVector<::testing::NiceMock<MockPlayer>*>* playerVector;
    const MockPlayer* p1;
    const MockPlayer* p2;
    Damage d1;
    Damage d2;
};

/* Test cases */

TEST_F(PlayerVectorDamageTest, reset) {
    // Reset to remove the two players added in the SetUp.
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Reset an empty vector:
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Add two players
    std::string p1Name = "dealerAfterReset1";
    std::string p2Name = "dealerAfterReset2";
    p1 = addPlayerToVector(p1Name, playerVector);
    p2 = addPlayerToVector(p2Name, playerVector);
    EXPECT_EQ(2, playerVector->size());
    EXPECT_EQ(p1Name, p1->getName());
    EXPECT_EQ(p2Name, p2->getName());
}

TEST_F(PlayerVectorDamageTest, getTotalDamage) {
    /* Calls getTotalDamage().
    Verifies that each players getTotalDamage is in turn called and
    that the summed damage is correct. */

    EXPECT_CALL(*p1, getTotalDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage())
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamage();

    EXPECT_EQ((d1 + d2).getTotalDealtOnPlayer(), totalDamage.getTotalDealtOnPlayer());
}

TEST_F(PlayerVectorDamageTest, getTotalDamagePerDamageType) {
    /* Calls getTotalDamagePerDamageType("poison").
    Verifies that each players getTotalDamagePerDamageType is in turn
    called and that the summed damage is correct. */

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamagePerDamageType(damageType);

    EXPECT_EQ((d1 + d2).getTotalDealtOnPlayer(), totalDamage.getTotalDealtOnPlayer());
}

TEST_F(PlayerVectorDamageTest, getTotalHeals) {
    /* Calls getTotalHeals().
    Verifies that each players getTotalHeal is in turn called and that
    the summed heal is correct. */

    LineInfo li1;
    LineInfo li2;
    li1.amount = 10;
    li2.amount = 30;
    Heal h1;
    Heal h2;
    h1.addHealDealtOnPlayer(li1);
    h2.addHealDealtOnPlayer(li2);

    EXPECT_CALL(*p1, getTotalHeals())
        .WillOnce(::testing::Return(h1));
    EXPECT_CALL(*p2, getTotalHeals())
        .WillOnce(::testing::Return(h2));

    Heal resultHeal = playerVector->getTotalHeals();

    EXPECT_EQ((h1 + h2).getPotentialDealtOnPlayer(), resultHeal.getPotentialDealtOnPlayer());
}

TEST(PlayerVectorTest, getTotalDamageForEachPlayer) {
    /*
    Adds several players with different damage.
    Calls getTotalDamageForEachPlayer().
    Verifies that each players getTotalDamage() is called and that the
    returned vector is sorted on total received damage.
    */

    PlayerVector<::testing::NiceMock<MockPlayer>*> playerVector;
    playerVector.startLogging();
    // Add players to the vector.
    const MockPlayer* p1 = addPlayerToVector("Receiver1", &playerVector);
    const MockPlayer* p2 = addPlayerToVector("Receiver2", &playerVector);
    const MockPlayer* p3 = addPlayerToVector("Receiver3", &playerVector);
    const MockPlayer* p4 = addPlayerToVector("Receiver4", &playerVector);

    // Set up the return values.

    Damage d1 = createReceiverDamage(7000);
    Damage d2 = createReceiverDamage(0);
    Damage d3 = createReceiverDamage(500000);
    Damage d4 = createReceiverDamage(1500);

    EXPECT_CALL(*p1, getTotalDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*p3, getTotalDamage())
        .WillOnce(::testing::Return(d3));
    EXPECT_CALL(*p4, getTotalDamage())
        .WillOnce(::testing::Return(d4));

    std::vector<std::pair<std::string, Damage>> result;
    result = playerVector.getTotalDamageDealtPerPlayer();

    // Assuming the order is the same as when added which could be untrue.
    EXPECT_EQ(d1.getTotalReceivedFromPlayer(),
              result[0].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d2.getTotalReceivedFromPlayer(),
              result[1].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d3.getTotalReceivedFromPlayer(),
              result[2].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d4.getTotalReceivedFromPlayer(),
              result[3].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(4, result.size());
}
