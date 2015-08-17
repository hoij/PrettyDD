#include "heal.h"
#include "damage.h"
#include "line_info.h"
#include "player.h"
#include "player_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Test Players */

class StubPlayer : public Player {
public:
    StubPlayer(std::string name) : Player(name) {}
    void add(LineInfo& li) {
        (void)li;
        callsToAdd++;
    }
    int callsToAdd = 0;
};

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

void addToPlayers(std::string dealer,
                  std::string receiver,
                  PlayerVector<StubPlayer*>* playerVector) {
    LineInfo li;
    li.dealer_name = dealer;
    li.receiver_name = receiver;
    playerVector->addToPlayers(li);
    // Verify that they get added to the vector
    if (dealer != "") {
        const StubPlayer* p = playerVector->getPlayer(dealer);
        EXPECT_FALSE(p == nullptr);
    }
    if (receiver != "") {
        const StubPlayer* p = playerVector->getPlayer(receiver);
        EXPECT_FALSE(p == nullptr);
    }
}

Damage createDamage(LineInfo li, std::string playerType) {
    Damage d;
    d.add(li, playerType);
    return d;
}

class PlayerVectorStubPlayerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector<StubPlayer*>;
    }
    virtual void TearDown() {
        delete playerVector;
    }
    PlayerVector<StubPlayer*>* playerVector;
};

class PlayerVectorDamageTest : public ::testing::Test {
/* NiceMock suppresses warnings about function calls not expected via an
EXPECT_CALL(). */
protected:
    virtual void SetUp() {
        playerVector = new PlayerVector<::testing::NiceMock<MockPlayer>*>;

        // Set up the return value.
        LineInfo li;
        d.add(li, "dealer");

        // Add players to the vector.
        p1 = addPlayerToVector("dealer1", playerVector);
        p2 = addPlayerToVector("dealer2", playerVector);
    }
    virtual void TearDown() {
        delete playerVector;
    }

    PlayerVector<::testing::NiceMock<MockPlayer>*>* playerVector;
    Damage d;
    const MockPlayer* p1;
    const MockPlayer* p2;
};

/* Test cases */

TEST_F(PlayerVectorStubPlayerTest, noDealerNorReceiver) {
    /* Adds a line info object with no dealer and no receiver.
    Verifies that the total nr of players is 0. */
    addToPlayers("", "", playerVector);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        (void)player;
        nrOfPlayers++;
    }
    EXPECT_EQ(0, nrOfPlayers);
}


TEST_F(PlayerVectorStubPlayerTest, newDealerOnly) {
    /* Adds a line info object with only the dealer name defined
    Verifies that only the dealer is added. */
    addToPlayers("Dealer", "", playerVector);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Dealer", player->getName());
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorStubPlayerTest, newReceiverOnly) {
    /* Adds a line info object with only the receiver name defined
    Verifies that only the receiver is added. */
    addToPlayers("", "Receiver", playerVector);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Receiver", player->getName());
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorStubPlayerTest, newDealerAndReceiver) {
    /*Adds a line info object with both dealer and receiver names defined.
    Verifies that they are unique and that the total nr of players is 2. */
    addToPlayers("Dealer", "Receiver", playerVector);

    int nrOfPlayers = 0;
    bool isDealerFound = false;
    bool isReceiverFound = false;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        if (player->getName() == "Dealer" && !isDealerFound) {
            isDealerFound = true;
        }
        else if (player->getName() == "Receiver" && !isReceiverFound) {
            isReceiverFound = true;
        }
    }

    EXPECT_EQ(2, nrOfPlayers);
    EXPECT_TRUE(isDealerFound);
    EXPECT_TRUE(isReceiverFound);
}

TEST_F(PlayerVectorStubPlayerTest, existingDealer) {
    /*Adds an identical line info object, with dealer name defined, twice.
    Verifies that the dealer info is added to the existing dealer.
    Also verifies that the added players add method gets called twice. */
    addToPlayers("Dealer", "", playerVector);
    addToPlayers("Dealer", "", playerVector);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Dealer", player->getName());
        EXPECT_EQ(2, player->callsToAdd);
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorStubPlayerTest, existingReceiver) {
    /* Adds an identical line info object, with receiver name defined, twice.
    Verifies that the receiver info is added to the existing receiver.
    Also verifies that the added players add method gets called twice. */
    addToPlayers("", "Receiver", playerVector);
    addToPlayers("", "Receiver", playerVector);

    int nrOfPlayers = 0;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        EXPECT_EQ("Receiver", player->getName());
        EXPECT_EQ(2, player->callsToAdd);
    }
    EXPECT_EQ(1, nrOfPlayers);
}

TEST_F(PlayerVectorStubPlayerTest, existingDealerAndReceiver) {
    /*
    Adds an identical line info object, with receiver and dealer name
    defined, twice.
    Verifies that only one of each player is added.
    */

    addToPlayers("Dealer", "Receiver", playerVector);
    addToPlayers("Dealer", "Receiver", playerVector);

    int nrOfPlayers = 0;
    bool isDealerFound = false;
    bool isReceiverFound = false;
    for (const auto& player : *playerVector) {
        nrOfPlayers++;
        if (player->getName() == "Dealer" && !isDealerFound) {
            isDealerFound = true;
        }
        else if (player->getName() == "Receiver" && !isReceiverFound) {
            isReceiverFound = true;
        }
    }
    EXPECT_EQ(2, nrOfPlayers);
    EXPECT_TRUE(isDealerFound);
    EXPECT_TRUE(isReceiverFound);
}

TEST_F(PlayerVectorStubPlayerTest, getLongestNameLength) {
    /* Adds eigth players and gets the longest name length. */
    std::string theLongestName = "AReallyReallyLongName OfSomeone";
    addToPlayers("Name", "Name2", playerVector);
    addToPlayers("AName2", "AName", playerVector);
    addToPlayers("A Really Long Name Of Someone", theLongestName, playerVector);
    addToPlayers("ALongName", "AReallyReallyLongNameOfSomeone", playerVector);

    EXPECT_EQ(theLongestName.length(), playerVector->getLongestNameLength());
}

TEST_F(PlayerVectorStubPlayerTest, getPlayer) {
    /* Adds players and uses the getPlayer method to return them.
    Verifies that they are correct by comparing their names. */
    addToPlayers("Dealer", "Receiver", playerVector);
    addToPlayers("Dealer2", "Receiver2", playerVector);

    EXPECT_EQ("Dealer", playerVector->getPlayer("Dealer")->getName());
    EXPECT_EQ("Dealer2", playerVector->getPlayer("Dealer2")->getName());
    EXPECT_EQ("Receiver", playerVector->getPlayer("Receiver")->getName());
    EXPECT_EQ("Receiver2", playerVector->getPlayer("Receiver2")->getName());
}

TEST_F(PlayerVectorDamageTest, getTotalDamage_regular) {
    /*
    This test case will depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamage(false) to retreive the regular damage.
    Verifies that each players getTotalDamage is in turn called and that
    the summed damage is correct. The sum is verified by checking number
    of dealt attacks (getCountDealt()).
    */

    EXPECT_CALL(*p1, getTotalDamage(false))
        .WillOnce(::testing::Return(d));
    EXPECT_CALL(*p2, getTotalDamage(false))
        .WillOnce(::testing::Return(d));

    Damage totalDamage = playerVector->getTotalDamage(false);
    EXPECT_EQ(2, totalDamage.getCountDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamage_nanobots) {
    // TODO: Keep two tests for regular/nanonbots or just have one?
    /*
    This test case will depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamage(true) to retreive the nanobot damage.
    Verifies that each players getTotalDamage is in turn called and that
    the summed damage is correct. The sum is verified by checking number
    of dealt attacks (getCountDealt()).
    */

    EXPECT_CALL(*p1, getTotalDamage(true))
        .WillOnce(::testing::Return(d));
    EXPECT_CALL(*p2, getTotalDamage(true))
        .WillOnce(::testing::Return(d));

    Damage totalDamage = playerVector->getTotalDamage(true);
    EXPECT_EQ(2, totalDamage.getCountDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamagePerDamageType_regular) {
    /*
    This test case will depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamagePerDamageType("poison", false) to retreive the regular
    damage.
    Verifies that each players getTotalDamagePerDamageType is in turn
    called and that the summed damage is correct. The sum is verified by
    checking number of dealt attacks (getCountDealt()).
    */

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, false))
        .WillOnce(::testing::Return(d));

    Damage totalDamage = playerVector->getTotalDamagePerDamageType(damageType, false);
    EXPECT_EQ(2, totalDamage.getCountDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalDamagePerDamageType_nanobots) {
    /*
    This test case will depends on the implementation of Damage.
    If it fails it's a good idea to make sure that Damage's tests can pass.

    Calls getTotalDamagePerDamageType("poison", false) to retreive the nanobot
    damage.
    Verifies that each players getTotalDamagePerDamageType is in turn
    called and that the summed damage is correct. The sum is verified by
    checking number of dealt attacks (getCountDealt()).
    */

    std::string damageType = "poison";
    EXPECT_CALL(*p1, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d));
    EXPECT_CALL(*p2, getTotalDamagePerDamageType(damageType, true))
        .WillOnce(::testing::Return(d));

    Damage totalDamage = playerVector->getTotalDamagePerDamageType(damageType, true);
    EXPECT_EQ(2, totalDamage.getCountDealt());
}

TEST_F(PlayerVectorDamageTest, getTotalHeals) {
    /*
    This test case will depends on the implementation of Heal.
    If it fails it's a good idea to make sure that Heals's tests can pass.

    Calls getTotalHeals().
    Verifies that each players getTotalHeal is in turn called and that
    the summed heal is correct. The sum is verified by checking number
    of heals (getCountDealt()).
    */

    LineInfo li;
    Heal h;
    h.add(li, "dealer");

    EXPECT_CALL(*p1, getTotalHeals())
        .WillOnce(::testing::Return(h));
    EXPECT_CALL(*p2, getTotalHeals())
        .WillOnce(::testing::Return(h));

    Heal totalHeal = playerVector->getTotalHeals();
    EXPECT_EQ(2, totalHeal.getCountDealt());
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
    LineInfo li1;
    LineInfo li2;
    LineInfo li3;
    LineInfo li4;
    li1.amount = 7000;
    li2.amount = 0;
    li3.amount = 5000000;
    li4.amount = 15000;
    Damage d1 = createDamage(li1, "receiver");
    Damage d2 = createDamage(li2, "receiver");
    Damage d3 = createDamage(li3, "receiver");
    Damage d4 = createDamage(li4, "receiver");

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

    EXPECT_EQ(li3.amount, result[0].second.getTotalReceived());
    EXPECT_EQ(li4.amount, result[1].second.getTotalReceived());
    EXPECT_EQ(li1.amount, result[2].second.getTotalReceived());
    EXPECT_EQ(li2.amount, result[3].second.getTotalReceived());
}
