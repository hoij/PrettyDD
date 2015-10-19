#ifndef DAMAGE_H
#define DAMAGE_H


#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>


class LineInfo;

class Damage {
private:
	struct DamageInfo {
        DamageInfo& operator+=(const DamageInfo& rhs);
        void add(int amount);

        int count = 0;
        int total = 0;
        int max = -1;
        int min = std::numeric_limits<int>::max();
    };

    //              <   category,         <    subtype, stats     >>
    typedef std::map<std::string, std::map<std::string, DamageInfo>> damageMap;

public:
    Damage& operator+=(const Damage& rhs);

    void addDamageDealtOnPlayer(LineInfo& li);
    void addDamageReceivedFromPlayer(LineInfo& li);

    DamageInfo getTotalReceivedFromPlayer() const;  // Or just an int for the total.
    DamageInfo getTotalDealtOnPlayer() const;  // Or just an int for the total.

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalReceivedFromPlayerPerCategory() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalDealtOnPlayerPerCategory() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalReceivedFromPlayerPerDamageType() const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalDealtOnPlayerPerDamageType() const;


    // Flips position of subtype and category.
    damageMap getReceivedFromPlayerPerSubtype() const;
    damageMap getDealtOnPlayerPerSubtype() const;

    // Returns the damage maps as is.
    damageMap getReceivedFromPlayer() const;
    damageMap getDealtOnPlayer() const;

    void setDealtOnPlayerDPM(int damagePerMinute);
    void setReceivedFromPlayerDPM(int damagePerMinute);

private:
    void addDamage(LineInfo& li, damageMap& di);
    DamageInfo getTotal(const std::map<std::string, std::map<
                            std::string, DamageInfo>>& m) const;

    std::vector<std::pair<std::string, DamageInfo>>
    getTotalPerCategory(const damageMap& m) const;

    std::vector<std::pair<std::string, Damage::DamageInfo>>
    getTotalPerSubtype(const damageMap& m) const;

    damageMap getPerSubtype(const damageMap& m) const;

    damageMap receivedFromPlayer;
    damageMap dealtOnPlayer;

    //                   <   category,                      < subtype, stats     >>
    //std::vector<std::pair<std::string, std::vector<std::pair<std::string, DamageInfo>>>> receivedFromPlayer;
    //std::vector<std::pair<std::string, std::vector<std::pair<std::string, DamageInfo>>>> dealtOnPlayer;
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
