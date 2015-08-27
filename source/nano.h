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

    int getTotalDealt() const {return dealtOnPlayer.total;}
    int getCountDealt() const {return dealtOnPlayer.count;}
    int getMaxDealt() const {return dealtOnPlayer.max;}
    int getMinDealt() const {return dealtOnPlayer.min;}

    int getTotalReceived() const {return receivedFromPlayer.total;}
    int getCountReceived() const {return receivedFromPlayer.count;}
    int getMaxReceived() const {return receivedFromPlayer.max;}
    int getMinReceived() const {return receivedFromPlayer.min;}

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
