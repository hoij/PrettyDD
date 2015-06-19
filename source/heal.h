#ifndef HEAL_H
#define HEAL_H


#include <string>
#include <limits>
#include "line_info.h"

class Heal {
public:
    void add(LineInfo& li, std::string playerType) {
        if (playerType == "dealer") {
            dealt.count++;
            // The dealer amount is always the potential heal
            // This will be either the total amount the dealer has given to
            // you or the total amount you have given to the receiver.
            dealt.potential += li.amount;
            // Check min/max
            if (li.amount < dealt.potentialMin) {
                dealt.potentialMin = li.amount;
            }
            if (li.amount > dealt.potentialMax) {
                dealt.potentialMax = li.amount;
            }
        }
        else if (playerType == "receiver") {
            received.count++;
            if (li.subtype == "actual") {
                // Only available for "You".
                received.actual += li.amount;
                // Check min/max
                if (li.amount < received.actualMin) {
                    received.actualMin = li.amount;
                }
                if (li.amount > received.actualMax) {
                    received.actualMax = li.amount;
                }
            }
            else if (li.subtype == "potential") {
                // This will be either the total amount you have received or
                // the total amount you have given to the receiver.
                received.potential += li.amount;
                // Check min/max
                if (li.amount < received.potentialMin) {
                    received.potentialMin = li.amount;
                }
                if (li.amount > dealt.potentialMax) {
                    dealt.potentialMax = li.amount;
                }
            }
        }
    }

private:
    struct healInfo {
        int count = 0;
        int actual = 0;
        int potential = 0;
        int actualMin = std::numeric_limits<int>::max();
        int actualMax = -1;
        int potentialMin = std::numeric_limits<int>::max();
        int potentialMax = -1;
    };

    healInfo dealt;
    healInfo received;
};


#endif  // HEAL_H
