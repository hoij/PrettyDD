#ifndef DAMAGE_H
#define DAMAGE_H


#include <limits>
#include <ostream>
#include <string>

class LineInfo;

class Damage {
public:
    Damage& operator+=(const Damage& rhs);
    friend Damage operator+(Damage lhs, const Damage& rhs);

    void add(LineInfo& li, std::string playerType);
    void addDamageDealtOnPlayer(LineInfo& li);
    void addDamageReceivedFromPlayer(LineInfo& li);

    std::ostream& writeDamageDealtToPlayer(std::ostream& os);
    std::ostream& writeDamageReceivedFromPlayer(std::ostream& os);
    static std::ostream& writeHeadings(std::ostream& os);

    int getTotalDealt() const {return dealtOnPlayer.total;}
    int getCountDealt() const {return dealtOnPlayer.count;}
    int getRegularMaxDealt() const {return dealtOnPlayer.regularMax;}
    int getRegularMinDealt() const {return dealtOnPlayer.regularMin;}
    int getCritTotalDealt() const {return dealtOnPlayer.critTotal;}
    int getCritCountDealt() const {return dealtOnPlayer.critCount;}
    int getCritMaxDealt() const {return dealtOnPlayer.critMax;}
    int getCritMinDealt() const {return dealtOnPlayer.critMin;}
    int getDeflectsDealt() const {return dealtOnPlayer.deflects;}
    int getMissesDealt() const {return dealtOnPlayer.misses;}

    int getTotalReceived() const {return receivedFromPlayer.total;}
    int getCountReceived() const {return receivedFromPlayer.count;}
    int getRegularMaxReceived() const {return receivedFromPlayer.regularMax;}
    int getRegularMinReceived() const {return receivedFromPlayer.regularMin;}
    int getCritTotalReceived() const {return receivedFromPlayer.critTotal;}
    int getCritCountReceived() const {return receivedFromPlayer.critCount;}
    int getCritMaxReceived() const {return receivedFromPlayer.critMax;}
    int getCritMinReceived() const {return receivedFromPlayer.critMin;}
    int getDeflectsReceived() const {return receivedFromPlayer.deflects;}
    int getMissesReceived() const {return receivedFromPlayer.misses;}

private:
	struct damageInfo {
        damageInfo& operator+=(const damageInfo& rhs);

        int total = 0;
        int count = 0;
        int regularMax = -1;
        int regularMin = std::numeric_limits<int>::max();
        int critTotal = 0;
		int critCount = 0;
		int critMax = -1;
		int critMin = std::numeric_limits<int>::max();
        int deflects = 0;
        int misses = 0;
        // Not used at the moment:
        int meanTime = 0;
    };

    damageInfo dealtOnPlayer;
    damageInfo receivedFromPlayer;
};


#endif // DAMAGE_H
