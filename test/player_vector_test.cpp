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
class MockPlayer : public PlayerInterface {
public:
    MockPlayer(std::string name, std::unique_ptr<PlayerTime> playerTime) :
    name(name), playerTime(std::move(playerTime)) {
        std::unique_ptr<AffectedPlayerFactoryInterface>
            affectedPlayerFactory(new AffectedPlayerFactory());
        affectedPlayers = std::make_shared<AffectedPlayerVector>(std::move(affectedPlayerFactory));
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

    MOCK_CONST_METHOD0(getStartTime, std::time_t(void));
    MOCK_CONST_METHOD0(getTimeActive, std::time_t(void));
    MOCK_METHOD0(stopTimer, void(void));
    MOCK_METHOD0(resumeTimer, void(void));

    MOCK_CONST_METHOD0(getLongestAffectedPlayerNameLength, size_t(void));
    MOCK_METHOD0(nrOfAffectedPlayers,
        std::vector<AffectedPlayer*>::size_type(void));
private:
    std::string name;
    std::shared_ptr<AffectedPlayerVector> affectedPlayers;
    std::unique_ptr<PlayerTime> playerTime;
};

class MockPlayerFactory : public PlayerFactoryInterface {
public:
    virtual std::unique_ptr<PlayerInterface> createPlayer(std::string name) {
        std::shared_ptr<MyTimeInterface> myTime = std::make_shared<MyTime>();
        std::unique_ptr<PlayerTime> playerTime(new PlayerTime(myTime));

		MockPlayer* mp = new MockPlayer(name, std::move(playerTime));
		// The player vector should only create a new player if it
		// has something to add to it.
		EXPECT_CALL(*mp, add(::testing::_))
			.Times(1);

		return std::unique_ptr<PlayerInterface>(mp);
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

/* Test fixture */

class PlayerVectorTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector(
            "PlayerRunningProgram",
            std::unique_ptr<PlayerFactoryInterface>(new MockPlayerFactory()));
        playerVector->startLogging();
        // Set up standard return values
        d1 = createDamage(10);
        d2 = createDamage(30);
    }
    virtual void TearDown() {
        delete playerVector;
    }

    MockPlayer* addDealerToVector(std::string name);
    MockPlayer* addReceiverToVector(std::string name);
    void addPlayersToVector(std::string dealer,
                            std::string receiver);

    PlayerVector* playerVector;
    Damage d1;
    Damage d2;
};

MockPlayer* PlayerVectorTest::addDealerToVector(std::string name) {
    LineInfo li;
    li.dealer_name = name;
    playerVector->addToPlayers(li);
    PlayerInterface* p = playerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return dynamic_cast<MockPlayer*>(p);
}

MockPlayer* PlayerVectorTest::addReceiverToVector(std::string name) {
    LineInfo li;
    li.receiver_name = name;
    playerVector->addToPlayers(li);
    PlayerInterface* p = playerVector->getPlayer(name);
    EXPECT_FALSE(p == nullptr);
    return dynamic_cast<MockPlayer*>(p);
}

void PlayerVectorTest::addPlayersToVector(std::string dealer,
                                          std::string receiver) {
    LineInfo li;
    li.dealer_name = dealer;
    li.receiver_name = receiver;
    playerVector->addToPlayers(li);
    // Verify that they've been added to the vector
    if (dealer != "") {
        PlayerInterface* p = playerVector->getPlayer(dealer);
        EXPECT_FALSE(p == nullptr);
    }
    if (receiver != "") {
        PlayerInterface* p = playerVector->getPlayer(receiver);
        EXPECT_FALSE(p == nullptr);
    }
}

/* Test cases */

TEST_F(PlayerVectorTest, addToPlayers_noDealerNorReceiver) {
    /* Adds a line info object with no dealer and no receiver.
    Verifies that the total nr of players has not increased. */
    addPlayersToVector("", "");
    EXPECT_EQ(0, playerVector->size());
}

TEST_F(PlayerVectorTest, addToPlayers_newDealerOnly) {
    /* Adds a line info object with only the dealer name defined.
    Verifies that only the dealer is added. */
    std::string dealerName = "Dealer";    
    addPlayersToVector(dealerName, "");
    EXPECT_EQ(1, playerVector->size());
}

TEST_F(PlayerVectorTest, addToPlayers_newReceiverOnly) {
    /* Adds a line info object with only the receiver name defined.
    Verifies that only the receiver is added. */
    std::string receiverName = "Receiver";
    addPlayersToVector(receiverName, "");
    EXPECT_EQ(1, playerVector->size());
}

TEST_F(PlayerVectorTest, addToPlayers_newDealerAndReceiver) {
    /* Adds a line info object with both dealer and receiver names defined.
    Verifies that they are unique and that the total nr of players is 2. */
    std::string dealerName = "Dealer";
    std::string receiverName = "Receiver";
    addPlayersToVector(dealerName, receiverName);
    EXPECT_EQ(2, playerVector->size());
}

TEST_F(PlayerVectorTest, addToPlayers_existingDealer) {
    /* Adds an identical line info object, with dealer name defined, twice.
    Verifies that the dealer info is added to the existing dealer.
    Also verifies that the added players add method gets called twice. */

    // The first time a new player will be created.
    MockPlayer* dealer1 = addDealerToVector("Dealer");
    // The second time the add method of the previously
    // created player will be called.
    EXPECT_CALL(*dealer1, add(::testing::_))
        .Times(1);
    MockPlayer* dealer2 = addDealerToVector("Dealer");

    EXPECT_EQ(dealer1, dealer2);
    EXPECT_EQ(1, playerVector->size());
}

TEST_F(PlayerVectorTest, getLongestNameLength) {
    /* Adds eigth players and gets the longest name length. */
    std::string theLongestName = "AReallyReallyLongName OfSomeone";
    addPlayersToVector("Name", "Name2");
    addPlayersToVector("AName2", "AName");
    addPlayersToVector("A Really Long Name Of Someone", theLongestName);
    addPlayersToVector("ALongName", "AReallyReallyLongNameOfSomeone");

    EXPECT_EQ(theLongestName.length(), playerVector->getLongestNameLength());
}

TEST_F(PlayerVectorTest, reset) {
    // Add players to the vector.
    PlayerInterface* p1 = addDealerToVector("dealer1");
    PlayerInterface* p2 = addDealerToVector("dealer2");
    // Reset to remove them
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Reset an empty vector:
    playerVector->reset();
    EXPECT_EQ(0, playerVector->size());

    // Add two players to make sure it still
    // can be done.
    std::string p1Name = "dealerAfterReset1";
    std::string p2Name = "dealerAfterReset2";
    p1 = addDealerToVector(p1Name);
    p2 = addDealerToVector(p2Name);
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

    // Add a player to the vector.
    MockPlayer* p1 = dynamic_cast<MockPlayer*>(addDealerToVector("dealer1"));

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
    // These players being delaer or receivers has no real effect
    // on the test.
    MockPlayer* p1 = dynamic_cast<MockPlayer*>(addDealerToVector("Dealer1"));
    MockPlayer* p2 = dynamic_cast<MockPlayer*>(addDealerToVector("Dealer2"));
    MockPlayer* p3 = dynamic_cast<MockPlayer*>(addReceiverToVector("Receiver1"));
    MockPlayer* p4 = dynamic_cast<MockPlayer*>(addReceiverToVector("Receiver2"));
    MockPlayer* p5 = dynamic_cast<MockPlayer*>(addReceiverToVector("Receiver3"));
    MockPlayer* p6 = dynamic_cast<MockPlayer*>(addReceiverToVector("Receiver4"));

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

    // Add players to the vector.
    MockPlayer* p1 = dynamic_cast<MockPlayer*>(addDealerToVector("dealer1"));
    MockPlayer* p2 = dynamic_cast<MockPlayer*>(addDealerToVector("dealer2"));

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
