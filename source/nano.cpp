#include "nano.h"

#include <limits>
#include <string>


Nano& Nano::operator+=(const Nano& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    return *this;
}

void Nano::addNanoDealtOnPlayer(LineInfo& li) {
    Nano::NanoInfo ni;
    ni.count = 1;
    ni.max = li.amount;
    ni.min = li.amount;
    ni.total = li.amount;

    dealtOnPlayer += ni;
}

void Nano::addNanoReceivedFromPlayer(LineInfo& li) {
    Nano::NanoInfo ni;
    ni.count = 1;
    ni.max = li.amount;
    ni.min = li.amount;
    ni.total = li.amount;

    receivedFromPlayer += ni;
}

Nano::NanoInfo& Nano::NanoInfo::operator+=(const NanoInfo& rhs) {
    total += rhs.total;
    count += rhs.count;
    if (rhs.max > max) {
        max = rhs.max;
    }
    if (rhs.min < min) {
        min = rhs.min;
    }
    return *this;
}
