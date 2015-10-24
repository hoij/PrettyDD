#ifndef DAMAGE_WRITER_H
#define DAMAGE_WRITER_H


#include "player_vector.h"
#include "writer_helper.h"

#include <string>
#include <utility>
#include <vector>


class Damage;

class DamageWriter : public WriterHelper {

public:
    DamageWriter(PlayerVector<Player*>& playerVector,
                 Configuration& config,
                 std::ofstream& file);

    void createDDTopList();
    void createDRTopList();

    void createDDDetailedTopList();
    void createDRDetailedTopList();

    void createDDPerType(std::string playerName);
    void createDRPerType(std::string playerName);

    void createDDPerTypeDetailed(std::string playerName);
    void createDRPerTypeDetailed(std::string playerName);

    void createDDPerOpponent(std::string playerName);
    void createDRPerOpponent(std::string playerName);

    void createDamagePerType(std::string playerName,
                             std::string opponentName);

    void createDamagePerTypeDetailed(std::string playerName,
                                     std::string opponentName);

private:
    typedef void (DamageWriter::*writeHeadingsPointer)(bool self);
    typedef void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                                     const Damage& d,
                                                     int place,
                                                     bool self);

    // Common writes
    void createTopList(
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer);

    void createDetailedTopList(
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer);

    void createDamagePerType(
        std::string playerName,
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>> damagePerType,
        int typesPerWindow,
        bool detailed);

    void createDamagePerOpponent(
        std::string playerName,
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>> damagePerOpponent,
        bool detailed);

    void createDamagePerType(
        std::string playerName,
        std::string opponentName,
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>> damagePerOpponent,
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

    std::string nameFile(int fileNr);
    void writeExecutionOfNextScript(int fileNr, int nrOfFiles);

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
    void writeTopList(const std::string& name, const Damage& d, int place, bool self);
    void writePerPlayer(const std::string& name, const Damage& d, int place, bool self);
    void writePerType(const std::string& name, const Damage& d, int place, bool self);
    void writePerTypeDetailed(const std::string& name, const Damage& d, int place, bool self);

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
    void setPerPlayerWriteMethods(writeHeadingsPointer& whp,
                                  writeDamagePointer& wdp,
                                  bool detailed);
    void setPerTypeWriteMethods(writeHeadingsPointer& whp,
                                writeDamagePointer& wdp,
                                bool detailed);

    void sortByTotal(std::vector<std::pair<std::string, Damage>>& v);

    bool playerFound(std::string titleBase,
                     std::string playerName,
                     const std::vector<std::pair<std::string, Damage>>& v);

    bool opponentFound(std::string titleBase,
                       std::string playerName,
                       std::string opponentName,
                       const std::vector<std::pair<std::string, Damage>>& v);

    bool isSelf(std::string playerName, std::string opponentName = "");

    // TODO: Move to separate class or into Damage.
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
