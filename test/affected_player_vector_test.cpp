#include "heal.h"
#include "damage.h"
#include "line_info.h"
#include "affected_player.h"
#include "affected_player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Test Player */

class MockAffectedPlayer : public AffectedPlayer {
public:
    MockAffectedPlayer(std::string name) : AffectedPlayer(name) {}
    MOCK_METHOD1(add, void(LineInfo& li));
    MOCK_CONST_METHOD1(getTotalDamage, Damage(bool nanobots));
    MOCK_CONST_METHOD0(getTotalDamage, Damage(void));
    MOCK_CONST_METHOD1(getTotalDamagePerDamageType, Damage(std::string damageType));
    MOCK_CONST_METHOD2(getTotalDamagePerDamageType, Damage(const std::string damageType,
                                                           bool nanobots));
    MOCK_CONST_METHOD0(getRegularDamage, std::map<std::string, Damage>&(void));
    MOCK_CONST_METHOD0(getNanobotsDamage, std::map<std::string, Damage>&(void));
    MOCK_CONST_METHOD0(getHeal, Heal(void));
    MOCK_CONST_METHOD0(getNano, Heal(void));
};

/* Helper functions */

const MockAffectedPlayer* addPlayerToVector(std::string name,
    AffectedPlayerVector<::testing::NiceMock<MockAffectedPlayer>*>* affectedPlayerVector) {

    LineInfo li;
    li.dealer_name = name;
    affectedPlayerVector->addToPlayers(li);
    const MockAffectedPlayer* p = affectedPlayerVector->getPlayer(name);
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

Heal createHeal(int amount, std::string playerType) {
    LineInfo li;
    li.amount = amount;
    Heal h;
    h.add(li, playerType);
    return h;
}

/* Test Fixture */

class AffectedPlayerVectorDamageTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        affectedPlayerVector = new AffectedPlayerVector<::testing::NiceMock<MockAffectedPlayer>*>;

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 10;
        li2.amount = 30;
        d1.add(li1, "dealer");
        d2.add(li2, "dealer");

        // Add players to the vector.
        p1 = addPlayerToVector("dealer1", affectedPlayerVector);
        p2 = addPlayerToVector("dealer2", affectedPlayerVector);
    }
    virtual void TearDown() {
        delete affectedPlayerVector;
    }

    AffectedPlayerVector<::testing::NiceMock<MockAffectedPlayer>*>* affectedPlayerVector;
    const MockAffectedPlayer* p1;
    const MockAffectedPlayer* p2;
    Damage d1;
    Damage d2;
};

/* Test cases */

// TODO: Make sure to test adding a player with the callers name and making sure
// that that player is not included in the result.
// Do this for all tests.

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamage_regular) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Adds the a player with the same name as the caller to the vector.
    Calls getTotalDamage(false) to retreive the regular damage.
    Verifies that each players, except for the callers, getTotalDamage is in
    turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getTotalDamage(false))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage(false))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamage(true))
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamage("Caller", false);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamage_nanobots) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Adds the a player with the same name as the caller to the vector.
    Calls getTotalDamage(true) to retreive the nanobot damage.
    Verifies that each players, except for the callers, getTotalDamage is in
    turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getTotalDamage(true))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage(true))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamage(true))
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamage("Caller", true);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamagePerDamageType_regular) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Adds the a player with the same name as the caller to the vector and
    calls getTotalDamagePerDamageType to retreive the regular damage.
    Verifies that each players, except for the callers,
    getTotalDamagePerDamageType is called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamagePerDamageType(damageType, false))
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamagePerDamageType(
                             "Caller", damageType, false);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamagePerDamageType_nanobots) {
    /*
    This test case depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Adds the a player with the same name as the caller to the vector and
    calls getTotalDamagePerDamageType to retreive the nanobot damage.
    Verifies that each players, except for the callers,
    getTotalDamagePerDamageType is called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamagePerDamageType(damageType, true))
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamagePerDamageType(
                             "Caller", damageType, true);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getNanobotsDamagePerAffectedPlayer) {

}

TEST_F(AffectedPlayerVectorDamageTest, getRegularDamagePerAffectedPlayer) {

}

TEST_F(AffectedPlayerVectorDamageTest, getTotalHeals) {

}

TEST_F(AffectedPlayerVectorDamageTest, getHealsForEachAffectedPlayer) {
    /*
    This test case depends on the implementation of Heal.
    If it fails it's a good idea to make sure that Heals's tests can pass.

    Calls getTotalHeals().
    Verifies that each players getTotalHeal is in turn called and that
    the summed heal is correct.
    */
        // TODO: Fix the descriptions.


    // Add more players to the vector. "dealer1" and "dealer2" have already
    // been added in the SetUp();
    const MockAffectedPlayer* p3 = addPlayerToVector("dealer3", affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("dealer4", affectedPlayerVector);

    Heal h1 = createHeal(10, "dealer");
    Heal h2 = createHeal(0, "dealer");
    Heal h3 = createHeal(500000, "dealer");
    Heal h4 = createHeal(3000, "dealer");

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::Return(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::Return(h2));
    EXPECT_CALL(*p3, getHeal())
        .WillOnce(::testing::Return(h3));
    EXPECT_CALL(*p4, getHeal())
        .WillOnce(::testing::Return(h4));

    std::vector<std::pair<std::string, Heal>> result =
        affectedPlayerVector->getHealsForEachAffectedPlayer();

    EXPECT_EQ(h3.getPotentialDealt(), result[0].second.getPotentialDealt());
    EXPECT_EQ(h4.getPotentialDealt(), result[1].second.getPotentialDealt());
    EXPECT_EQ(h1.getPotentialDealt(), result[2].second.getPotentialDealt());
    EXPECT_EQ(h2.getPotentialDealt(), result[3].second.getPotentialDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalNano) {

}

TEST_F(AffectedPlayerVectorDamageTest, getNanoForEachAffectedPlayer) {

}

TEST(AffectedPlayerVectorTest, getTotalDamageForEachPlayer) {
    /*
    Adds several players with different damage.
    Calls getTotalDamageForEachPlayer().
    Verifies that each players getTotalDamage() is called and that the
    returned vector is sorted on total received damage.
    */

    AffectedPlayerVector<::testing::NiceMock<MockAffectedPlayer>*> affectedPlayerVector;
    // Add players to the vector.
    const MockAffectedPlayer* p1 = addPlayerToVector("Receiver1", &affectedPlayerVector);
    const MockAffectedPlayer* p2 = addPlayerToVector("Receiver2", &affectedPlayerVector);
    const MockAffectedPlayer* p3 = addPlayerToVector("Receiver3", &affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("Receiver4", &affectedPlayerVector);

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
    result = affectedPlayerVector.getTotalDamageForEachPlayer("Caller");

    EXPECT_EQ(d3.getTotalReceived(), result[0].second.getTotalReceived());
    EXPECT_EQ(d1.getTotalReceived(), result[1].second.getTotalReceived());
    EXPECT_EQ(d4.getTotalReceived(), result[2].second.getTotalReceived());
    EXPECT_EQ(d2.getTotalReceived(), result[3].second.getTotalReceived());
}
