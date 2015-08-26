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
    MockAffectedPlayer(std::string name) : AffectedPlayer(name) {}
    MOCK_CONST_METHOD0(getTotalDamage, Damage(void));
    MOCK_CONST_METHOD0(getTotalRegularDamage, Damage(void));
    MOCK_CONST_METHOD0(getTotalNanobotsDamage, Damage(void));
    MOCK_CONST_METHOD1(getTotalDamagePerDamageType, Damage(const std::string damageType));
    MOCK_CONST_METHOD1(getTotalRegularDamagePerDamageType, Damage(const std::string damageType));
    MOCK_CONST_METHOD1(getTotalNanobotsDamagePerDamageType, Damage(const std::string damageType));
    MOCK_CONST_METHOD0(getRegularDamage, std::map<std::string, Damage>&(void));
    MOCK_CONST_METHOD0(getNanobotsDamage, std::map<std::string, Damage>&(void));
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

Heal createHeal(int amount, std::string playerType) {
    LineInfo li;
    li.amount = amount;
    Heal h;
    h.add(li, playerType);
    return h;
}

Nano createNano(int amount, std::string playerType) {
    LineInfo li;
    li.amount = amount;
    Nano n;
    n.add(li, playerType);
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
        d1 = createDealerDamage(10);
        d2 = createDealerDamage(30);

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

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamage) {
    /*
    Adds the a player with the same name as the caller to the vector.
    Calls getTotalDamage().
    Verifies that each players, except for the callers, getTotalDamage
    is in turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getTotalDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamage())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamage())
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamage("Caller");

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalRegularDamage) {
    /*
    Adds the a player with the same name as the caller to the vector.
    Calls getTotalRegularDamage().
    Verifies that each players, except for the callers, getTotalRegularDamage
    is in turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getTotalRegularDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalRegularDamage())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalRegularDamage())
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalRegularDamage("Caller");

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalNanobotsDamage) {
    /*
    Adds the a player with the same name as the caller to the vector.
    Calls getTotalNanonbotsDamage().
    Verifies that each players, except for the callers, getTotalNanobotsDamage
    is in turn called and that the summed damage is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getTotalNanobotsDamage())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalNanobotsDamage())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalNanobotsDamage())
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalNanobotsDamage("Caller");

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalDamagePerDamageType) {
    /*
    Adds the a player with the same name as the caller to the vector and
    calls getTotalRegularDamagePerDamageType().
    Verifies that each players, except for the callers,
    getTotalDamagePerDamageType is called and that the summed damage is
    correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamagePerDamageType(damageType))
        .Times(0);

    Damage totalDamage =
        affectedPlayerVector->getTotalDamagePerDamageType("Caller",
                                                          damageType);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalRegularDamagePerDamageType) {
    /*
    Adds the a player with the same name as the caller to the vector and
    calls getTotalRegularDamagePerDamageType().
    Verifies that each players, except for the callers,
    getTotalRegularDamagePerDamageType is called and that the summed damage is
    correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalRegularDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalRegularDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalRegularDamagePerDamageType(damageType))
        .Times(0);

    Damage totalDamage =
        affectedPlayerVector->getTotalRegularDamagePerDamageType("Caller",
                                                                 damageType);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalNanobotsDamagePerDamageType) {
    /*
    Adds the a player with the same name as the caller to the vector and
    calls getTotalNanobotsDamagePerDamageType().
    Verifies that each players, except for the callers,
    getTotalNanobotsDamagePerDamageType is called and that the summed damage
    is correct.
    */

    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType))
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*caller, getTotalDamagePerDamageType(damageType))
        .Times(0);

    Damage totalDamage = affectedPlayerVector->getTotalDamagePerDamageType(
                             "Caller", damageType);

    EXPECT_EQ((d1 + d2).getTotalDealt(), totalDamage.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getNanobotsDamageFromAffectedPlayer) {
    /* Verifies that getNanobotsDamage is called on the the correct player */

    std::map<std::string, Damage> expected;
    expected["type"] = d2;

    EXPECT_CALL(*p2, getNanobotsDamage())
        .WillOnce(::testing::ReturnRef(expected));

    std::map<std::string, Damage> result2 =
        affectedPlayerVector->getNanobotsDamageFromAffectedPlayer("dealer2");

    EXPECT_EQ(1, result2.count("type"));
}

TEST_F(AffectedPlayerVectorDamageTest, getRegularDamageFromAffectedPlayer) {
    /* Verifies that getRegularDamage is called on the the correct player */

    std::map<std::string, Damage> expected;
    expected["type"] = d2;

    EXPECT_CALL(*p2, getRegularDamage())
        .WillOnce(::testing::ReturnRef(expected));

    std::map<std::string, Damage> result2 =
        affectedPlayerVector->getRegularDamageFromAffectedPlayer("dealer2");

    EXPECT_EQ(1, result2.count("type"));
}

TEST_F(AffectedPlayerVectorDamageTest, getRegularDamageFromAffectedPlayer_notFound) {
    /* Verifies that the correct exception is thrown when a player is not
    found in the vector */

    std::string name = "NotInTheVector";

    EXPECT_THROW(affectedPlayerVector->getRegularDamageFromAffectedPlayer(name),
                 std::invalid_argument);

    try {
        affectedPlayerVector->getRegularDamageFromAffectedPlayer(name);
    }
    catch(const std::invalid_argument& e) {
        EXPECT_EQ(e.what(), "\"" + name + "\" was not found among the " +
                            "affected players.");
    }
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalHeals) {
    /* Verifies that getHeal is called for each player in the vector and
    that the returned sum is correct. */

    Heal h1 = createHeal(10, "dealer");
    Heal h2 = createHeal(30, "dealer");
    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*caller, getHeal())
        .Times(0);

    Heal totalHeal = affectedPlayerVector->getTotalHeals("Caller");

    EXPECT_EQ((h1 + h2).getPotentialDealt(), totalHeal.getPotentialDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getHealsForAllAffectedPlayers) {
    /*
    This test case depends on the implementation of Heal.
    If it fails, make sure that Heals's tests can pass.

    Calls getHealsForAllAffectedPlayers().
    Verifies that each players getHeal is in turn called and that
    the returned vector is sorted on potential heal dealt.
    */

    // Add more players to the vector. "dealer1" and "dealer2" have already
    // been added in the SetUp();
    const MockAffectedPlayer* p3 = addPlayerToVector("dealer3", affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("dealer4", affectedPlayerVector);

    Heal h1 = createHeal(10, "dealer");
    Heal h2 = createHeal(0, "dealer");
    Heal h3 = createHeal(500000, "dealer");
    Heal h4 = createHeal(3000, "dealer");

    EXPECT_CALL(*p1, getHeal())
        .WillOnce(::testing::ReturnRef(h1));
    EXPECT_CALL(*p2, getHeal())
        .WillOnce(::testing::ReturnRef(h2));
    EXPECT_CALL(*p3, getHeal())
        .WillOnce(::testing::ReturnRef(h3));
    EXPECT_CALL(*p4, getHeal())
        .WillOnce(::testing::ReturnRef(h4));

    std::vector<std::pair<std::string, Heal>> result =
        affectedPlayerVector->getHealsForAllAffectedPlayers();

    EXPECT_EQ(h3.getPotentialDealt(), result[0].second.getPotentialDealt());
    EXPECT_EQ(h4.getPotentialDealt(), result[1].second.getPotentialDealt());
    EXPECT_EQ(h1.getPotentialDealt(), result[2].second.getPotentialDealt());
    EXPECT_EQ(h2.getPotentialDealt(), result[3].second.getPotentialDealt());
    EXPECT_EQ(4, result.size());
}

TEST_F(AffectedPlayerVectorDamageTest, getTotalNano) {
    /* Verifies that getNano is called for each player in the vector and
    that the returned sum is correct. */

    Nano n1 = createNano(10, "dealer");
    Nano n2 = createNano(30, "dealer");
    const MockAffectedPlayer* caller = addPlayerToVector("Caller",
                                                         affectedPlayerVector);

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*caller, getNano())
        .Times(0);

    Nano totalNano = affectedPlayerVector->getTotalNano("Caller");

    EXPECT_EQ((n1 + n2).getTotalDealt(), totalNano.getTotalDealt());
}

TEST_F(AffectedPlayerVectorDamageTest, getNanoForAllAffectedPlayers) {
    /* Verifies that the nanos are returned in a sorted list */
    // Add more players to the vector. "dealer1" and "dealer2" have already
    // been added in the SetUp();
    const MockAffectedPlayer* p3 = addPlayerToVector("dealer3", affectedPlayerVector);
    const MockAffectedPlayer* p4 = addPlayerToVector("dealer4", affectedPlayerVector);

    Nano n1 = createNano(10, "dealer");
    Nano n2 = createNano(0, "dealer");
    Nano n3 = createNano(500000, "dealer");
    Nano n4 = createNano(3000, "dealer");

    EXPECT_CALL(*p1, getNano())
        .WillOnce(::testing::ReturnRef(n1));
    EXPECT_CALL(*p2, getNano())
        .WillOnce(::testing::ReturnRef(n2));
    EXPECT_CALL(*p3, getNano())
        .WillOnce(::testing::ReturnRef(n3));
    EXPECT_CALL(*p4, getNano())
        .WillOnce(::testing::ReturnRef(n4));

    std::vector<std::pair<std::string, Nano>> result =
        affectedPlayerVector->getNanoForAllAffectedPlayers();

    EXPECT_EQ(n3.getTotalDealt(), result[0].second.getTotalDealt());
    EXPECT_EQ(n4.getTotalDealt(), result[1].second.getTotalDealt());
    EXPECT_EQ(n1.getTotalDealt(), result[2].second.getTotalDealt());
    EXPECT_EQ(n2.getTotalDealt(), result[3].second.getTotalDealt());
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

    EXPECT_EQ(d3.getTotalReceived(), result[0].second.getTotalReceived());
    EXPECT_EQ(d1.getTotalReceived(), result[1].second.getTotalReceived());
    EXPECT_EQ(d4.getTotalReceived(), result[2].second.getTotalReceived());
    EXPECT_EQ(d2.getTotalReceived(), result[3].second.getTotalReceived());
    EXPECT_EQ(4, result.size());
}
