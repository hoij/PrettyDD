#ifdef WIN32
#pragma warning(disable : 4250)
#endif

#include "affected_player_factory.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "nano.h"
#include "player.h"
#include "player_factory_interface.h"
#include "player_interface.h"
#include "player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>


/* Many of these test cases rely on the implementation of Damage/Heal/Nano/XP.
If a test case fails it might be a good idea to verify that the tests for
these classes pass. */


/* Test Player */

class MockPlayer : public virtual PlayerInterface {
public:
    MockPlayer(std::string name, std::shared_ptr<MyTimeInterface> myTime) :
    name(name), myTime(myTime) {
        AffectedPlayerFactoryInterface* affectedPlayerFactory =
            new AffectedPlayerFactory();
        affectedPlayers = new AffectedPlayerVector(affectedPlayerFactory);
    }

    MOCK_METHOD1(add, void(LineInfo& li));
    std::string getName() const { return name; }

    MOCK_CONST_METHOD0(getTotalDamageDealt, Damage(void));
    MOCK_CONST_METHOD0(getTotalDamageReceived, Damage(void));

    MOCK_CONST_METHOD0(getTotalDamageDealtPerType,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getTotalDamageReceivedPerType,
                       std::vector<std::pair<std::string, Damage>>(void));

    MOCK_CONST_METHOD0(getTotalDamageDealtPerAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));
    MOCK_CONST_METHOD0(getTotalDamageReceivedPerAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(void));

    MOCK_CONST_METHOD1(getDamageReceivedPerType,
        std::vector<std::pair<std::string, Damage>>(std::string));
    MOCK_CONST_METHOD1(getDamageDealtPerType,
        std::vector<std::pair<std::string, Damage>>(std::string));

    MOCK_CONST_METHOD0(getTotalHeals, Heal(void));
    MOCK_CONST_METHOD0(getHealsPerAffectedPlayer,
        std::vector<std::pair<std::string, Heal>>(void));
    MOCK_CONST_METHOD1(getHeal, Heal(std::string name));

    MOCK_CONST_METHOD0(getTotalNano, Nano(void));
    MOCK_CONST_METHOD0(getNanoPerAffectedPlayer,
        std::vector<std::pair<std::string, Nano>>(void));
    MOCK_CONST_METHOD1(getNano, Nano(std::string name));

    MOCK_CONST_METHOD0(getNanoPrograms, NanoPrograms&(void));

    MOCK_METHOD0(getXp, XP&(void));

    MOCK_CONST_METHOD0(getTimeActive, std::time_t(void));
    MOCK_CONST_METHOD0(getPauseDuration, std::time_t(void));
    MOCK_CONST_METHOD0(getStartTime, std::time_t(void));
    MOCK_METHOD0(stopTimer, void(void));
    MOCK_METHOD0(resumeTimer, void(void));

    MOCK_CONST_METHOD0(getLongestAffectedPlayerNameLength, size_t(void));
    MOCK_METHOD0(nrOfAffectedPlayers,
        std::vector<AffectedPlayer*>::size_type(void));
private:
    AffectedPlayerVector* affectedPlayers;
    std::shared_ptr<MyTimeInterface> myTime;
    std::string name;
};

class MockPlayerFactory : public PlayerFactoryInterface {
public:
    virtual PlayerInterface* createPlayer(std::string name) {
        return new ::testing::NiceMock<MockPlayer>(name, std::make_shared<MyTime>());
    }
};

/* Helper functions */

Damage createDamage(int amount) {
    LineInfo li;
    li.amount = amount;
    Damage d;
    d.add(li);
    return d;
}

class PlayerVectorTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        PlayerFactoryInterface* mockPlayerFactory = new MockPlayerFactory();
        playerVector = new PlayerVector(
            "PlayerRunningProgram", mockPlayerFactory);
        playerVector->startLogging();
        // Set up the return values
        d1 = createDamage(10);
        d2 = createDamage(30);

        // Add players to the vector.
        p1 = dynamic_cast<MockPlayer*>(addPlayerToVector("dealer1"));
        p2 = dynamic_cast<MockPlayer*>(addPlayerToVector("dealer2"));
    }
    virtual void TearDown() {
        delete playerVector;
    }

    PlayerInterface* addPlayerToVector(std::string name);

    PlayerVector* playerVector;
    MockPlayer* p1;
    MockPlayer* p2;
    Damage d1;
    Damage d2;
};

PlayerInterface* PlayerVectorTest::addPlayerToVector(std::string name) {
    LineInfo li;
    li.dealer_name = name;
    playerVector->addToPlayers(li);
    PlayerInterface* p = playerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return p;
}

/* Test cases */

TEST_F(PlayerVectorTest, reset) {
    // Reset to remove the two players added in the SetUp.
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Reset an empty vector:
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Add two players
    std::string p1Name = "dealerAfterReset1";
    std::string p2Name = "dealerAfterReset2";
    p1 = dynamic_cast<MockPlayer*>(addPlayerToVector(p1Name));
    p2 = dynamic_cast<MockPlayer*>(addPlayerToVector(p2Name));
    EXPECT_EQ(2, playerVector->size());
    EXPECT_EQ(p1Name, p1->getName());
    EXPECT_EQ(p2Name, p2->getName());
}

TEST_F(PlayerVectorTest, getDamageDealtPerType) {
    /* Get damage per type from one existing player.
    Verify that it gets called.
    Get damage per type from nonexisting player.
    Verify that none of the players in the vector are called
    and that an empty vector is instead returned. */

    std::vector<std::pair<std::string, Damage>> v;
    Damage d;
    v.emplace_back("type", d);
    EXPECT_CALL(*p1, getTotalDamageDealtPerType())
        .WillOnce(::testing::Return(v));

    std::vector<std::pair<std::string, Damage>> result
        = playerVector->getDamageDealtPerType(p1->getName());
    EXPECT_EQ(result[0].first, "type");


    result = playerVector->getDamageDealtPerType("nonexistingPlayer");
    EXPECT_CALL(*p1, getTotalDamageDealtPerType())
        .Times(0);
    EXPECT_CALL(*p1, getTotalDamageDealtPerType())
        .Times(0);
    EXPECT_EQ(0, result.size());
}

TEST_F(PlayerVectorTest, getTotalDamageForEachPlayer) {
    /*
    Adds several players.
    Make p1 and p2 return empty damages (meaning
    they should not be included in the vector).
    Gets total damage received per player.
    Verifie that each player is called and that the
    returned vector contains players that have returned non-empty
    damages.
    */

    // Add players to the vector.
    const MockPlayer* p3 = dynamic_cast<MockPlayer*>(addPlayerToVector("Receiver1"));
    const MockPlayer* p4 = dynamic_cast<MockPlayer*>(addPlayerToVector("Receiver2"));
    const MockPlayer* p5 = dynamic_cast<MockPlayer*>(addPlayerToVector("Receiver3"));
    const MockPlayer* p6 = dynamic_cast<MockPlayer*>(addPlayerToVector("Receiver4"));

    // Set up the return values.

    Damage d3 = createDamage(7000);
    Damage d4 = createDamage(0);
    Damage emptyDamage;

    EXPECT_CALL(*p1, getTotalDamageReceived())
        .WillOnce(::testing::Return(emptyDamage));
    EXPECT_CALL(*p2, getTotalDamageReceived())
        .WillOnce(::testing::Return(emptyDamage));
    EXPECT_CALL(*p3, getTotalDamageReceived())
        .WillOnce(::testing::Return(d1));
    EXPECT_CALL(*p4, getTotalDamageReceived())
        .WillOnce(::testing::Return(d2));
    EXPECT_CALL(*p5, getTotalDamageReceived())
        .WillOnce(::testing::Return(d3));
    EXPECT_CALL(*p6, getTotalDamageReceived())
        .WillOnce(::testing::Return(d4));

    std::vector<std::pair<std::string, Damage>> result;
    result = playerVector->getTotalDamageReceivedPerPlayer();

    Damage d;
    for (const auto& p : result) {
        d += p.second;
    }
    EXPECT_EQ(4, result.size());
    EXPECT_EQ((d1 + d2 + d3 + d4).getTotal(), d.getTotal());
}

TEST_F(PlayerVectorTest, stopAndStartLogging) {
    /* Stop logging
    Verify that each players stopTimer() gets called
    Add new player
    Verify that it wasn't added
    Stop logging
    Verify that each players stopTimer() is NOT called
    Add new player
    Verify that it wasn't added

    Start logging
    Verify that each players getStartTime() gets called
    Add new player
    Verify it gets added
    Start Logging
    Verify that each players getStartTime() is NOT called
    Add new player
    Verify that it gets added
    */

    LineInfo li1;
    li1.dealer_name = "dealer3";
    LineInfo li2;
    li2.dealer_name = "dealer4";

    EXPECT_CALL(*p1, stopTimer());
    EXPECT_CALL(*p2, stopTimer());
    playerVector->stopLogging();

    playerVector->addToPlayers(li1);
    EXPECT_EQ(2, playerVector->size());

    EXPECT_CALL(*p1, stopTimer())
        .Times(0);
    EXPECT_CALL(*p2, stopTimer())
        .Times(0);
    playerVector->stopLogging();

    playerVector->addToPlayers(li1);
    EXPECT_EQ(2, playerVector->size());

    // Have to expect call to getStartTime instead
    // of resumeTimer() as getStartTime is checked for 0
    // before allowing a call to resumeTimer. And
    // startTime is 0 in this case as no stats have been added.
    EXPECT_CALL(*p1, getStartTime());
    EXPECT_CALL(*p2, getStartTime());
    playerVector->startLogging();

    playerVector->addToPlayers(li1);
    EXPECT_EQ(3, playerVector->size());

    EXPECT_CALL(*p1, getStartTime())
        .Times(0);
    EXPECT_CALL(*p2, getStartTime())
        .Times(0);
    playerVector->startLogging();

    playerVector->addToPlayers(li2);
    EXPECT_EQ(4, playerVector->size());
}
