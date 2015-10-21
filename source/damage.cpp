#include "damage.h"
#include "line_info.h"

#include <iomanip>
#include <limits>
#include <ostream>
#include <string>


Damage& Damage::operator+=(const Damage& rhs) {
    dealtOnPlayer += rhs.dealtOnPlayer;
    receivedFromPlayer += rhs.receivedFromPlayer;
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


bool Damage::hasShieldReceivedFromPlayer() const {
    return receivedFromPlayer.shieldCount > 0;
}

bool Damage::hasShieldDealtOnPlayer() const {
    return dealtOnPlayer.shieldCount > 0;
}

bool Damage::hasSpecialReceivedFromPlayer() const {
    return receivedFromPlayer.specialCount > 0 ||
           receivedFromPlayer.specialDeflectCount > 0 ||
           receivedFromPlayer.specialMisses > 0;
}

bool Damage::hasSpecialDealtOnPlayer() const {
    return dealtOnPlayer.specialCount > 0 ||
           dealtOnPlayer.specialDeflectCount > 0 ||
           dealtOnPlayer.specialMisses > 0;
}

bool Damage::hasRegularMissReceivedFromPlayer() const {
    return receivedFromPlayer.regularMisses > 0;
}

bool Damage::hasRegularMissDealtOnPlayer() const {
    return dealtOnPlayer.regularMisses > 0;
}

bool Damage::hasRegularReceivedFromPlayer() const {
    return receivedFromPlayer.regularCount > 0 ||
           receivedFromPlayer.regularDeflectCount > 0 ||
           receivedFromPlayer.critCount > 0;
}

bool Damage::hasRegularDealtOnPlayer() const {
    return dealtOnPlayer.regularCount > 0 ||
           dealtOnPlayer.regularDeflectCount > 0 ||
           dealtOnPlayer.critCount > 0;
}

bool Damage::hasNanobotReceivedFromPlayer() const {
    return receivedFromPlayer.nanobotCount > 0;
}

bool Damage::hasNanobotDealtOnPlayer() const {
    return dealtOnPlayer.nanobotCount > 0;
}

void Damage::addDamage(LineInfo& li, Damage::DamageInfo& di) {
    di.total += li.amount;
    di.count++;

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
    else if (li.miss && li.special) {
        di.specialMisses++;
    }
    else if (li.miss) {
        di.regularMisses++;
    }
    else if (li.deflect && !li.special) {
        di.regularDeflectTotal += li.amount;
        di.regularDeflectCount++;
        if (li.amount > di.regularDeflectMax) {
            di.regularDeflectMax = li.amount;
        }
        if (li.amount < di.regularDeflectMin) {
            di.regularDeflectMin = li.amount;
        }
    }
    else if (li.deflect && li.special) {
        di.specialDeflectTotal += li.amount;
        di.specialDeflectCount++;
        if (li.amount > di.specialDeflectMax) {
            di.specialDeflectMax = li.amount;
        }
        if (li.amount < di.specialDeflectMin) {
            di.specialDeflectMin = li.amount;
        }
    }
    else if (li.special) {
        di.specialCount++;
        di.specialTotal += li.amount;
        if (li.amount > di.specialMax) {
            di.specialMax = li.amount;
        }
        if (li.amount < di.specialMin) {
            di.specialMin = li.amount;
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
    else if (li.shield) {
        di.shieldCount++;
        di.shieldTotal += li.amount;
        if (li.amount > di.shieldMax) {
            di.shieldMax = li.amount;
        }
        if (li.amount < di.shieldMin) {
            di.shieldMin = li.amount;
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

    specialTotal += rhs.specialTotal;
    specialCount += rhs.specialCount;
    if (rhs.specialMax > specialMax) {
        specialMax = rhs.specialMax;
    }
    if (rhs.specialMin < specialMin) {
        specialMin = rhs.specialMin;
    }

    specialDeflectTotal += rhs.specialDeflectTotal;
    specialDeflectCount += rhs.specialDeflectCount;
    if (rhs.specialDeflectMax > specialDeflectMax) {
        specialDeflectMax = rhs.specialDeflectMax;
    }
    if (rhs.specialDeflectMin < specialDeflectMin) {
        specialDeflectMin = rhs.specialDeflectMin;
    }

    shieldTotal += rhs.shieldTotal;
    shieldCount += rhs.shieldCount;
    if (rhs.shieldMax > shieldMax) {
        shieldMax = rhs.shieldMax;
    }
    if (rhs.shieldMin < shieldMin) {
        shieldMin = rhs.shieldMin;
    }

    regularMisses += rhs.regularMisses;
    specialMisses += rhs.specialMisses;

    return *this;
}
