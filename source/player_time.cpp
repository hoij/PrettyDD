#include "my_time.h"
#include "player_time.h"


std::time_t PlayerTime::getTimeActive() const {
    // Start time is set the first time the add method of a new
    // Player is called.
    return timeOfLastAction - startTime - getPauseDuration();
}

std::time_t PlayerTime::getPauseDuration() const {
    std::time_t pauseDuration = 0;
    for (const Pause& pause : pauses) {
        if (timeOfLastAction >= pause.stop) {
            pauseDuration += pause.stop - pause.start;
        }
    }
    return pauseDuration;
}

void PlayerTime::setTimeOfLastAction(std::time_t t) {
    if (startTime == 0) {
        // Set the start time on the first action made.
        startTime = t;
    }
    timeOfLastAction = t;
}

void PlayerTime::stopTimer() {
    stopTime = myTime->currentTime();
}

void PlayerTime::resumeTimer() {
    Pause p = {stopTime, myTime->currentTime()};
    pauses.push_back(p);
    stopTime = 0;
}

int PlayerTime::amountPerMinute(int amount) const {
    /* Calculates the amount per minute based on the
     * active time */
    std::time_t t = getTimeActive();
    if (t == 0) {
        return 0;
    }
    else {
        return int(amount / ((double)t / 60));
    }
}
