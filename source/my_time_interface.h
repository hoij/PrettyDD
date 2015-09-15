#ifndef MY_TIME_INTERFACE_H
#define MY_TIME_INTERFACE_H


#include <ctime>
#include <string>

#ifdef WIN32
#pragma warning(disable : 4996)  // Disable ctime unsafe warning
#endif

class MyTimeInterface {
public:
    virtual ~MyTimeInterface() {}
    virtual std::time_t currentTime() = 0;
    virtual std::string currentTimeString() = 0;
};


#endif  // MY_TIME_INTERFACE_H

