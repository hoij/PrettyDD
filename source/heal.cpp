#include "logger.h"
#include "heal.h"
#include "line_info.h"

#include <string>

Heal& Heal::operator+=(const Heal& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    return *this;
}

void Heal::addHealDealtOnPlayer(LineInfo& li) {
    // The dealt amount is always a potential heal
    // but it will be verified here just to make sure.
    if (li.subtype == "potential") {
        Heal::HealInfo hi;
        hi.count = 1;
        hi.potential = li.amount;
        hi.potentialMax = li.amount;
        hi.potentialMin = li.amount;

        dealtOnPlayer += hi;
    }
    else {
        errorLog.write("Error: addHealDealtOnPlayer called with subtype " +
                       '"' + li.subtype + '"' +
                       " when it should have been \"potential\"");
    }
}

void Heal::addHealReceivedFromPlayer(LineInfo& li) {
    if (li.subtype == "actual") {
        // Only ever called for AffectedPlayer "You" as the actual amount
        // healed is only available in log lines for yourself.
        Heal::HealInfo hi;
        hi.count = 1;
        hi.actual = li.amount;
        hi.actualMax = li.amount;
        hi.actualMin = li.amount;

        receivedFromPlayer += hi;
    }
    else if (li.subtype == "potential") {
        // This will either be the total amount that you have received from
        // Player or the total amount that you have given to the receiver.
        Heal::HealInfo hi;
        hi.count = 1;
        hi.potential = li.amount;
        hi.potentialMax = li.amount;
        hi.potentialMin = li.amount;

        receivedFromPlayer += hi;
    }
}

Heal::HealInfo& Heal::HealInfo::operator+=(const HealInfo& rhs) {
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
