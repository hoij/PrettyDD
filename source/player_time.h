#ifndef PLAYER_TIME_H
#define PLAYER_TIME_H


#include "my_time_interface.h"

#include <ctime>
#include <memory>
#include <vector>


class PlayerTime {
public:
    PlayerTime(std::shared_ptr<MyTimeInterface> myTime)
    : myTime(myTime) {}

    std::time_t getTimeActive() const;
    std::time_t getStartTime() const {return startTime;}
    void setTimeOfLastAction(std::time_t t);
    void stopTimer();
    void resumeTimer();
    int amountPerMinute(int amount) const;

private:
    std::time_t getPauseDuration() const;

    std::shared_ptr<MyTimeInterface> myTime;
    std::time_t startTime = 0;
    std::time_t timeOfLastAction = 0;
    std::time_t stopTime = 0;
    struct Pause {
        std::time_t start;
        std::time_t stop;
    };
    std::vector<Pause> pauses;
};


#endif  // PLAYER_TIME_H