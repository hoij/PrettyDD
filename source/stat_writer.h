#ifndef STAT_WRITER_H
#define STAT_WRITER_H


#include "player_vector.h"

#include <ostream>
#include <string>


class Damage;
class Player;

class StatWriter {
// TODO: Split into smaller classes
public:
    StatWriter(PlayerVector<Player*>& playerVector);

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
    // Damage headings
    std::ostream& writeDDTopListHeadings(size_t maxNameLength,
                                         std::ostream& file);
    std::ostream& writeDDDetailedOverviewHeadings(size_t maxNameLength,
                                                  std::ostream& os);
    std::ostream& writeDDOnSpecificOpponentHeadings(size_t maxNameLength,
                                                    std::ostream& os);
    std::ostream& writeDDPerDamageTypeHeadings(size_t maxNameLength,
                                                    std::ostream& os);
    std::ostream& writeDDHeadings(std::ostream& os);

    // Common headings
    std::ostream& writeNameHeading(std::string category,
                                   size_t maxNameLength,
                                   std::ostream& os);

    // Damage writes
    std::ostream& writeDDTopList(const Damage& d, std::ostream& os);
    std::ostream& writeDDDetailedOverview(const Damage& d, std::ostream& os);
    std::ostream& writeDR(const Damage& d, std::ostream& os);
    // Remove when done:
    std::ostream& writeDDOld(const Damage& d, std::ostream& os);

    // Common writes
    void writeContentsToFile(std::string fileNameBase,
                             std::vector<std::pair<std::string, Damage>>& v,
                             int nrOfFiles,
                             int typesPerFile,
                             size_t maxNameLength,
                             std::ostream& (StatWriter::*writeHeadings)
                                (size_t maxNameLength, std::ostream& os),
                             std::ostream& (StatWriter::*writeDD)
                                (const Damage& d, std::ostream& os));
    void writeContents(
        std::vector<std::pair<std::string, Damage>>::iterator start,
        std::vector<std::pair<std::string, Damage>>::iterator stop1,
        std::vector<std::pair<std::string, Damage>>::iterator stop2,
        std::string fileName,
        size_t maxNameLength,
        int& place,
        std::ostream& (StatWriter::*writeHeadings)
            (size_t maxNameLength, std::ostream& os),
        std::ostream& (StatWriter::*writeDD)
            (const Damage& d, std::ostream& os));

    std::ostream& writePlace(int place, std::ostream& os);
    std::ostream& writeName(std::string name,
                            size_t maxNameLength,
                            std::ostream& os);

    // Helper functions
    double percentage(int total, int part);
    int damagePerMinute(int totalDamage, int startTime);
    void createNotFoundMessage(std::string fileName,
                               std::string message);
    void sortByDealt(std::vector<std::pair<std::string, Damage>>& v);
    void sortByReceived(std::vector<std::pair<std::string, Damage>>& v);

    // Comparators
    // Remove when done:
    static bool compareTotalDealtOld(const Player* p1, const Player* p2);
    static bool compareTotalReceived(const Player* p1, const Player* p2);


    PlayerVector<Player*>& playerVector;
};


#endif  // STAT_WRITER_H
