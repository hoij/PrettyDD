#include "damage.h"
#include "line_info.h"

#include <iomanip>
#include <ostream>
#include <string>


Damage& Damage::operator+=(const Damage& rhs) {
    total += rhs.total;
    count += rhs.count;

    normalTotal += rhs.normalTotal;
    normalCount += rhs.normalCount;
    if (rhs.normalMax > normalMax) {
        normalMax = rhs.normalMax;
    }
    if (rhs.normalMin < normalMin) {
        normalMin = rhs.normalMin;
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

void Damage::addDamage(LineInfo& li) {
    total += li.amount;
    count++;

    if (li.crit) {
        critCount++;
        critTotal += li.amount;
        if (li.amount > critMax) {
            critMax = li.amount;
        }
        if (li.amount < critMin) {
            critMin = li.amount;
        }
    }
    else if (li.miss && li.special) {
        specialMisses++;
    }
    else if (li.miss) {
        regularMisses++;
    }
    else if (li.deflect && !li.special) {
        regularDeflectTotal += li.amount;
        regularDeflectCount++;
        if (li.amount > regularDeflectMax) {
            regularDeflectMax = li.amount;
        }
        if (li.amount < regularDeflectMin) {
            regularDeflectMin = li.amount;
        }
    }
    else if (li.deflect && li.special) {
        specialDeflectTotal += li.amount;
        specialDeflectCount++;
        if (li.amount > specialDeflectMax) {
            specialDeflectMax = li.amount;
        }
        if (li.amount < specialDeflectMin) {
            specialDeflectMin = li.amount;
        }
    }
    else if (li.special) {
        specialCount++;
        specialTotal += li.amount;
        if (li.amount > specialMax) {
            specialMax = li.amount;
        }
        if (li.amount < specialMin) {
            specialMin = li.amount;
        }
    }
    else if (li.nanobots) {
        nanobotCount++;
        nanobotTotal += li.amount;
        if (li.amount > nanobotMax) {
            nanobotMax = li.amount;
        }
        if (li.amount < nanobotMin) {
            nanobotMin = li.amount;
        }
    }
    else if (li.shield) {
        shieldCount++;
        shieldTotal += li.amount;
        if (li.amount > shieldMax) {
            shieldMax = li.amount;
        }
        if (li.amount < shieldMin) {
            shieldMin = li.amount;
        }
    }
    else {
        normalCount++;
        normalTotal += li.amount;
        if (li.amount > normalMax) {
            normalMax = li.amount;
        }
        if (li.amount < normalMin) {
            normalMin = li.amount;
        }
    }
}

void Damage::setDPM(int damagePerMinute) {
    dpm = damagePerMinute;
}

bool Damage::hasShield() const {
    return shieldCount > 0;
}

bool Damage::hasSpecial() const {
    return specialCount > 0 ||
           specialDeflectCount > 0 ||
           specialMisses > 0;
}

bool Damage::hasRegularMiss() const {
    return regularMisses > 0;
}

bool Damage::hasRegular() const {
    return normalCount > 0 ||
           regularDeflectCount > 0 ||
           critCount > 0;
}

bool Damage::hasNanobot() const {
    return nanobotCount > 0;
}
