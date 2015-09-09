#ifndef NANO_H
#define NANO_H

#include "line_info.h"

#include <limits>
#include <string>


class Nano {
public:
    Nano& operator+=(const Nano& rhs);

    void addNanoDealtOnPlayer(LineInfo& li);
    void addNanoReceivedFromPlayer(LineInfo& li);

    int getTotalDealtOnPlayer() const {return dealtOnPlayer.total;}
    int getCountDealtOnPlayer() const {return dealtOnPlayer.count;}
    int getMaxDealtOnPlayer() const {return dealtOnPlayer.max;}
    int getMinDealtOnPlayer() const {return dealtOnPlayer.min;}

    int getTotalReceivedFromPlayer() const {return receivedFromPlayer.total;}
    int getCountReceivedFromPlayer() const {return receivedFromPlayer.count;}
    int getMaxReceivedFromPlayer() const {return receivedFromPlayer.max;}
    int getMinReceivedFromPlayer() const {return receivedFromPlayer.min;}

private:
    struct NanoInfo {
        NanoInfo& operator+=(const NanoInfo& rhs);

        int total = 0;
        int count = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

    NanoInfo dealtOnPlayer;
    NanoInfo receivedFromPlayer;
};


inline Nano operator+(Nano lhs, const Nano& rhs) {
    return lhs += rhs;
}

#endif  // NANO_H
