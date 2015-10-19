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
    typedef void (DamageWriter::*writeHeadingsPointer)();
    typedef void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                                     const Damage& d,
                                                     int place);

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
        writeHeadingsPointer,
        writeDamagePointer);

    void writeStats(
        std::vector<std::pair<std::string, Damage>>::iterator start,
        std::vector<std::pair<std::string, Damage>>::iterator stop,
        std::string title,
        int& place,
        writeHeadingsPointer,
        writeDamagePointer);

    // Headings
    void writeTopListHeadings();
    void writeOverviewHeadingsOthers();
    void writeOverviewHeadingsSelf();
    void writeOverviewHeadings(bool self);
    void writeOverviewHeadingsDetailed();
    void writePerDamageTypeHeadings();
    void writeDDHeadings();  // TODO: Remove if not to be used.

    // Writes
    void writeDDTopList(const std::string& name, const Damage& d, int place);
    void writeDDOverviewDetailedOthers(const std::string& name, const Damage& d, int place);
    void writeDDOverviewDetailedSelf(const std::string& name, const Damage& d, int place);
    void writeDDOverviewDetailed(const std::string& name, const Damage& d, int place, bool self);
    void writeDDOverviewOthers(const std::string& name, const Damage& d, int place);
    void writeDDOverviewSelf(const std::string& name, const Damage& d, int place);
    void writeDDOverview(const std::string& name, const Damage& d, int place, bool self);

    void writeDRTopList(const std::string& name, const Damage& d, int place);
    void writeDROverviewOthers(const std::string& name, const Damage& d, int place);
    void writeDROverviewSelf(const std::string& name, const Damage& d, int place);
    void writeDROverview(const std::string& name, const Damage& d, int place, bool self);

    // Detailed type write helper functions
    void writeDetailedRegularInfo(const Damage& d, bool self);
    void writeDetailedNanobotInfo(const Damage& d);
    void writeTotalInfo(int total, int count);
    void writeDetailedInfoForType(std::string type,
                                  std::string hitPercent,
                                  std::string nrOfhits,
                                  std::string maxHit,
                                  std::string minHit,
                                  std::string dmgPercent);

    // Helper functions
    void setDDWriteMethods(std::string playerName,
                           std::string opponentName,
                           writeHeadingsPointer& whp,
                           writeDamagePointer& wdp,
                           bool detailed);
    void setDDWriteMethods(std::string playerName,
                           writeHeadingsPointer& whp,
                           writeDamagePointer& wdp,
                           bool detailed);
    void setDRWriteMethods(std::string playerName,
                           writeHeadingsPointer& whp,
                           writeDamagePointer& wdp);

    std::vector<std::pair<std::string, Damage>> makeSortable(std::map<std::string, Damage>& m);

    void sortByDealt(std::vector<std::pair<std::string, Damage>>& v);
    void sortByReceived(std::vector<std::pair<std::string, Damage>>& v);

    std::string determineMin(int minDmg);
    std::string determineMax(int maxDmg);

    std::string calcCritHitPercentage(Damage&d);


    PlayerVector<Player*>& playerVector;
};


#endif  //DAMAGE_DEALT_WRITER_H
