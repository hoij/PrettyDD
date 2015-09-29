#ifndef STAT_WRITER_H
#define STAT_WRITER_H


#include "player_vector.h"
#include "configuration.h"

#include <ostream>
#include <string>


class Damage;
class Player;

class StatWriter {
// TODO: Split into smaller classes
public:
    StatWriter(PlayerVector<Player*>& playerVector, Configuration& config);
    StatWriter& operator=(StatWriter rhs) = delete;

    void createDDTopList();
    void createDDDetailedTopList();
    void createDDPerDamageType(std::string playerName);
    void createDDPerOpponent(std::string playerName);  // Lists all opponents for playerName
    void createDDOnSpecificOpponent(std::string playerName,
                                    std::string opponentName);

    void createDRTopList();
    void createDRDetailedTopList();
    void createDRPerDamageType(std::string playerName);
    void createDRPerOpponent(std::string playerName);  // Lists all opponents for playerName
    // This is probably covered by using
    // createDDOnSpecificOpponent(playerName, opponentName)
    // in reversed order?:
    void createDRFromSpecificOpponent(std::string playerName,
                                      std::string opponentName);

    void createHealReceivedTopList();
    void createHealReceivedDetailedTopList();
    void createHealDealtPerReceiver();  // Can only be done for "You".

    void createNanoReceivedTopList();
    void createNanoReceivedDetailedTopList();
    void createNanoDealtPerReceiver();  // Can only be done for "You".

    void createNanoProgramsCasted();  // Can only be done for "You".
    void createNanoProgramsCastedPerTarget();  // Can only be done for "You".
    void createNanoProgramsCastedOnTarget(std::string targetName);  // Can only be done for "You".
    void createNanoProgramsReceived();  // Can only be done for "You".
    void createNanoProgramsReceivedPerTarget();  // Can only be done for "You".
    void createNanoProgramsReceivedFromTarget(std::string targetName);  // Can only be done for "You".

    void createXPInfo();

    void createHelp();

    // Remove when done:
    void createDDOverviewUnsorted();
    void createOldDDOverview();

private:
    // Common writes
    typedef std::ostream& (StatWriter::*writeHeadingsPointer)(
        std::ostream& os);
    typedef std::ostream& (StatWriter::*writeDDPointer)(const Damage& d,
                                                        std::ostream& os);
    void writeDDToFile(
        std::string titleBase,
        std::vector<std::pair<std::string, Damage>>& v,
        unsigned int nrOfWindows,
        int typesPerWindow,
        writeHeadingsPointer,
        writeDDPointer);

    void writeDDStats(
        std::vector<std::pair<std::string, Damage>>::iterator start,
        std::vector<std::pair<std::string, Damage>>::iterator stop,
        std::ostream& file,
        std::string title,
        int& place,
        writeHeadingsPointer,
        writeDDPointer);

    void writeDDStatsReadable(
        std::vector<std::pair<std::string, Damage>>::iterator start,
        std::vector<std::pair<std::string, Damage>>::iterator stop,
        std::ostream& file,
        std::string title,
        int& place,
        writeHeadingsPointer,
        writeDDPointer);

    std::ostream& writePlace(int place, std::ostream& os);
    std::ostream& writeName(std::string name, std::ostream& os);

    // Damage headings
    std::ostream& writeDDTopListHeadings(std::ostream& file);
    std::ostream& writeDDDetailedOverviewHeadingsOthers(std::ostream& os);
    std::ostream& writeDDDetailedOverviewHeadingsSelf(std::ostream& os);
    std::ostream& writeDDDetailedOverviewHeadings(std::ostream& os, bool self);
    std::ostream& writeDDPerDamageTypeHeadings(std::ostream& os);
    std::ostream& writeDDHeadings(std::ostream& os);

    std::ostream& writeXPHeadingsOverall(std::ostream& os);
    std::ostream& writeXPHeadingsDetailed(std::ostream& os);

    // Common headings

    // Damage writes
    std::ostream& writeDDTopList(const Damage& d, std::ostream& os);

    std::ostream&
    writeDDDetailedOverviewOthers(const Damage& d, std::ostream& os);

    std::ostream&
    writeDDDetailedOverviewSelf(const Damage& d, std::ostream& os);

    std::ostream&
    writeDDDetailedOverview(const Damage& d, std::ostream& os, bool self);

    std::ostream& writeDR(const Damage& d, std::ostream& os);
    std::ostream& writeDRSelf(const Damage& d, std::ostream& os);
    // Remove when done:
    std::ostream& writeDDOld(const Damage& d, std::ostream& os);

    std::ostream& writeXPStatsOverview(const XP& xp,
                                       std::string type,
                                       std::ostream& os);

    std::ostream& writeXPStatsDetailed(const XP& xp,
                                       std::string type,
                                       std::ostream& os);

    // Helper functions
    double percentage(int total, int part);
    void createNotFoundMessage(std::string title,
                               std::string message);
    void sortByDealt(std::vector<std::pair<std::string, Damage>>& v);
    void sortByReceived(std::vector<std::pair<std::string, Damage>>& v);
    std::string dblToString(const double d);

    // Comparators
    // Remove when done:
    static bool compareTotalDealtOld(const Player* p1, const Player* p2);
    static bool compareTotalReceived(const Player* p1, const Player* p2);


    // Colors
    const std::string lime = "#00FF00";
    const std::string lightBlue = "#3399FF";
    const std::string yellow = "#FFFF00";

    const char fillChar = '_';

    PlayerVector<Player*>& playerVector;
    Configuration& config;
};


#endif  // STAT_WRITER_H
