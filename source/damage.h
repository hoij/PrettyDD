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
    void setShield() {shield = true;}
    void setSpecial() {special = true;}
    bool isShield() const { return shield; }
    bool isSpecial() const {return special;}

    // TODO: This is shit, just make the data public.
    int getTotalDealtOnPlayer() const {return dealtOnPlayer.total;}
    int getDPMDealtOnPlayer() const {return dealtOnPlayer.dpm;}
    int getCountDealtOnPlayer() const {return dealtOnPlayer.count;}
    int getRegularTotalDealtOnPlayer() const {return dealtOnPlayer.regularTotal;}
    int getRegularCountDealtOnPlayer() const {return dealtOnPlayer.regularCount;}
    int getRegularMaxDealtOnPlayer() const {return dealtOnPlayer.regularMax;}
    int getRegularMinDealtOnPlayer() const {return dealtOnPlayer.regularMin;}
    int getRegularDeflectTotalDealtOnPlayer() const {return dealtOnPlayer.regularDeflectTotal;}
    int getRegularDeflectCountDealtOnPlayer() const {return dealtOnPlayer.regularDeflectCount;}
    int getRegularDeflectMaxDealtOnPlayer() const {return dealtOnPlayer.regularDeflectMax;}
    int getRegularDeflectMinDealtOnPlayer() const {return dealtOnPlayer.regularDeflectMin;}
    int getNanobotTotalDealtOnPlayer() const {return dealtOnPlayer.nanobotTotal;}
    int getNanobotCountDealtOnPlayer() const {return dealtOnPlayer.nanobotCount;}
    int getNanobotMaxDealtOnPlayer() const {return dealtOnPlayer.nanobotMax;}
    int getNanobotMinDealtOnPlayer() const {return dealtOnPlayer.nanobotMin;}
    int getNanobotDeflectTotalDealtOnPlayer() const {return dealtOnPlayer.nanobotDeflectTotal;}
    int getNanobotDeflectCountDealtOnPlayer() const {return dealtOnPlayer.nanobotDeflectCount;}
    int getNanobotDeflectMaxDealtOnPlayer() const {return dealtOnPlayer.nanobotDeflectMax;}
    int getNanobotDeflectMinDealtOnPlayer() const {return dealtOnPlayer.nanobotDeflectMin;}
    int getCritTotalDealtOnPlayer() const {return dealtOnPlayer.critTotal;}
    int getCritCountDealtOnPlayer() const {return dealtOnPlayer.critCount;}
    int getCritMaxDealtOnPlayer() const {return dealtOnPlayer.critMax;}
    int getCritMinDealtOnPlayer() const {return dealtOnPlayer.critMin;}
    int getMissesDealtOnPlayer() const {return dealtOnPlayer.misses;}

    int getTotalReceivedFromPlayer() const {return receivedFromPlayer.total;}
    int getDPMReceivedFromPlayer() const {return receivedFromPlayer.dpm;}
    int getCountReceivedFromPlayer() const {return receivedFromPlayer.count;}
    int getRegularTotalReceivedFromPlayer() const {return receivedFromPlayer.regularTotal;}
    int getRegularCountReceivedFromPlayer() const {return receivedFromPlayer.regularCount;}
    int getRegularMaxReceivedFromPlayer() const {return receivedFromPlayer.regularMax;}
    int getRegularMinReceivedFromPlayer() const {return receivedFromPlayer.regularMin;}
    int getRegularDeflectTotalReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectTotal;}
    int getRegularDeflectCountReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectCount;}
    int getRegularDeflectMaxReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectMax;}
    int getRegularDeflectMinReceivedFromPlayer() const {return receivedFromPlayer.regularDeflectMin;}
    int getNanobotTotalReceivedFromPlayer() const {return receivedFromPlayer.nanobotTotal;}
    int getNanobotCountReceivedFromPlayer() const {return receivedFromPlayer.nanobotCount;}
    int getNanobotMaxReceivedFromPlayer() const {return receivedFromPlayer.nanobotMax;}
    int getNanobotMinReceivedFromPlayer() const {return receivedFromPlayer.nanobotMin;}
    int getNanobotDeflectTotalReceivedFromPlayer() const {return receivedFromPlayer.nanobotDeflectTotal;}
    int getNanobotDeflectCountReceivedFromPlayer() const {return receivedFromPlayer.nanobotDeflectCount;}
    int getNanobotDeflectMaxReceivedFromPlayer() const {return receivedFromPlayer.nanobotDeflectMax;}
    int getNanobotDeflectMinReceivedFromPlayer() const {return receivedFromPlayer.nanobotDeflectMin;}
    int getCritTotalReceivedFromPlayer() const {return receivedFromPlayer.critTotal;}
    int getCritCountReceivedFromPlayer() const {return receivedFromPlayer.critCount;}
    int getCritMaxReceivedFromPlayer() const {return receivedFromPlayer.critMax;}
    int getCritMinReceivedFromPlayer() const {return receivedFromPlayer.critMin;}
    int getMissesReceivedFromPlayer() const {return receivedFromPlayer.misses;}

private:
	struct DamageInfo {
        DamageInfo& operator+=(const DamageInfo& rhs);

        int total = 0;
        int dpm = 0;
        int count = 0;
        int regularTotal = 0;
        int regularCount = 0;
        int regularMax = -1;
        int regularMin = std::numeric_limits<int>::max();
        int regularDeflectTotal = 0;
        int regularDeflectCount = 0;
        int regularDeflectMax = -1;
        int regularDeflectMin = std::numeric_limits<int>::max();
        int nanobotTotal = 0;
        int nanobotCount = 0;
        int nanobotMax = -1;
        int nanobotMin = std::numeric_limits<int>::max();
        int nanobotDeflectTotal = 0;
        int nanobotDeflectCount = 0;
        int nanobotDeflectMax = -1;
        int nanobotDeflectMin = std::numeric_limits<int>::max();
        int critTotal = 0;
		int critCount = 0;
		int critMax = -1;
		int critMin = std::numeric_limits<int>::max();
        int misses = 0;
    };

    bool shield = false;
    bool special = false;
    DamageInfo dealtOnPlayer;
    DamageInfo receivedFromPlayer;

    void addDamage(LineInfo& li, DamageInfo& di);
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
