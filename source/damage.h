#ifndef DAMAGE_H
#define DAMAGE_H


#include <limits>
#include <ostream>


class LineInfo;

class Damage {
public:
    Damage& operator+=(const Damage& rhs);

    void addDamageDealtOnPlayer(LineInfo& li);
    void addDamageReceivedFromPlayer(LineInfo& li);

    void setDealtOnPlayerDPM(int damagePerMinute);
    void setReceivedFromPlayerDPM(int damagePerMinute);

    bool hasShieldReceivedFromPlayer() const;
    bool hasSpecialReceivedFromPlayer() const;
    bool hasRegularMissReceivedFromPlayer() const;
    bool hasRegularReceivedFromPlayer() const;
    bool hasNanobotReceivedFromPlayer() const;
    bool hasShieldDealtOnPlayer() const;
    bool hasSpecialDealtOnPlayer() const;
    bool hasRegularMissDealtOnPlayer() const;
    bool hasRegularDealtOnPlayer() const;
    bool hasNanobotDealtOnPlayer() const;

    // TODO: This is shit, just rework everything.
    int getTotalDealtOnPlayer() const {return dealtOnPlayer.total;}
    int getDPMDealtOnPlayer() const {return dealtOnPlayer.dpm;}
    int getCountDealtOnPlayer() const {return dealtOnPlayer.count;}
    int getNormalTotalDealtOnPlayer() const {return dealtOnPlayer.normalTotal;}
    int getNormalCountDealtOnPlayer() const {return dealtOnPlayer.normalCount;}
    int getNormalMaxDealtOnPlayer() const {return dealtOnPlayer.normalMax;}
    int getNormalMinDealtOnPlayer() const {return dealtOnPlayer.normalMin;}
    int getRegularDeflectTotalDealtOnPlayer() const {return dealtOnPlayer.regularDeflectTotal;}
    int getRegularDeflectCountDealtOnPlayer() const {return dealtOnPlayer.regularDeflectCount;}
    int getRegularDeflectMaxDealtOnPlayer() const {return dealtOnPlayer.regularDeflectMax;}
    int getRegularDeflectMinDealtOnPlayer() const {return dealtOnPlayer.regularDeflectMin;}
    int getSpecialTotalDealtOnPlayer() const {return dealtOnPlayer.specialTotal;}
    int getSpecialCountDealtOnPlayer() const {return dealtOnPlayer.specialCount;}
    int getSpecialMaxDealtOnPlayer() const {return dealtOnPlayer.specialMax;}
    int getSpecialMinDealtOnPlayer() const {return dealtOnPlayer.specialMin;}
    int getSpecialDeflectTotalDealtOnPlayer() const {return dealtOnPlayer.specialDeflectTotal;}
    int getSpecialDeflectCountDealtOnPlayer() const {return dealtOnPlayer.specialDeflectCount;}
    int getSpecialDeflectMaxDealtOnPlayer() const {return dealtOnPlayer.specialDeflectMax;}
    int getSpecialDeflectMinDealtOnPlayer() const {return dealtOnPlayer.specialDeflectMin;}
    int getNanobotTotalDealtOnPlayer() const {return dealtOnPlayer.nanobotTotal;}
    int getNanobotCountDealtOnPlayer() const {return dealtOnPlayer.nanobotCount;}
    int getNanobotMaxDealtOnPlayer() const {return dealtOnPlayer.nanobotMax;}
    int getNanobotMinDealtOnPlayer() const {return dealtOnPlayer.nanobotMin;}
    int getCritTotalDealtOnPlayer() const {return dealtOnPlayer.critTotal;}
    int getCritCountDealtOnPlayer() const {return dealtOnPlayer.critCount;}
    int getCritMaxDealtOnPlayer() const {return dealtOnPlayer.critMax;}
    int getCritMinDealtOnPlayer() const {return dealtOnPlayer.critMin;}
    int getShieldTotalDealtOnPlayer() const {return dealtOnPlayer.shieldTotal;}
    int getShieldCountDealtOnPlayer() const {return dealtOnPlayer.shieldCount;}
    int getShieldMaxDealtOnPlayer() const {return dealtOnPlayer.shieldMax;}
    int getShieldMinDealtOnPlayer() const {return dealtOnPlayer.shieldMin;}
    int getRegularMissesDealtOnPlayer() const {return dealtOnPlayer.regularMisses;}
    int getSpecialMissesDealtOnPlayer() const {return dealtOnPlayer.specialMisses;}

    int getTotalReceivedFromPlayer() const {return receivedFromPlayer.total;}
    int getDPMReceivedFromPlayer() const {return receivedFromPlayer.dpm;}
    int getCountReceivedFromPlayer() const {return receivedFromPlayer.count;}
    int getNormalTotalReceivedFromPlayer() const {return receivedFromPlayer.normalTotal;}
    int getNormalCountReceivedFromPlayer() const {return receivedFromPlayer.normalCount;}
    int getNormalMaxReceivedFromPlayer() const {return receivedFromPlayer.normalMax;}
    int getNormalMinReceivedFromPlayer() const {return receivedFromPlayer.normalMin;}
    int getRegularDeflectTotalReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectTotal;}
    int getRegularDeflectCountReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectCount;}
    int getRegularDeflectMaxReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectMax;}
    int getRegularDeflectMinReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectMin;}
    int getSpecialTotalReceivedFromPlayer() const {return receivedFromPlayer.specialTotal;}
    int getSpecialCountReceivedFromPlayer() const {return receivedFromPlayer.specialCount;}
    int getSpecialMaxReceivedFromPlayer() const {return receivedFromPlayer.specialMax;}
    int getSpecialMinReceivedFromPlayer() const {return receivedFromPlayer.specialMin;}
    int getSpecialDeflectTotalReceivedFromPlayer() const {return receivedFromPlayer.specialDeflectTotal;}
    int getSpecialDeflectCountReceivedFromPlayer() const {return receivedFromPlayer.specialDeflectCount;}
    int getSpecialDeflectMaxReceivedFromPlayer() const {return receivedFromPlayer.specialDeflectMax;}
    int getSpecialDeflectMinReceivedFromPlayer() const {return receivedFromPlayer.specialDeflectMin;}
    int getNanobotTotalReceivedFromPlayer() const {return receivedFromPlayer.nanobotTotal;}
    int getNanobotCountReceivedFromPlayer() const {return receivedFromPlayer.nanobotCount;}
    int getNanobotMaxReceivedFromPlayer() const {return receivedFromPlayer.nanobotMax;}
    int getNanobotMinReceivedFromPlayer() const {return receivedFromPlayer.nanobotMin;}
    int getCritTotalReceivedFromPlayer() const {return receivedFromPlayer.critTotal;}
    int getCritCountReceivedFromPlayer() const {return receivedFromPlayer.critCount;}
    int getCritMaxReceivedFromPlayer() const {return receivedFromPlayer.critMax;}
    int getCritMinReceivedFromPlayer() const {return receivedFromPlayer.critMin;}
    int getShieldTotalReceivedFromPlayer() const {return receivedFromPlayer.shieldTotal;}
    int getShieldCountReceivedFromPlayer() const {return receivedFromPlayer.shieldCount;}
    int getShieldMaxReceivedFromPlayer() const {return receivedFromPlayer.shieldMax;}
    int getShieldMinReceivedFromPlayer() const {return receivedFromPlayer.shieldMin;}
    int getRegularMissesReceivedFromPlayer() const {return receivedFromPlayer.regularMisses;}
    int getSpecialMissesReceivedFromPlayer() const {return receivedFromPlayer.specialMisses;}

private:
	struct DamageInfo {
        DamageInfo& operator+=(const DamageInfo& rhs);

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

    DamageInfo dealtOnPlayer;
    DamageInfo receivedFromPlayer;

    void addDamage(LineInfo& li, DamageInfo& di);
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
