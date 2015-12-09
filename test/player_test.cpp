#include "affected_player.h"
#include "affected_player_factory.h"
#include "affected_player_vector.h"
#include "damage.h"
#include "definitions.h"
#include "heal.h"
#include "line_info.h"
#include "my_time_interface.h"
#include "player.h"
#include "player_time.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>


class MockAffectedPlayerVector : public AffectedPlayerVector {
public:
    MockAffectedPlayerVector(std::unique_ptr<AffectedPlayerFactoryInterface> affectedPlayerFactory) :
        AffectedPlayerVector(std::move(affectedPlayerFactory)) {}

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
        std::unique_ptr<AffectedPlayerFactoryInterface>
            affectedPlayerFactory(new AffectedPlayerFactory());
        mockAffectedPlayerVector =
            std::make_shared<MockAffectedPlayerVector>(std::move(affectedPlayerFactory));
        mockMyTime = std::make_shared<::testing::NiceMock<MockMyTime>>();
        std::unique_ptr<PlayerTime> playerTime(new PlayerTime(mockMyTime));
        player = std::unique_ptr<Player>(new Player(
                            "You",
                            mockAffectedPlayerVector,
                            std::move(playerTime)));

        // Set up the return values.
        LineInfo li1;
        LineInfo li2;
        li1.amount = 1000;
        li2.amount = 3000;
        d1.add(li1);
        d2.add(li2);
    }
    virtual void TearDown() {}

    std::unique_ptr<Player> player;
    std::shared_ptr<MockAffectedPlayerVector> mockAffectedPlayerVector;
    std::shared_ptr<::testing::NiceMock<MockMyTime>> mockMyTime;

    Damage d1;
    Damage d2;
};

bool operator==(const LineInfo& lhs, const LineInfo& rhs) {
    return lhs.dealer_name == rhs.dealer_name &&
    lhs.receiver_name == rhs.receiver_name &&
    lhs.type == rhs.type &&
    lhs.subtype == rhs.subtype &&
    lhs.nanoProgramName == rhs.nanoProgramName;
}

TEST_F(PlayerTest, add_time) {
    /* Verifies that the time info is propagated to the
    PlayerTime instance. Note: Uses PlayerTime
    directly instead of a mock. */

    std::time_t t = 1442318667;
    LineInfo li;
    li.time = t;

    // Adding a type and expect all just to avoid the
    // unexpected call warning.
    li.type = LineType::damage;
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);

    player->add(li);
    EXPECT_EQ(t, player->getStartTime());
}

TEST_F(PlayerTest, add_damage) {
    LineInfo li;
    li.type = LineType::damage;
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_heal) {
    LineInfo li;
    li.type = LineType::heal;
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nano) {
    LineInfo li;
    li.type = LineType::nano;
    EXPECT_CALL(*mockAffectedPlayerVector, addToPlayers(li))
        .Times(1);
    player->add(li);
}

TEST_F(PlayerTest, add_nanoCast) {
    /* This test uses the real class NanoProgram.
    Add a nano cast with a named nano.
    Expect the nano program name list to be empty (the nano is saved to a
    local variable).
    Add another nano cast with an empty nano name.
    Expect the nano program name list to contain one nano,
    the named nano added previously. */
    LineInfo li1;
    li1.type = LineType::nanoCast;
    li1.nanoProgramName = "Test Program";
    player->add(li1);
    EXPECT_EQ(0, player->getNanoPrograms().getNanoProgramNames().size());

    LineInfo li2;
    li2.type = LineType::nanoCast;
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
    li.type = LineType::xp;
    li.subtype = "gained";
    li.amount = 123456;
    player->add(li);
    EXPECT_EQ(li.amount, player->getXp().getTotalGained(LineType::xp));
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
