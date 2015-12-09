#include "my_time_interface.h"
#include "player_time.h"

#include <gmock/gmock.h>


class MockMyTime : public MyTimeInterface {
public:
    MOCK_METHOD0(currentTime, std::time_t(void));
    MOCK_METHOD0(currentTimeString, std::string(void));
};

class PlayerTimeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        mockMyTime = std::make_shared<::testing::NiceMock<MockMyTime>>();
        playerTime = std::unique_ptr<PlayerTime>(
            new PlayerTime(mockMyTime));
    }

    std::unique_ptr<PlayerTime> playerTime = nullptr;
    std::shared_ptr<::testing::NiceMock<MockMyTime>> mockMyTime = nullptr;

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

TEST_F(PlayerTimeTest, normalUsage) {
    /* Add LineInfo.
     *    Start time.
     *    Verify the active time.
     *    "Stop" time.
     *    Resume time.
     *    Set timeOfLastAction.
     *    Verify the active time.
     *    Stop time a second time.
     *    Resume time a second time.
     *    Verify the active time.
     *    Set timeOfLastAction.
     *    Verify the active time.
     */
    
    // Set the start time.
    playerTime->setTimeOfLastAction(startTime);

    // Before adding anything the active time should be 0.
    EXPECT_EQ(0, playerTime->getTimeActive());

    playerTime->setTimeOfLastAction(startTime);
    // Active time remains 0 until a second time is set.
    EXPECT_EQ(0, playerTime->getTimeActive());

    // Stop and resume
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    playerTime->stopTimer();
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(resumeTime));
    playerTime->resumeTimer();

    // Active time still remains 0 until a second time is set.
    EXPECT_EQ(0, playerTime->getTimeActive());
    
    // Set a second time and verify that the time active.
    playerTime->setTimeOfLastAction(resumeTime);
    EXPECT_EQ(stopTime - startTime, playerTime->getTimeActive());
    
    // Stop and resume a second time
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime2));
    playerTime->stopTimer();
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(resumeTime2));
    playerTime->resumeTimer();

    // No change to the active time.
    EXPECT_EQ(stopTime - startTime, playerTime->getTimeActive());

    // Set a new timeOfLastAction.
    playerTime->setTimeOfLastAction(resumeTime2 + 70);
    std::time_t expected = stopTime - startTime +
                           stopTime2 - resumeTime +
                           resumeTime2 + 70 - resumeTime2;
    EXPECT_EQ(expected, playerTime->getTimeActive());
}

TEST_F(PlayerTimeTest, instantStopResume) {
    /* Stop and resume time on the same second */

    playerTime->setTimeOfLastAction(startTime);

    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    playerTime->stopTimer();

    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(stopTime));
    playerTime->resumeTimer();

    EXPECT_EQ(0, playerTime->getTimeActive());
}

TEST_F(PlayerTimeTest, amountPerMinute) {
    /* TODO: Move to Damage */
    
    /* Test when the time is not stopped */
    playerTime->setTimeOfLastAction(startTime);
    playerTime->setTimeOfLastAction(DPMTime);
    
    // The active time is 200 s.
    int expected1 = (int)(300001/((float)800/60));
    EXPECT_EQ(expected1, playerTime->amountPerMinute(300001));
    
    /* Change start and stop time to be the same =>
       an active time of 0. */
    EXPECT_CALL(*mockMyTime, currentTime())
        .WillOnce(::testing::Return(startTime));
    playerTime->stopTimer();
    playerTime->setTimeOfLastAction(startTime);
    EXPECT_EQ(0, playerTime->amountPerMinute(2000));
}

/* Stop several times in a row */
