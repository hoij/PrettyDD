#ifndef HEAL_H
#define HEAL_H


#include <limits>

class LineInfo;


class Heal {
public:
    Heal& operator+=(const Heal& rhs);

    void addHealDealtOnPlayer(LineInfo& li);
    void addHealReceivedFromPlayer(LineInfo& li);

    int getActualDealtOnPlayer() const {return dealtOnPlayer.actual;}
    int getPotentialDealtOnPlayer() const {return dealtOnPlayer.potential;}
    int getCountDealtOnPlayer() const {return dealtOnPlayer.count;}
    int getActualMinDealtOnPlayer() const {return dealtOnPlayer.actualMin;}
    int getActualMaxDealtOnPlayer() const {return dealtOnPlayer.actualMax;}
    int getPotentialMinDealtOnPlayer() const {return dealtOnPlayer.potentialMin;}
    int getPotentialMaxDealtOnPlayer() const {return dealtOnPlayer.potentialMax;}

    int getActualReceivedFromPlayer() const {return receivedFromPlayer.actual;}
    int getPotentialReceivedFromPlayer() const {return receivedFromPlayer.potential;}
    int getCountReceivedFromPlayer() const {return receivedFromPlayer.count;}
    int getActualMinReceivedFromPlayer() const {return receivedFromPlayer.actualMin;}
    int getActualMaxReceivedFromPlayer() const {return receivedFromPlayer.actualMax;}
    int getPotentialMinReceivedFromPlayer() const {return receivedFromPlayer.potentialMin;}
    int getPotentialMaxReceivedFromPlayer() const {return receivedFromPlayer.potentialMax;}

private:
    struct HealInfo {
        HealInfo& operator+=(const HealInfo& rhs);

        int actual = 0;
        int potential = 0;
        int count = 0;
        int actualMin = std::numeric_limits<int>::max();
        int actualMax = -1;
        int potentialMin = std::numeric_limits<int>::max();
        int potentialMax = -1;
    };

    HealInfo dealtOnPlayer;
    HealInfo receivedFromPlayer;
};


inline Heal operator+(Heal lhs, const Heal& rhs) {
    return lhs += rhs;
}


#endif  // HEAL_H
