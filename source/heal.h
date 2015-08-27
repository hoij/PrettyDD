#ifndef HEAL_H
#define HEAL_H


#include <limits>
#include <string>

class LineInfo;


class Heal {
public:
    Heal& operator+=(const Heal& rhs);

    void addHealDealtOnPlayer(LineInfo& li);
    void addHealReceivedFromPlayer(LineInfo& li);

    int getActualDealt() const {return dealtOnPlayer.actual;}
    int getPotentialDealt() const {return dealtOnPlayer.potential;}
    int getCountDealt() const {return dealtOnPlayer.count;}
    int getActualMinDealt() const {return dealtOnPlayer.actualMin;}
    int getActualMaxDealt() const {return dealtOnPlayer.actualMax;}
    int getPotentialMinDealt() const {return dealtOnPlayer.potentialMin;}
    int getPotentialMaxDealt() const {return dealtOnPlayer.potentialMax;}

    int getActualReceived() const {return receivedFromPlayer.actual;}
    int getPotentialReceived() const {return receivedFromPlayer.potential;}
    int getCountReceived() const {return receivedFromPlayer.count;}
    int getActualMinReceived() const {return receivedFromPlayer.actualMin;}
    int getActualMaxReceived() const {return receivedFromPlayer.actualMax;}
    int getPotentialMinReceived() const {return receivedFromPlayer.potentialMin;}
    int getPotentialMaxReceived() const {return receivedFromPlayer.potentialMax;}

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
