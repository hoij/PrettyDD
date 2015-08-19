#include "heal.h"
#include "damage.h"
#include "line_info.h"
#include "player.h"
#include "player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Test Player */

class MockPlayer : public Player {
public:
    MockPlayer(std::string name) : Player(name) {}
    MOCK_METHOD1(add, void(LineInfo& li));
    MOCK_CONST_METHOD1(getTotalDamage, Damage(bool nanobots));
    MOCK_CONST_METHOD0(getTotalDamage, Damage(void));
    MOCK_CONST_METHOD2(getTotalDamagePerDamageType,
                       Damage(std::string damageType, bool nanobots));
    MOCK_CONST_METHOD0(getTotalDamageForEachPlayer,
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

Damage createDamage(int amount, std::string playerType) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.add(li, playerType);
    return d;
}

class PlayerVectorDamageTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector<::testing::NiceMock<MockPlayer>*>;

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 10;
        li2.amount = 30;
        d1.add(li1, "dealer");
        d2.add(li2, "dealer");

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

TEST_F(PlayerVectorDamageTest, getTotalDamage_regular) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamage(false) to retreive the regular damage.
    Verifies that each players getTotalDamage is in turn called and that
    the summed damage is correct.
    */

    EXPECT_CALL(*p1, getTotalDamage(false))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage(false))
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamage(false);
    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamage_nanobots) {
    // TODO: Keep two tests for regular/nanonbots or just have one?
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamage(true) to retreive the nanobot damage.
    Verifies that each players getTotalDamage is in turn called and that
    the summed damage is correct.
    */

    EXPECT_CALL(*p1, getTotalDamage(true))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage(true))
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamage(true);
    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamagePerDamageType_regular) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamagePerDamageType("poison", false) to retreive the regular
    damage.
    Verifies that each players getTotalDamagePerDamageType is in turn
    called and that the summed damage is correct.
    */

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamagePerDamageType(damageType, false);
    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamagePerDamageType_nanobots) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamagePerDamageType("poison", false) to retreive the nanobot
    damage.
    Verifies that each players getTotalDamagePerDamageType is in turn
    called and that the summed damage is correct.
    */

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d2));

    Damage totalDamage = playerVector->getTotalDamagePerDamageType(damageType, true);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalHeals) {
    /*
    This test case depends on the implementation of Heal.
    If it fails it's a good idea to make sure that Heals's tests can pass.

    Calls getTotalHeals().
    Verifies that each players getTotalHeal is in turn called and that
    the summed heal is correct.
    */

    LineInfo li1;
    LineInfo li2;
    li1.amount = 10;
    li2.amount = 30;
    Heal h1;
    Heal h2;
    h1.add(li1, "dealer");
    h2.add(li2, "dealer");

    EXPECT_CALL(*p1, getTotalHeals())
        .WillOnce(::testing::Return(h1));
    EXPECT_CALL(*p2, getTotalHeals())
        .WillOnce(::testing::Return(h2));

    Heal resultHeal = playerVector->getTotalHeals();

    EXPECT_EQ((h1 + h2).getPotentialDealt(), resultHeal.getPotentialDealt());
}

TEST(PlayerVectorTest, getTotalDamageForEachPlayer) {
    /*
    Adds several players with different damage.
    Calls getTotalDamageForEachPlayer().
    Verifies that each players getTotalDamage() is called and that the
    returned vector is sorted on total received damage.
    */

    PlayerVector<::testing::NiceMock<MockPlayer>*> playerVector;
    // Add players to the vector.
    const MockPlayer* p1 = addPlayerToVector("Receiver1", &playerVector);
    const MockPlayer* p2 = addPlayerToVector("Receiver2", &playerVector);
    const MockPlayer* p3 = addPlayerToVector("Receiver3", &playerVector);
    const MockPlayer* p4 = addPlayerToVector("Receiver4", &playerVector);

    // Set up the return values.
    Damage d1 = createDamage(7000, "receiver");
    Damage d2 = createDamage(0, "receiver");
    Damage d3 = createDamage(500000, "receiver");
    Damage d4 = createDamage(1500, "receiver");

    EXPECT_CALL(*p1, getTotalDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*p3, getTotalDamage())
        .WillOnce(::testing::Return(d3));
    EXPECT_CALL(*p4, getTotalDamage())
        .WillOnce(::testing::Return(d4));

    std::vector<std::pair<std::string, Damage>> result;
    result = playerVector.getTotalDamageForEachPlayer();

    EXPECT_EQ(d3.getTotalReceived(), result[0].second.getTotalReceived());
    EXPECT_EQ(d1.getTotalReceived(), result[1].second.getTotalReceived());
    EXPECT_EQ(d4.getTotalReceived(), result[2].second.getTotalReceived());
    EXPECT_EQ(d2.getTotalReceived(), result[3].second.getTotalReceived());
}
