#include "damage.h"
#include "line_info.h"

#include <iomanip>
#include <limits>
#include <ostream>
#include <string>


Damage& Damage::operator+=(const Damage& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
    shield = rhs.shield;
    special = rhs.special;
    return *this;
}

void Damage::addDamageDealtOnPlayer(LineInfo& li) {
    addDamage(li, dealtOnPlayer);
}

void Damage::addDamageReceivedFromPlayer(LineInfo& li) {
    addDamage(li, receivedFromPlayer);
}

void Damage::setDealtOnPlayerDPM(int damagePerMinute) {
    dealtOnPlayer.dpm = damagePerMinute;
}

void Damage::setReceivedFromPlayerDPM(int damagePerMinute) {
    receivedFromPlayer.dpm = damagePerMinute;
}

void Damage::addDamage(LineInfo& li, Damage::DamageInfo& di) {
    di.total += li.amount;
    di.count++;
    // TODO: Set these bools on construction only and not
    // on each addition.
    shield = li.shield;
    special = li.special;

    if (li.crit) {
        di.critCount++;
        di.critTotal += li.amount;
        if (li.amount > di.critMax) {
            di.critMax = li.amount;
        }
        if (li.amount < di.critMin) {
            di.critMin = li.amount;
        }
    }
    else if (li.miss) {
        di.misses++;
    }
    else if (!li.nanobots && li.deflect) {
        di.regularDeflectTotal += li.amount;
        di.regularDeflectCount++;
        if (li.amount > di.regularDeflectMax) {
            di.regularDeflectMax = li.amount;
        }
        if (li.amount < di.regularDeflectMin) {
            di.regularDeflectMin = li.amount;
        }
    }
    else if (li.nanobots && li.deflect) {
        di.nanobotDeflectTotal += li.amount;
        di.nanobotDeflectCount++;
        if (li.amount > di.nanobotDeflectMax) {
            di.nanobotDeflectMax = li.amount;
        }
        if (li.amount < di.nanobotDeflectMin) {
            di.nanobotDeflectMin = li.amount;
        }
    }
    else if (li.nanobots) {
        di.nanobotCount++;
        di.nanobotTotal += li.amount;
        if (li.amount > di.nanobotMax) {
            di.nanobotMax = li.amount;
        }
        if (li.amount < di.nanobotMin) {
            di.nanobotMin = li.amount;
        }
    }
    else {
        di.regularCount++;
        di.regularTotal += li.amount;
        if (li.amount > di.regularMax) {
            di.regularMax = li.amount;
        }
        if (li.amount < di.regularMin) {
            di.regularMin = li.amount;
        }
    }
}

Damage::DamageInfo& Damage::DamageInfo::operator+=(const DamageInfo& rhs) {
    total += rhs.total;
    count += rhs.count;

    regularTotal += rhs.regularTotal;
    regularCount += rhs.regularCount;
    if (rhs.regularMax > regularMax) {
        regularMax = rhs.regularMax;
    }
    if (rhs.regularMin < regularMin) {
        regularMin = rhs.regularMin;
    }

    nanobotTotal += rhs.nanobotTotal;
    nanobotCount += rhs.nanobotCount;
    if (rhs.nanobotMax > nanobotMax) {
        nanobotMax = rhs.nanobotMax;
    }
    if (rhs.nanobotMin < nanobotMin) {
        nanobotMin = rhs.nanobotMin;
    }

    critTotal += rhs.critTotal;
    critCount += rhs.critCount;
    if (rhs.critMax > critMax) {
        critMax = rhs.critMax;
    }
    if (rhs.critMin < critMin) {
        critMin = rhs.critMin;
    }

    regularDeflectTotal += rhs.regularDeflectTotal;
    regularDeflectCount += rhs.regularDeflectCount;
    if (rhs.regularDeflectMax > regularDeflectMax) {
        regularDeflectMax = rhs.regularDeflectMax;
    }
    if (rhs.regularDeflectMin < regularDeflectMin) {
        regularDeflectMin = rhs.regularDeflectMin;
    }

    nanobotDeflectTotal += rhs.nanobotDeflectTotal;
    nanobotDeflectCount += rhs.nanobotDeflectCount;
    if (rhs.nanobotDeflectMax > nanobotDeflectMax) {
        nanobotDeflectMax = rhs.nanobotDeflectMax;
    }
    if (rhs.nanobotDeflectMin < nanobotDeflectMin) {
        nanobotDeflectMin = rhs.nanobotDeflectMin;
    }

    misses += rhs.misses;

    return *this;
}
