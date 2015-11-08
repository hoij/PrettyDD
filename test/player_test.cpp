#include "affected_player.h"
#include "affected_player_factory.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "my_time_interface.h"
#include "player.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>


class MockAffectedPlayerVector : public AffectedPlayerVector {
public:
    MockAffectedPlayerVector(AffectedPlayerFactoryInterface* affectedPlayerFactory) :
        AffectedPlayerVector(affectedPlayerFactory) {}

    MOCK_METHOD1(addToPlayers, void(LineInfo& li));

    MOCK_CONST_METHOD0(getLongestNameLength, size_t());

    MOCK_CONST_METHOD1(getTotalDamageReceivedFromPlayer, Damage(std::string callerName));
    MOCK_CONST_METHOD1(getTotalDamageDealtOnPlayer, Damage(std::string callerName));

    MOCK_CONST_METHOD2(getTotalReceivedFromPlayerPerDamageType,
                       Damage(std::string callerName,
                              const std::string damageType));
    MOCK_CONST_METHOD2(getTotalDealtOnPlayerPerDamageType,
                       Damage(std::string callerName,
                              const std::string damageType));

    MOCK_CONST_METHOD1(getTotalDamageReceivedFromPlayerPerAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string callerName));

    MOCK_CONST_METHOD1(getTotalDamageDealtOnPlayerPerAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string callerName));

    MOCK_CONST_METHOD1(getDamageReceivedFromPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string name));

    MOCK_CONST_METHOD1(getDamageDealtOnPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string name));

    MOCK_METHOD1(getTotalHeals, Heal(std::string callerName));

    MOCK_CONST_METHOD0(getHealsPerAffectedPlayer,
                       std::vector<std::pair<std::string, Heal>>(void));

    MOCK_CONST_METHOD1(getTotalNano, Nano(std::string callerName));

    MOCK_CONST_METHOD1(getNanoPerAffectedPlayer,
                       std::vector<std::pair<std::string, Nano>>(
                           std::string callerName));
};

class MockMyTime : public MyTimeInterface {
public:
    MOCK_METHOD0(currentTime, std::time_t(void));
    MOCK_METHOD0(currentTimeString, std::string(void));
};

class PlayerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // mockAffectedPlayerVector and mockMyTime will be deleted in Player
        mockMyTime = std::make_shared<::testing::NiceMock<MockMyTime>>();
        AffectedPlayerFactoryInterface* affectedPlayerFactory =
            new AffectedPlayerFactory();
        mockAffectedPlayerVector =
            new MockAffectedPlayerVector(affectedPlayerFactory);
        player = new Player("You",
                            mockAffectedPlayerVector,
                            mockMyTime);

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 1000;
        li2.amount = 3000;
        d1.add(li1);
        d2.add(li2);
    }
    virtual void TearDown() {
        delete player;
    }
    Player* player;
    MockAffectedPlayerVector* mockAffectedPlayerVector;
    std::shared_ptr<::testing::NiceMock<MockMyTime>> mockMyTime;

    Damage d1;
    Damage d2;

    std::time_t startTime = 1442318667;
    std::time_t stopTime = startTime + 180;
    std::time_t resumeTime = startTime + 780;
    std::time_t pauseDuration = resumeTime - stopTime; // 600
    std::time_t DPMTime = startTime + 800;
    std::time_t stopTime2 = startTime + 820;
    std::time_t resumeTime2 = startTime + 886;
    std::time_t pauseDuration2 = pauseDuration +
                                 resumeTime2 -
                                 stopTime2; // 666
};

bool operator==(const LineInfo& lhs, const LineInfo& rhs) {
    return lhs.dealer_name == rhs.dealer_name &&
    lhs.receiver_name == rhs.receiver_name &&
    lhs.type == rhs.type &&
    lhs.subtype == rhs.subtype &&
    lhs.nanoProgramName == rhs.nanoProgramName;
}

TEST_F(PlayerTest, timerTest_normalUsage) {
    /* Add LineInfo.
    "Stop" time.
    Resume time.
    Add another LineInfo.
    Verify the active time.
    Stop time a second time.
    Resume time a second time.
    Verify the active time.
    Add a third LineInfo
    Verify the active time.
    */
    LineInfo li1;
    li1.type = "damage";
    li1.time = startTime;
    LineInfo li2;
    li2.type = "damage";
    li2.time = resumeTime;
    LineInfo li3;
    li3.type = "damage";
    li3.time = resumeTime2 + 70;

    // Before adding anything to the player the active time should be 0.
    EXPECT_EQ(0, player->getTimeActive());

    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li1))
        .Times(1);
    player->add(li1);
    // Active time is 0 until a line with a different time is added.
    EXPECT_EQ(0, player->getTimeActive());

    // Stop and resume
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    player->stopTimer();
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(resumeTime));
    player->resumeTimer();

    // Add a new line with the same time stamp as the resume time
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li2))
        .Times(1);
    player->add(li2);
    EXPECT_EQ(stopTime - startTime, player->getTimeActive());

    // Stop and resume a second time
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime2));
    player->stopTimer();
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(resumeTime2));
    player->resumeTimer();

    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li3))
        .Times(1);
    player->add(li3);
    std::time_t expected = stopTime - startTime +
                           stopTime2 - resumeTime +
                           li3.time - resumeTime2;
    EXPECT_EQ(expected, player->getTimeActive());
}

TEST_F(PlayerTest, timerTest_instantStopResume) {
    /* Stop and resume time on the same second */
    LineInfo li;
    li.type = "damage";
    li.time = startTime;

    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);

    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    player->stopTimer();

    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    player->resumeTimer();

    EXPECT_EQ(0, player->getTimeActive());
}

TEST_F(PlayerTest, amountPerMinute) {

    /* Test when the time is not stopped */
    player->startTime = startTime;
    player->timeOfLastAction = DPMTime;

    // The active time is 200 s.
    int expected1 = (int)(300001/((float)800/60));
    EXPECT_EQ(expected1, player->amountPerMinute(300001));

    /* Start and stop time are the same => an active time of 0. */
    player->stopTime = startTime;
    player->timeOfLastAction = startTime;
    EXPECT_EQ(0, player->amountPerMinute(2000));
}

TEST_F(PlayerTest, add_damage) {
    LineInfo li;
    li.type = "damage";
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_heal) {
    LineInfo li;
    li.type = "heal";
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nano) {
    LineInfo li;
    li.type = "nano";
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nanoCast) {
    /* This test uses the real class NanoProgram.
    Add a "nano cast" with a named nano.
    Expect the nano program name list to be empty (the nano is saved to a
    local variable).
    Add another "nano cast" with an empty nano name.
    Expect the nano program name list to contain one nano,
    the named nano added previously. */
    LineInfo li1;
    li1.type = "nano cast";
    li1.nanoProgramName = "Test Program";
    player->add(li1);
    EXPECT_EQ(0, player->getNanoPrograms().getNanoProgramNames().size());

    LineInfo li2;
    li2.type = "nano cast";
    li2.subtype = "execute";
    li2.nanoProgramName = "";
    player->add(li2);
    EXPECT_EQ(1, player->getNanoPrograms().getNanoProgramNames().size());
    std::string storedNanoName =
        player->getNanoPrograms().getNanoProgramNames()[0];
    EXPECT_EQ(li1.nanoProgramName, storedNanoName);
}

TEST_F(PlayerTest, add_xp) {
    /* This test uses the real class XP.
    Test that a LineInfo object of type xp can be added successfully. */
    LineInfo li;
    li.type = "xp";
    li.subtype = "gained";
    li.amount = 123456;
    player->add(li);
    EXPECT_EQ(li.amount, player->getXp().getTotalGained("xp"));
}

TEST_F(PlayerTest, getTotalDamageDealt) {
    /* Verifies that getTotalDamageDealt() calls AffectedPlayerVectors
    getTotalDamageReceivedFromPlayer() */

    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalDamageReceivedFromPlayer(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage dealt = player->getTotalDamageDealt();

    EXPECT_EQ(d1.getTotal(), dealt.getTotal());
}

TEST_F(PlayerTest, getTotalDamageDealtPerAffectedPlayer) {
    /* Verifies that correct method in AffectedPlayerVector
    is called. */

    std::vector<std::pair<std::string, Damage>> totalDamageForAllAffectedPlayers;
    totalDamageForAllAffectedPlayers.emplace_back("AffectedPlayer1", d1);

    EXPECT_CALL(
        *mockAffectedPlayerVector,
        getTotalDamageReceivedFromPlayerPerAffectedPlayer(player->getName()))
        .WillOnce(::testing::Return(totalDamageForAllAffectedPlayers));

    std::vector<std::pair<std::string, Damage>> result =
        player->getTotalDamageDealtPerAffectedPlayer();
}

TEST_F(PlayerTest, getDamageDealtPerType) {
    /* Verifies that correct method in AffectedPlayerVector
    is called. */

    std::string affectedPlayerName = "AffectedPlayer1";
    std::vector<std::pair<std::string, Damage>> damageFromAffectedPlayer;
    damageFromAffectedPlayer.emplace_back("type", d1);

    EXPECT_CALL(*mockAffectedPlayerVector,
        getDamageReceivedFromPlayer(affectedPlayerName))
        .WillOnce(::testing::Return(damageFromAffectedPlayer));

    std::vector<std::pair<std::string, Damage>> result =
        player->getDamageDealtPerType(affectedPlayerName);

    EXPECT_EQ(damageFromAffectedPlayer[0].second.getTotal(),
              result[0].second.getTotal());
}

TEST_F(PlayerTest, getTotalHeals) {
    /* Verifies that getTotalHeals() calls
    PlayerVectors getTotalHeals. */

    LineInfo li;
    li.amount = 10;
    Heal h;
    h.addHealDealtOnPlayer(li);

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalHeals(player->getName()))
        .WillOnce(::testing::Return(h));

    Heal result = player->getTotalHeals();

    EXPECT_EQ(h.getPotentialDealtOnPlayer(), result.getPotentialDealtOnPlayer());
}
