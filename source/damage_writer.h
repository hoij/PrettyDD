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
    void createDDPerTypeOnSpecificOpponent(std::string playerName,
                                           std::string opponentName);
    void createDDPerTypeDetailedOnSpecificOpponent(std::string playerName,
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
    void createDamagePerTypeOnSpecificOpponent(std::string playerName,
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
    void writePerPlayerHeadings(bool self);
    void writePerTypeHeadings(bool self);
    void writePerTypeDetailedHeadings(bool self);

    // Writes
    void writeDDTopList(const std::string& name, const Damage& d, int place, bool self);
    void writeDDPerPlayer(const std::string& name, const Damage& d, int place, bool self);
    void writeDDPerType(const std::string& name, const Damage& d, int place, bool self);
    void writeDDPerTypeDetailed(const std::string& name, const Damage& d, int place, bool self);

    void writeDRTopList(const std::string& name, const Damage& d, int place, bool self);
    void writeDRPerType(const std::string& name, const Damage& d, int place, bool self);
    void writeDRPerPlayer(const std::string& name, const Damage& d, int place, bool self);

    // WriteDDPerTypeDetailed helper methods
    void writeTotalInfo(int total, int count, std::string category);
    void writeDetailedRegularInfo(const Damage& d);
    void writeDetailedNanobotInfo(const Damage& d);
    void writeDetailedSpecialInfo(const Damage& d, bool self);
    void writeDetailedShieldInfo(const Damage& d);
    void writeDetailedMissInfo(const Damage& d, bool self);
    void writeDetailedInfoForType(std::string type,
                                  int total,
                                  int nrOfhits,
                                  std::string dmgPercent,
                                  std::string maxHit,
                                  std::string minHit,
                                  std::string hitPercentOfType = "",
                                  std::string hitPercentOfCategory = "");

    // Helper methods
    void setDDWriteMethods(writeHeadingsPointer& whp,
                           writeDamagePointer& wdp,
                           bool detailed);
    void setDDPerTypeWriteMethods(writeHeadingsPointer& whp,
                                  writeDamagePointer& wdp,
                                  bool detailed);

    void sortByTotal(std::vector<std::pair<std::string, Damage>>& v);
    void sortByReceived(std::vector<std::pair<std::string, Damage>>& v);

    std::string calcCritHitPercentage(const Damage&d);
    std::string calcMissPercentage(const Damage&d);
    std::string calcRegularMissPercentage(const Damage&d);
    std::string calcSpecialMissPercentage(const Damage&d);
    std::string calcDeflectHitPercentage(const Damage&d);
    std::string calcSpecialDeflectHitPercentage(const Damage&d);
    int calcRegularAndSpecialHits(const Damage&d);
    std::string calcRegularDmgPercentage(const Damage&d);
    std::string calcSpecialDmgPercentage(const Damage&d);
    std::string calcNanobotDmgPercentage(const Damage&d);
    std::string calcShieldDmgPercentage(const Damage&d);


    PlayerVector<Player*>& playerVector;
};


#endif  //DAMAGE_DEALT_WRITER_H
