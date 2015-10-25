#ifdef WIN32
#pragma warning(disable : 4250)
#pragma warning(disable : 4373)
#endif

#include "affected_player.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "nano.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Many of these test cases rely on the implementation of Damage/Heal/Nano/XP.
If a test case fails it might be a good idea to verify that the tests for
these classes pass. */


/* Test Player */

class MockAffectedPlayer : public virtual AffectedPlayerInterface, public AffectedPlayer {
public:
    MockAffectedPlayer(std::string name, MyTime* myTime) : AffectedPlayer(name, myTime) {}
    MOCK_CONST_METHOD0(getTotalDamageReceivedFromPlayer, Damage(void));
    MOCK_CONST_METHOD0(getTotalDamageDealtOnPlayer, Damage(void));
    MOCK_CONST_METHOD0(getDamageReceivedFromPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getDamageDealtOnPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getHeal, Heal&(void));
    MOCK_CONST_METHOD0(getNano, Nano&(void));
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

Damage createDamage(int amount) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.add(li);
    return d;
}

Heal createHeal(int amount) {
    LineInfo li;
    li.amount = amount;
    Heal h;
    h.addHealDealtOnPlayer(li);
    return h;
}

Nano createNano(int amount) {
    LineInfo li;
    li.amount = amount;
    Nano n;
    n.addNanoDealtOnPlayer(li);
    return n;
}

/* Test Fixture */

class AffectedPlayerVectorDamageTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        affectedPlayerVector = new AffectedPlayerVector<::testing::NiceMock<MockAffectedPlayer>*>;

        // Set up the return values.
        d1 = createDamage(10);
        d2 = createDamage(30);

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

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamageDealtOnPlayer) {
    /*
    Adds the a player with the same name as the caller to the vector.
    Calls getTotalDamage().
    Verifies that each players, except for the callers, getTotalDamage
    is in turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getDamageDealtOnPlayer())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getDamageDealtOnPlayer())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getDamageDealtOnPlayer())
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamageDealtOnPlayer("Caller");

    EXPECT_EQ((d1 + d2).getTotal(), totalDamage.getTotal());
}

TEST_F(AffectedPlayerVectorDamageTest, getAllDamageFromAffectedPlayer) {
    /* Verifies that getAllDamage is called on the the correct player */

    std::vector<std::pair<std::string, Damage>> expected;
    expected.emplace_back("type", d2);

    EXPECT_CALL(*p2, getAllDamage())
        .WillOnce(::testing::Return(expected));

    std::vector<std::pair<std::string, Damage>> result2 =
        affectedPlayerVector->getAllDamageFromAffectedPlayer("dealer2");

    EXPECT_EQ(expected[0].first, result2[0].first);
    EXPECT_EQ(expected[0].second.getTotalDealtOnPlayer(),
              result2[0].second.getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorDamageTest, getAllDamageFromAffectedPlayer_notFound) {
    /* Verifies that a vector with the pair "empty" and an empty Damage
    is returned when the player is not found in the vector. */

    std::string name = "NotInTheVector";

    auto result = affectedPlayerVector->getAllDamageFromAffectedPlayer(name);

    EXPECT_EQ(1, result.size());
    EXPECT_EQ("empty", result[0].first);
    EXPECT_EQ(0, result[0].second.getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalHeals) {
    /* Verifies that getHeal is called for each player in the vector and
    that the returned sum is correct. */

    Heal h1 = createHeal(10);
    Heal h2 = createHeal(30);
    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*caller, getHeal())
        .Times(0);

    Heal totalHeal = affectedPlayerVector->getTotalHeals("Caller");

    EXPECT_EQ((h1 + h2).getPotentialDealtOnPlayer(), totalHeal.getPotentialDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorDamageTest, getHealsPerAffectedPlayer) {
    /*
    This test case depends on the implementation of Heal.
    If it fails, make sure that Heals's tests can pass.

    Calls getHealsPerAffectedPlayer().
    Verifies that each players getHeal is in turn called and that
    the returned vector is sorted on potential heal dealt.
    */

    // Add more players to the vector. "dealer1" and "dealer2" have already
    // been added in the SetUp();
    const MockAffectedPlayer* p3 = addPlayerToVector("dealer3",
                                                     affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("dealer4",
                                                     affectedPlayerVector);

    Heal h1 = createHeal(10);
    Heal h2 = createHeal(0);
    Heal h3 = createHeal(500000);
    Heal h4 = createHeal(3000);

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*p3, getHeal())
        .WillOnce(::testing::ReturnRef(h3));
    EXPECT_CALL(*p4, getHeal())
        .WillOnce(::testing::ReturnRef(h4));

    std::vector<std::pair<std::string, Heal>> result =
        affectedPlayerVector->getHealsPerAffectedPlayer();

    // Assuming the order is the same as when added which could be untrue.
    EXPECT_EQ(h1.getPotentialDealtOnPlayer(), result[0].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h2.getPotentialDealtOnPlayer(), result[1].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h3.getPotentialDealtOnPlayer(), result[2].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(h4.getPotentialDealtOnPlayer(), result[3].second.getPotentialDealtOnPlayer());
    EXPECT_EQ(4, result.size());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalNano) {
    /* Verifies that getNano is called for each player in the vector and
    that the returned sum is correct. */

    Nano n1 = createNano(10);
    Nano n2 = createNano(30);
    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*caller, getNano())
        .Times(0);

    Nano totalNano = affectedPlayerVector->getTotalNano("Caller");

    EXPECT_EQ((n1 + n2).getTotalDealtOnPlayer(), totalNano.getTotalDealtOnPlayer());
}

TEST_F(AffectedPlayerVectorDamageTest, getNanoPerAffectedPlayer) {
    /* Verifies that the nanos are returned in a sorted list */
    // Add more players to the vector. "dealer1" and "dealer2" have already
    // been added in the SetUp();
    const MockAffectedPlayer* p3 = addPlayerToVector("dealer3", affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("dealer4", affectedPlayerVector);

    Nano n1 = createNano(10);
    Nano n2 = createNano(0);
    Nano n3 = createNano(500000);
    Nano n4 = createNano(3000);

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*p3, getNano())
        .WillOnce(::testing::ReturnRef(n3));
    EXPECT_CALL(*p4, getNano())
        .WillOnce(::testing::ReturnRef(n4));

    std::vector<std::pair<std::string, Nano>> result =
        affectedPlayerVector->getNanoPerAffectedPlayer();

    EXPECT_EQ(n3.getTotalDealtOnPlayer(), result[0].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n4.getTotalDealtOnPlayer(), result[1].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n1.getTotalDealtOnPlayer(), result[2].second.getTotalDealtOnPlayer());
    EXPECT_EQ(n2.getTotalDealtOnPlayer(), result[3].second.getTotalDealtOnPlayer());
    EXPECT_EQ(p3->getName(), result[0].first);
    EXPECT_EQ(p4->getName(), result[1].first);
    EXPECT_EQ(p1->getName(), result[2].first);
    EXPECT_EQ(p2->getName(), result[3].first);
    EXPECT_EQ(4, result.size());
}

TEST(AffectedPlayerVectorTest, getTotalDamageForAllAffectedPlayers) {
    /*
    Adds several players with different damage.
    Calls getTotalDamageForAllAffectedPlayers().
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
    result = affectedPlayerVector.getTotalDamageForAllAffectedPlayers("Caller");

    // Assuming the order is the same as when added which could be untrue.
    EXPECT_EQ(d1.getTotalReceivedFromPlayer(), result[0].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d2.getTotalReceivedFromPlayer(), result[1].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d3.getTotalReceivedFromPlayer(), result[2].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(d4.getTotalReceivedFromPlayer(), result[3].second.getTotalReceivedFromPlayer());
    EXPECT_EQ(4, result.size());
}
