#ifndef HEAL_H
#define HEAL_H


#include "line_info.h"

#include <limits>
#include <string>


class Heal {
public:
    Heal& operator+=(const Heal& rhs) {
        dealtOnPlayer += rhs.dealtOnPlayer;
        receivedFromPlayer += rhs.receivedFromPlayer;
        return *this;
    }

    void add(LineInfo& li, std::string playerType) {
        if (playerType == "dealer") {
            dealtOnPlayer.count++;
            // The dealer amount is always the potential heal
            // This will be either the total amount the dealer has given to
            // you or the total amount you have given to the receiver.
            // TODO: What? The above makes no sense.
            dealtOnPlayer.potential += li.amount;
            // Check min/max
            if (li.amount < dealtOnPlayer.potentialMin) {
                dealtOnPlayer.potentialMin = li.amount;
            }
            if (li.amount > dealtOnPlayer.potentialMax) {
                dealtOnPlayer.potentialMax = li.amount;
            }
        }
        else if (playerType == "receiver") {
            receivedFromPlayer.count++;
            if (li.subtype == "actual") {
                // Only available for "You".
                receivedFromPlayer.actual += li.amount;
                // Check min/max
                if (li.amount < receivedFromPlayer.actualMin) {
                    receivedFromPlayer.actualMin = li.amount;
                }
                if (li.amount > receivedFromPlayer.actualMax) {
                    receivedFromPlayer.actualMax = li.amount;
                }
            }
            else if (li.subtype == "potential") {
                // This will be either the total amount you have receivedFromPlayer or
                // the total amount you have given to the receiver.
                receivedFromPlayer.potential += li.amount;
                // Check min/max
                if (li.amount < receivedFromPlayer.potentialMin) {
                    receivedFromPlayer.potentialMin = li.amount;
                }
                if (li.amount > dealtOnPlayer.potentialMax) {
                    receivedFromPlayer.potentialMax = li.amount;
                }
            }
        }
    }

int getActualDealt() const {return dealtOnPlayer.actual;}
int getPotentialDealt() const {return dealtOnPlayer.potential;}
int getCountDealt() const {return dealtOnPlayer.count;}
int getActualMaxDealt() const {return dealtOnPlayer.actualMax;}
int getActualMinDealt() const {return dealtOnPlayer.actualMin;}
int getPotentialMaxDealt() const {return dealtOnPlayer.potentialMax;}
int getPotentialMinDealt() const {return dealtOnPlayer.potentialMin;}

int getActualReceived() const {return receivedFromPlayer.actual;}
int getPotentialReceived() const {return receivedFromPlayer.potential;}
int getCountReceived() const {return receivedFromPlayer.count;}
int getActualMaxReceived() const {return receivedFromPlayer.actualMax;}
int getActualMinReceived() const {return receivedFromPlayer.actualMin;}
int getPotentialMaxReceived() const {return receivedFromPlayer.potentialMax;}
int getPotentialMinReceived() const {return receivedFromPlayer.potentialMin;}

private:
    struct healInfo {
        healInfo& operator+=(const healInfo& rhs) {
            count += rhs.count;
            actual += rhs.actual;
            potential += rhs.potential;
            if (rhs.actualMin < actualMin) {
                actualMin = rhs.actualMin;
            }
            if (rhs.actualMax > actualMax) {
                actualMax = rhs.actualMax;
            }
            if (rhs.potentialMin < potentialMin) {
                potentialMin = rhs.potentialMin;
            }
            if (rhs.potentialMax > potentialMax) {
                potentialMax = rhs.potentialMax;
            }

            return *this;
        }

        int count = 0;
        int actual = 0;
        int potential = 0;
        int actualMin = std::numeric_limits<int>::max();
        int actualMax = -1;
        int potentialMin = std::numeric_limits<int>::max();
        int potentialMax = -1;
    };

    healInfo dealtOnPlayer;
    healInfo receivedFromPlayer;
};

inline Heal operator+(Heal lhs, const Heal& rhs) {
    return lhs += rhs;
}


#endif  // HEAL_H
