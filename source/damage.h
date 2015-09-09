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

    // TODO: Just make the data public?
    int getTotalDealtOnPlayer() const {return dealtOnPlayer.total;}
    int getCountDealtOnPlayer() const {return dealtOnPlayer.count;}
    int getRegularTotalDealtOnPlayer() const {return dealtOnPlayer.regularTotal;}
    int getRegularCountDealtOnPlayer() const {return dealtOnPlayer.regularCount;}
    int getRegularMaxDealtOnPlayer() const {return dealtOnPlayer.regularMax;}
    int getRegularMinDealtOnPlayer() const {return dealtOnPlayer.regularMin;}
    int getNanobotTotalDealtOnPlayer() const {return dealtOnPlayer.nanobotTotal;}
    int getNanobotCountDealtOnPlayer() const {return dealtOnPlayer.nanobotCount;}
    int getNanobotMaxDealtOnPlayer() const {return dealtOnPlayer.nanobotMax;}
    int getNanobotMinDealtOnPlayer() const {return dealtOnPlayer.nanobotMin;}
    int getCritTotalDealtOnPlayer() const {return dealtOnPlayer.critTotal;}
    int getCritCountDealtOnPlayer() const {return dealtOnPlayer.critCount;}
    int getCritMaxDealtOnPlayer() const {return dealtOnPlayer.critMax;}
    int getCritMinDealtOnPlayer() const {return dealtOnPlayer.critMin;}
    int getDeflectsDealtOnPlayer() const {return dealtOnPlayer.deflects;}
    int getMissesDealtOnPlayer() const {return dealtOnPlayer.misses;}

    int getTotalReceivedFromPlayer() const {return receivedFromPlayer.total;}
    int getCountReceivedFromPlayer() const {return receivedFromPlayer.count;}
    int getRegularTotalReceivedFromPlayer() const {return receivedFromPlayer.regularTotal;}
    int getRegularCountReceivedFromPlayer() const {return receivedFromPlayer.regularCount;}
    int getRegularMaxReceivedFromPlayer() const {return receivedFromPlayer.regularMax;}
    int getRegularMinReceivedFromPlayer() const {return receivedFromPlayer.regularMin;}
    int getNanobotTotalReceivedFromPlayer() const {return receivedFromPlayer.nanobotTotal;}
    int getNanobotCountReceivedFromPlayer() const {return receivedFromPlayer.nanobotCount;}
    int getNanobotMaxReceivedFromPlayer() const {return receivedFromPlayer.nanobotMax;}
    int getNanobotMinReceivedFromPlayer() const {return receivedFromPlayer.nanobotMin;}
    int getCritTotalReceivedFromPlayer() const {return receivedFromPlayer.critTotal;}
    int getCritCountReceivedFromPlayer() const {return receivedFromPlayer.critCount;}
    int getCritMaxReceivedFromPlayer() const {return receivedFromPlayer.critMax;}
    int getCritMinReceivedFromPlayer() const {return receivedFromPlayer.critMin;}
    int getDeflectsReceivedFromPlayer() const {return receivedFromPlayer.deflects;}
    int getMissesReceivedFromPlayer() const {return receivedFromPlayer.misses;}

private:
	struct DamageInfo {
        DamageInfo& operator+=(const DamageInfo& rhs);

        int total = 0;
        int count = 0;
        int regularTotal = 0;
        int regularCount = 0;
        int regularMax = -1;
        int regularMin = std::numeric_limits<int>::max();
        int nanobotTotal = 0;
        int nanobotCount = 0;
        int nanobotMax = -1;
        int nanobotMin = std::numeric_limits<int>::max();
        int critTotal = 0;
		int critCount = 0;
		int critMax = -1;
		int critMin = std::numeric_limits<int>::max();
        int deflects = 0;
        int misses = 0;
    };

    DamageInfo dealtOnPlayer;
    DamageInfo receivedFromPlayer;

    void addDamage(LineInfo& li, DamageInfo& di);
};


inline Damage operator+(Damage lhs, const Damage& rhs) {
    return lhs += rhs;
}


#endif // DAMAGE_H
