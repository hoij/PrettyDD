#include "my_time.h"

#include <chrono>


std::time_t MyTime::currentTime() {
    /* Returns time in s since 1970 */
    std::chrono::time_point<std::chrono::system_clock> time_p =
        std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(time_p);
}

std::string MyTime::currentTimeString() {
    std::time_t time = currentTime();
    return std::ctime(&time);  // ctime converts to readable string
}
