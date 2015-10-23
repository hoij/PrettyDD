#ifndef DAMAGE_H
#define DAMAGE_H


#include <limits>
#include <ostream>


class LineInfo;

class Damage {
public:
    Damage& operator+=(const Damage& rhs);

    void addDamage(LineInfo& li);

    void setDPM(int damagePerMinute);

    bool hasShield() const;
    bool hasSpecial() const;
    bool hasRegularMiss() const;
    bool hasRegular() const;
    bool hasNanobot() const;

    int getTotal() const {return total;}
    int getDPM() const {return dpm;}
    int getCount() const {return count;}
    int getNormalTotal() const {return normalTotal;}
    int getNormalCount() const {return normalCount;}
    int getNormalMax() const {return normalMax;}
    int getNormalMin() const {return normalMin;}
    int getRegularDeflectTotal() const {return regularDeflectTotal;}
    int getRegularDeflectCount() const {return regularDeflectCount;}
    int getRegularDeflectMax() const {return regularDeflectMax;}
    int getRegularDeflectMin() const {return regularDeflectMin;}
    int getSpecialTotal() const {return specialTotal;}
    int getSpecialCount() const {return specialCount;}
    int getSpecialMax() const {return specialMax;}
    int getSpecialMin() const {return specialMin;}
    int getSpecialDeflectTotal() const {return specialDeflectTotal;}
    int getSpecialDeflectCount() const {return specialDeflectCount;}
    int getSpecialDeflectMax() const {return specialDeflectMax;}
    int getSpecialDeflectMin() const {return specialDeflectMin;}
    int getNanobotTotal() const {return nanobotTotal;}
    int getNanobotCount() const {return nanobotCount;}
    int getNanobotMax() const {return nanobotMax;}
    int getNanobotMin() const {return nanobotMin;}
    int getCritTotal() const {return critTotal;}
    int getCritCount() const {return critCount;}
    int getCritMax() const {return critMax;}
    int getCritMin() const {return critMin;}
    int getShieldTotal() const {return shieldTotal;}
    int getShieldCount() const {return shieldCount;}
    int getShieldMax() const {return shieldMax;}
    int getShieldMin() const {return shieldMin;}
    int getRegularMisses() const {return regularMisses;}
    int getSpecialMisses() const {return specialMisses;}

private:
    int total = 0;
    int dpm = 0;
    int count = 0;
    int normalTotal = 0;
    int normalCount = 0;
    int normalMax = -1;
    int normalMin = std::numeric_limits<int>::max();
    int regularDeflectTotal = 0;
    int regularDeflectCount = 0;
    int regularDeflectMax = -1;
    int regularDeflectMin = std::numeric_limits<int>::max();
    int specialTotal = 0;
    int specialCount = 0;
    int specialMax = -1;
    int specialMin = std::numeric_limits<int>::max();
    int specialDeflectTotal = 0;
    int specialDeflectCount = 0;
    int specialDeflectMax = -1;
    int specialDeflectMin = std::numeric_limits<int>::max();
    int nanobotTotal = 0;
    int nanobotCount = 0;
    int nanobotMax = -1;
    int nanobotMin = std::numeric_limits<int>::max();
    int critTotal = 0;
    int critCount = 0;
    int critMax = -1;
    int critMin = std::numeric_limits<int>::max();
    int shieldTotal = 0;
    int shieldCount = 0;
    int shieldMax = -1;
    int shieldMin = std::numeric_limits<int>::max();
    int regularMisses = 0;
    int specialMisses = 0;
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
