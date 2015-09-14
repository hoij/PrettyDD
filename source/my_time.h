#ifndef MY_TIME_H
#define MY_TIME_H


#include <chrono>
#include <ctime>
#include <string>

#ifdef WIN32
#pragma warning(disable : 4996)  // Disable ctime unsafe warning
#endif

class MyTime {
public:
    std::time_t currentTime();
    std::string currentTimeString();
};


#endif  // MY_TIME_H
