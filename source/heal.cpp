#include "logger.h"
#include "heal.h"
#include "line_info.h"


Heal& Heal::operator+=(const Heal& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    return *this;
}

Heal operator+(Heal lhs, const Heal& rhs) {
    return lhs += rhs;
}

void Heal::addHealDealtOnPlayer(LineInfo& li) {
    // The dealt amount is always a potential heal
    // but it will be verified here just to make sure.
    if (li.subtype == "potential") {
        dealtOnPlayer.count++;
        dealtOnPlayer.potential += li.amount;
        if (li.amount < dealtOnPlayer.potentialMin) {
            dealtOnPlayer.potentialMin = li.amount;
        }
        if (li.amount > dealtOnPlayer.potentialMax) {
            dealtOnPlayer.potentialMax = li.amount;
        }
    }
    else {
        errorLog.write("Error: addHealDealtOnPlayer called with subtype ", false);
        errorLog.write("\"" + li.subtype + "\"" + " when it should have been \"potential\"");
    }
}

void Heal::addHealReceivedFromPlayer(LineInfo& li) {
    receivedFromPlayer.count++;
    if (li.subtype == "actual") {
        // Only ever called for AffectedPlayer "You" as the actual amount
        // healed is only available in log lines for yourself.
        receivedFromPlayer.actual += li.amount;
        if (li.amount < receivedFromPlayer.actualMin) {
            receivedFromPlayer.actualMin = li.amount;
        }
        if (li.amount > receivedFromPlayer.actualMax) {
            receivedFromPlayer.actualMax = li.amount;
        }
    }
    else if (li.subtype == "potential") {
        // This will either be the total amount that you have received from
        // Player or the total amount that you have given to the receiver.
        receivedFromPlayer.potential += li.amount;
        if (li.amount < receivedFromPlayer.potentialMin) {
            receivedFromPlayer.potentialMin = li.amount;
        }
        if (li.amount > receivedFromPlayer.potentialMax) {
            receivedFromPlayer.potentialMax = li.amount;
        }
    }
}

Heal::healInfo& Heal::healInfo::operator+=(const healInfo& rhs) {
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
