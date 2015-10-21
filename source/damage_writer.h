#ifndef DAMAGE_WRITER_H
#define DAMAGE_WRITER_H


#include "player_vector.h"
#include "writer_helper.h"

#include <string>
#include <utility>
#include <vector>


class Damage;
class Player;

class DamageWriter : public WriterHelper {

public:
    DamageWriter(PlayerVector<Player*>& playerVector,
                 Configuration& config,
                 std::ofstream& file);

    void createDDTopList();
    void createDDDetailedTopList();
    void createDDPerDamageType(std::string playerName);
    void createDDPerDamageTypeDetailed(std::string playerName);
    void createDDPerOpponent(std::string playerName);  // Lists all opponents for playerName
    void createDDOnSpecificOpponent(std::string playerName,
                                    std::string opponentName);
    void createDDOnSpecificOpponentDetailed(std::string playerName,
                                            std::string opponentName);

    void createDRTopList();
    void createDRDetailedTopList();
    void createDRPerDamageType(std::string playerName);
    void createDRPerOpponent(std::string playerName);  // Lists all opponents for playerName

private:
    typedef void (DamageWriter::*writeHeadingsPointer)(bool self);
    typedef void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                                     const Damage& d,
                                                     int place,
                                                     bool self);

    // Common writes
    void createDamagePerDamageType(std::string playerName,
                                   std::string titleBase,
                                   int typesPerWindow,
                                   bool dealt,
                                   bool detailed);
    void createDamagePerOpponent(std::string playerName,
                                 std::string titleBase,
                                 bool dealt,
                                 bool detailed);
    void createDamageOnSpecificOpponent(std::string playerName,
                                        std::string opponentName,
                                        std::string titleBase,
                                        int typesPerWindow,
                                        bool detailed);
    void writeToFile(
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>>& v,
        unsigned int nrOfWindows,
        int typesPerWindow,
        bool self,
        writeHeadingsPointer,
        writeDamagePointer);

    void writeStats(
        std::vector<std::pair<std::string, Damage>>::iterator start,
        std::vector<std::pair<std::string, Damage>>::iterator stop,
        std::string title,
        int& place,
        bool self,
        writeHeadingsPointer,
        writeDamagePointer);

    // Headings
    void writeTopListHeadings(bool self);
    void writeOverviewHeadingsOthers();
    void writeOverviewHeadingsSelf();
    void writeOverviewHeadings(bool self);
    void writeOverviewHeadingsDetailed(bool self);
    void writePerTypeOverviewHeadingsOthers();
    void writePerTypeOverviewHeadingsSelf();
    void writePerTypeOverviewHeadings(bool self);

    // Writes
    void writeDDTopList(const std::string& name, const Damage& d, int place, bool self);
    void writeDDOverviewDetailed(const std::string& name, const Damage& d, int place, bool self);
    void writeDDOverview(const std::string& name, const Damage& d, int place, bool self);

    void writeDDPerTypeOverview(const std::string& name, const Damage& d, int place, bool self);

    void writeDRPerTypeOverview(const std::string& name, const Damage& d, int place, bool self);


    void writeDRTopList(const std::string& name, const Damage& d, int place, bool self);
    void writeDROverview(const std::string& name, const Damage& d, int place, bool self);

    // Detailed type write helper functions
    void writeDetailedRegularInfo(const Damage& d);
    void writeDetailedNanobotInfo(const Damage& d);
    void writeDetailedSpecialInfo(const Damage& d, bool self);
    void writeDetailedShieldInfo(const Damage& d);
    void writeDetailedMissInfo(const Damage& d, bool self);
    void writeTotalInfo(int total, int count);
    void writeDetailedInfoForType(std::string type,
                                  int total,
                                  int nrOfhits,
                                  std::string dmgPercent,
                                  std::string maxHit,
                                  std::string minHit,
                                  std::string hitPercent);

    // Helper functions
    void setDDWriteMethods(writeHeadingsPointer& whp,
                           writeDamagePointer& wdp,
                           bool detailed);
    void setDDPerTypeWriteMethods(writeHeadingsPointer& whp,
                                  writeDamagePointer& wdp,
                                  bool detailed);
    void setDRWriteMethods(writeHeadingsPointer& whp,
                           writeDamagePointer& wdp);
    void setDRPerTypeWriteMethods(writeHeadingsPointer& whp,
                                  writeDamagePointer& wdp);

    void sortByDealt(std::vector<std::pair<std::string, Damage>>& v);
    void sortByReceived(std::vector<std::pair<std::string, Damage>>& v);

    std::string determineMin(int minDmg);
    std::string determineMax(int maxDmg);

    std::string calcCritHitPercentageReceivedFromPlayer(const Damage&d);
    std::string calcMissPercentageReceivedFromPlayer(const Damage&d);
    std::string calcRegularMissPercentageReceivedFromPlayer(const Damage&d);
    std::string calcSpecialMissPercentageReceivedFromPlayer(const Damage&d);
    std::string calcDeflectHitPercentageReceivedFromPlayer(const Damage&d);
    std::string calcSpecialDeflectHitPercentageReceivedFromPlayer(const Damage&d);
    int calcRegularAndSpecialHitsReceivedFromPlayer(const Damage&d);
    std::string calcRegularDmgPercentageReceivedFromPlayer(const Damage&d);
    std::string calcSpecialDmgPercentageReceivedFromPlayer(const Damage&d);
    std::string calcNanobotDmgPercentageReceivedFromPlayer(const Damage&d);

    std::string calcCritHitPercentageDealtOnPlayer(const Damage&d);
    std::string calcMissPercentageDealtOnPlayer(const Damage&d);
    std::string calcDeflectHitPercentageDealtOnPlayer(const Damage&d);
    int calcRegularAndSpecialHitsDealtOnPlayer(const Damage&d);
    std::string calcRegularDmgPercentageDealtOnPlayer(const Damage&d);
    std::string calcSpecialDmgPercentageDealtOnPlayer(const Damage&d);
    std::string calcNanobotDmgPercentageDealtOnPlayer(const Damage&d);


    PlayerVector<Player*>& playerVector;
};


#endif  //DAMAGE_DEALT_WRITER_H
