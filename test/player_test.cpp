#include "affected_player.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "heal.h"
#include "line_info.h"
#include "my_time_interface.h"
#include "player.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


class MockAffectedPlayerVector : public AffectedPlayerVector<AffectedPlayer*> {
public:
    MOCK_METHOD1(addToPlayers, void(LineInfo& li));

    MOCK_CONST_METHOD0(getLongestNameLength, size_t());

    MOCK_CONST_METHOD1(getTotalDamage, Damage(std::string callerName));

    MOCK_CONST_METHOD2(getTotalDamagePerDamageType,
                       Damage(std::string callerName,
                              const std::string damageType));

    MOCK_CONST_METHOD1(getTotalDamageForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string callerName));

    MOCK_CONST_METHOD1(getAllDamageFromAffectedPlayer,
                       std::vector<std::pair<std::string, Damage>>(
                           std::string name));

    MOCK_METHOD1(getTotalHeals, Heal(std::string callerName));

    MOCK_CONST_METHOD0(getHealsForAllAffectedPlayers,
                       std::vector<std::pair<std::string, Heal>>(void));

    MOCK_CONST_METHOD1(getTotalNano, Nano(std::string callerName));

    MOCK_CONST_METHOD1(getNanoForAllAffectedPlayers,
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
        mockAffectedPlayerVector = new MockAffectedPlayerVector();
        mockMyTime = new ::testing::NiceMock<MockMyTime>;
        player = new Player("You", mockAffectedPlayerVector, mockMyTime);

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 1000;
        li2.amount = 3000;
        d1.addDamageDealtOnPlayer(li1);
        d2.addDamageDealtOnPlayer(li2);
    }
    virtual void TearDown() {
        delete player;
    }
    Player* player;
    MockAffectedPlayerVector* mockAffectedPlayerVector;
    ::testing::NiceMock<MockMyTime>* mockMyTime;

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

TEST_F(PlayerTest, timerTest) {
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

TEST_F(PlayerTest, instantStopResume) {
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
    Expect the nanoPrograms vector to be empty (the nano is saved to a
    local variable).
    Add another "nano cast" with an empty nano name.
    Expect the nanoPrograms vector to contain one nano,
    the named nano added previously. */
    LineInfo li1;
    li1.type = "nano cast";
    li1.nanoProgramName = "Test Program";
    player->add(li1);
    EXPECT_EQ(0, player->getNanoPrograms().size());

    LineInfo li2;
    li2.type = "nano cast";
    li2.nanoProgramName = "";
    player->add(li2);
    EXPECT_EQ(1, player->getNanoPrograms().size());
    std::string storedNanoName = player->getNanoPrograms()[0].getName();
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

TEST_F(PlayerTest, getTotalDamage) {
    /* Verifies that getTotalDamage() calls AffectedPlayerVectors
    getTotalDamage() */

    EXPECT_CALL(*mockAffectedPlayerVector, getTotalDamage(player->getName()))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamage();

    EXPECT_EQ(d1.getTotalDealtOnPlayer(), result.getTotalDealtOnPlayer());
}

TEST_F(PlayerTest, getTotalDamagePerDamageType) {
    /* Verifies that getTotalDamagePerDamageType() calls AffectedPlayerVectors
    getTotalRegularDamagePerDamageType() and that the damage is summed.*/

    std::string damageType = "chemical";
    EXPECT_CALL(*mockAffectedPlayerVector,
                getTotalDamagePerDamageType(player->getName(), damageType))
        .WillOnce(::testing::Return(d1));

    Damage result = player->getTotalDamagePerDamageType(damageType);

    EXPECT_EQ(d1.getTotalDealtOnPlayer(), result.getTotalDealtOnPlayer());
}

TEST_F(PlayerTest, getTotalDamageForAllAffectedPlayers) {
    /* Verifies that getTotalDamageForAllAffectedPlayers() calls
    PlayerVectors getTotalDamageForAllAffectedPlayers(std::string callerName)
    */

    std::vector<std::pair<std::string, Damage>> totalDamageForAllAffectedPlayers;
    totalDamageForAllAffectedPlayers.emplace_back("AffectedPlayer1", d1);

    EXPECT_CALL(*mockAffectedPlayerVector,
        getTotalDamageForAllAffectedPlayers(player->getName()))
        .WillOnce(::testing::Return(totalDamageForAllAffectedPlayers));

    std::vector<std::pair<std::string, Damage>> result =
        player->getTotalDamageForAllAffectedPlayers();
}

TEST_F(PlayerTest, getAllDamageFromAffectedPlayer) {
    /* Verifies that getAllDamageFromAffectedPlayer() calls
    PlayerVectors getAllDamageFromAffectedPlayer(std::string callerName).
    */

    std::string name = "AffectedPlayer1";
    std::vector<std::pair<std::string, Damage>> damageFromAffectedPlayer;
    damageFromAffectedPlayer.emplace_back(name, d1);

    EXPECT_CALL(*mockAffectedPlayerVector,
        getAllDamageFromAffectedPlayer(name))
        .WillOnce(::testing::Return(damageFromAffectedPlayer));

    std::vector<std::pair<std::string, Damage>> result =
        player->getAllDamageFromAffectedPlayer(name);

    EXPECT_EQ(damageFromAffectedPlayer[0].second.getTotalDealtOnPlayer(),
              result[0].second.getTotalDealtOnPlayer());
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
