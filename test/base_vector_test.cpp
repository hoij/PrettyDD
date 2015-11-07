#include "line_info.h"
#include "player.h"
#include "base_vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>


/* Test Players */

class StubPlayer : public Player {
public:
    StubPlayer(std::string name, std::shared_ptr<MyTimeInterface> myTime) : Player(name, myTime) {}
    void add(LineInfo& li) {
        (void)li;
        callsToAdd++;
    }
    int callsToAdd = 0;
};

/* Helper functions */

void addToPlayers(std::string dealer,
                  std::string receiver,
                  BaseVector<StubPlayer*>* playerVector) {
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

class BaseVectorTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        playerVector = new BaseVector<StubPlayer*>;
    }
    virtual void TearDown() {
        delete playerVector;
    }
    BaseVector<StubPlayer*>* playerVector;
};

/* Test cases */

TEST_F(BaseVectorTest, noDealerNorReceiver) {
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


TEST_F(BaseVectorTest, newDealerOnly) {
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

TEST_F(BaseVectorTest, newReceiverOnly) {
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

TEST_F(BaseVectorTest, newDealerAndReceiver) {
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

TEST_F(BaseVectorTest, existingDealer) {
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

TEST_F(BaseVectorTest, existingReceiver) {
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

TEST_F(BaseVectorTest, existingDealerAndReceiver) {
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

TEST_F(BaseVectorTest, getLongestNameLength) {
    /* Adds eigth players and gets the longest name length. */
    std::string theLongestName = "AReallyReallyLongName OfSomeone";
    addToPlayers("Name", "Name2", playerVector);
    addToPlayers("AName2", "AName", playerVector);
    addToPlayers("A Really Long Name Of Someone", theLongestName, playerVector);
    addToPlayers("ALongName", "AReallyReallyLongNameOfSomeone", playerVector);

    EXPECT_EQ(theLongestName.length(), playerVector->getLongestNameLength());
}

TEST_F(BaseVectorTest, getPlayer) {
    /* Adds players and uses the getPlayer method to return them.
    Verifies that they are correct by comparing their names. */
    addToPlayers("Dealer", "Receiver", playerVector);
    addToPlayers("Dealer2", "Receiver2", playerVector);

    EXPECT_EQ("Dealer", playerVector->getPlayer("Dealer")->getName());
    EXPECT_EQ("Dealer2", playerVector->getPlayer("Dealer2")->getName());
    EXPECT_EQ("Receiver", playerVector->getPlayer("Receiver")->getName());
    EXPECT_EQ("Receiver2", playerVector->getPlayer("Receiver2")->getName());
}
