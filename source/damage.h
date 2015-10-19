#ifndef DAMAGE_H
#define DAMAGE_H


#include <limits>
#include <string>
#include <utility>
#include <vector>


class LineInfo;

class Damage {
private:
	struct DamageInfo {
        DamageInfo& operator+=(const DamageInfo& rhs);

        int count = 0;
        int total = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

public:
    Damage& operator+=(const Damage& rhs);

    void addDamageDealtOnPlayer(LineInfo& li);
    void addDamageReceivedFromPlayer(LineInfo& li);

    std::vector<DamageInfo> getTotalDamage() const;  // Or just an int for the total.

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalReceivedFromPlayerPerCategory() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalDealtOnPlayerPerCategory() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalReceivedFromPlayerPerDamageType() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalDealtOnPlayerPerDamageType() const;


    // Flips position of damageType and category.
    std::vector<std::pair<std::string, std::vector<std::string, DamageInfo>>>
    getReceivedFromPlayerPerType(const std::string damageType) const;
    // Flips position of damageType and category.
    std::vector<std::pair<std::string, std::vector<std::string, DamageInfo>>>
    getDealtOnPlayerPerType(const std::string damageType) const;

    // Returns receivedFromPlayer as is
    std::vector<std::pair<std::string, std::vector<std::string, DamageInfo>>>
    getDamageReceivedFromPlayer() const;
    // Returns dealtOnPlayer as is
    std::vector<std::pair<std::string, std::vector<std::string, DamageInfo>>>
    getDamageDealtOnPlayer() const;

    void setDealtOnPlayerDPM(int damagePerMinute);
    void setReceivedFromPlayerDPM(int damagePerMinute);

private:
    //                   <   category,                      < damageType, stats     >>
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, DamageInfo>>>> receivedFromPlayer;
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, DamageInfo>>>> dealtOnPlayer;

    void addDamage(LineInfo& li, DamageInfo& di);
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
