#ifndef STAT_WRITER_H
#define STAT_WRITER_H


#include "player_vector.h"

#include <ostream>
#include <string>


class Player;

class StatWriter {
public:
    StatWriter(PlayerVector<Player*>& playerVector);

    void createDDTopList();
    void createDDDetailedOverview();
    void createDDPerOpponent(std::string playerName);
    void createDDOnSpecificOpponent(std::string PlayerName,
                                    std::string opponentName);
    void createDDPerDamageType(std::string playerName);

    void createDRTopList();
    void createDRDetailedOverview();
    void createDRPerOpponent(std::string playerName);
    void createDRFromSpecificOpponent(std::string PlayerName,
                                      std::string opponentName);
    void createDRPerDamageType(std::string playerName);

    void createHealReceivedOverview();
    void createHealDealtPerReceiver();  // Can only be done for "You".

    void createNanoReceivedOverview();
    void createNanoDealtPerReceiver();  // Can only be done for "You".

    void createNanoProgramsCasted();  // Can only be done for "You".
    void createNanoProgramsReceived();  // Can only be done for "You".
    void createNanoProgramsReceivedPerOpponent();  // Can only be done for "You".

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
