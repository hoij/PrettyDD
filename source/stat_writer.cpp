#include "affected_player.h"
#include "damage.h"
#include "logger.h"
#include "player.h"
#include "stat_writer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>


StatWriter::StatWriter(PlayerVector<Player*>& playerVector) :
    playerVector(playerVector) {}

void StatWriter::createDDTopList() {

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    size_t maxNameLength = playerVector.getLongestNameLength();

    // Calculate the number of files needed to write all players
    const int playersPerFile = 20;
    int nrOfPlayers = playerVector.size();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    int place = 1;
    for (int i = 0; i != nrOfFiles; i++) {
        std::string fileNr = std::to_string(i+1);
        std::string fileName;
        if (nrOfFiles == 1) {
            fileName = "dd top list";
        }
        else {
            fileName = ("dd top list " + fileNr);
        }
        std::ofstream file(fileName);
        if (file.is_open()) {
            file << "<a href=\"text://Top Damage:<br>" << std::endl;

            writeDDTopListHeadings(maxNameLength, file);

            auto start = totalDamageForEachPlayer.begin() +
                         i * playersPerFile;
            auto stop1 = totalDamageForEachPlayer.begin() +
                         (i + 1) * playersPerFile;
            auto stop2 = totalDamageForEachPlayer.end();
            for (auto it = start; it != stop1 && it != stop2; it++) {
                writePlace(place++, file);
                writeName(it->first, maxNameLength, file);
                writeDDTopList(it->second, file);
            }

            file << "\">Top " << i * playersPerFile + bool(!i) <<
                    " - " << (i + 1) * playersPerFile << "</a>" << std::endl;
        }
        else {
            // Write a function to write error common to all write functions.
        }
    }
}

void StatWriter::createDDDetailedOverview() {

    std::string fileNameBase = "dd overview";

    // Calculate the number of files needed to write all players
    const int playersPerFile = 12;
    int nrOfPlayers = playerVector.size();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    size_t maxNameLength = playerVector.getLongestNameLength();

    writeContentsToFile(fileNameBase,
                        totalDamageForEachPlayer,
                        nrOfFiles,
                        playersPerFile,
                        maxNameLength,
                        &StatWriter::writeDDDetailedOverviewHeadings);
}

void StatWriter::createDDPerOpponent(std::string playerName) {

    std::string fileNameBase = "dd per opponent for " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(fileNameBase + " 1",
                              playerName + " not found.");
        return;
    }

    // Calculate the number of files needed to write all players
    const int playersPerFile = 12;
    int nrOfPlayers = pp->nrOfAffectedPlayers();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    std::vector<std::pair<std::string, Damage>>
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageForAllAffectedPlayers();
    size_t maxNameLength = pp->getLongestAffectedPlayerNameLength();

    writeContentsToFile(fileNameBase,
                        totalDamageForEachAffectedPlayer,
                        nrOfFiles,
                        playersPerFile,
                        maxNameLength,
                        &StatWriter::writeDDDetailedOverviewHeadings);
}

void StatWriter::createDDOnSpecificOpponent(
    std::string playerName,
    std::string opponentName) {

    std::string fileNameBase =
        "dd on " + opponentName + " by " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(fileNameBase + " 1",
                              playerName + " not found.");
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getAllDamageFromAffectedPlayer(opponentName);
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // When the player is not found, a map with the key "empty" is returned.
    bool notFound = allDamageTypesFromAffectedPlayer.size() == 1 &&
                    (allDamageTypesFromAffectedPlayer[0].first == "empty");
    if (notFound) {
        createNotFoundMessage("dd on " + opponentName + " by " + pp->getName(),
                              opponentName + " not found among " +
                              playerName + "'s opponents.");
        return;
    }

    // Calculate the number of files needed to write all players
    const int typesPerFile = 2;
    int nrOfTypes = allDamageTypesFromAffectedPlayer.size();
    int nrOfFiles = nrOfTypes / typesPerFile +
                   (nrOfTypes % typesPerFile != 0);

    size_t maxNameLength = 20;

    writeContentsToFile(fileNameBase,
                        allDamageTypesFromAffectedPlayer,
                        nrOfFiles,
                        typesPerFile,
                        maxNameLength,
                        &StatWriter::writeDDOnSpecificOpponentHeadings);
}

void StatWriter::createNotFoundMessage(std::string fileName,
                                       std::string message) {
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << "<a href=\"text://" + message + "<br>" << std::endl;
        file << "\">" + fileName + "</a>" << std::endl;
    }
    else {
        // Write error.
    }
}

void StatWriter::writeContentsToFile(
    std::string fileNameBase,
    std::vector<std::pair<std::string, Damage>>& v,
    int nrOfFiles,
    int typesPerFile,
    size_t maxNameLength,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os)) {

    int place = 1;
    for (int fileNr = 1; fileNr <= nrOfFiles; fileNr++) {
        // Append the file nr to the file name
        std::string fileName = fileNameBase + " " + std::to_string(fileNr);

        // Do the actual writing:
        auto start = v.begin() +
                     (fileNr - 1) * typesPerFile;
        auto stop1 = v.begin() +
                    fileNr * typesPerFile;
        auto stop2 = v.end();
        writeContents(start,
                      stop1,
                      stop2,
                      fileName,
                      maxNameLength,
                      place,
                      writeHeadings);
    }
}

void StatWriter::writeContents(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop1,
    std::vector<std::pair<std::string, Damage>>::iterator stop2,
    std::string fileName,
    size_t maxNameLength,
    int& place,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os)) {

    std::ofstream file(fileName);
    if (file.is_open()) {

        file << "<a href=\"text://" + fileName + ":<br>" << std::endl;

        (this->*writeHeadings)(maxNameLength, file);

        for (auto it = start; it != stop1 && it != stop2; it++) {
            writePlace(place++, file);
            writeName(it->first, maxNameLength, file);
            writeDDDetailedOverview(it->second, file);
        }

        file << "\">" + fileName + "</a>" << std::endl;
    }
    else {
        // Write a function to write error common to all write functions.
    }

}

void StatWriter::createDDPerDamageType(std::string playerName) {

    Player* pp = playerVector.getPlayer(playerName);
    if (pp != nullptr) {
        // Calculate the number of files needed to write all players
        const int typesPerFile = 12;

        // Get the data and sort it:
        std::vector<std::pair<std::string, Damage>>
            allDamageTypesFromAffectedPlayer =
                pp->getTotalDamageForEveryDamageType();
        sortByDealt(allDamageTypesFromAffectedPlayer);

        int nrOfTypes = allDamageTypesFromAffectedPlayer.size();
        int nrOfFiles = nrOfTypes / typesPerFile +
                       (nrOfTypes % typesPerFile != 0);

        // TODO: This should be the length of the longest damage type
        size_t longestTypeNameLength = 20;

        int place = 1;
        auto start = allDamageTypesFromAffectedPlayer.begin();
        for (int i = 0; i != nrOfFiles; i++) {
            std::string fileNr = std::to_string(i+1);
            std::string fileName;
            if (nrOfFiles == 1) {
                fileName = "dd per damage type by " + pp->getName();
            }
            else {
                fileName = "dd per damage type by " + pp->getName() +
                           " " + fileNr;
            }
            std::ofstream file(fileName);
            if (file.is_open()) {
                file << "<a href=\"text://" + fileName + ":<br>" << std::endl;

                writeDDPerDamageTypeHeadings(longestTypeNameLength, file);

                auto stop1 = (i + 1) * typesPerFile;
                auto stop2 = allDamageTypesFromAffectedPlayer.end();
                int row = 0;

                for (auto it = start; row != stop1 && it != stop2; it++, row++) {
                    writePlace(place++, file);
                    writeName(it->first, longestTypeNameLength, file);
                    writeDDDetailedOverview(it->second, file);
                    std::advance(start, 1);
                }

                file << "\">" + fileName + "</a>" << std::endl;
            }
            else {
                // Write a function to write error common to all write functions.
            }
        }
    }
}

void StatWriter::createOldDDOverview() {
    std::ofstream file("old_damage_dealt_overview");
    if (file.is_open()) {
        size_t maxNameLength = playerVector.getLongestNameLength();

        file << std::left << std::setw(maxNameLength) << "Name";
        writeDDHeadings(file);

        std::sort(playerVector.begin(), playerVector.end(), compareTotalDealtOld);
        for (const Player* p : playerVector) {
            file << std::left << std::setw(maxNameLength) << p->getName();
            writeDDOld(p->getTotalDamage(), file);
        }
    }
    else {
        // Write error
    }
}

void StatWriter::createDRDetailedOverview() {
    std::string fileName = "damage_received_overview";
    std::ofstream file(fileName);
    if (file.is_open()) {
        size_t maxNameLength = playerVector.getLongestNameLength();

        file << std::left << std::setw(maxNameLength) << "Name";
        writeDDHeadings(file);

        std::sort(playerVector.begin(), playerVector.end(), compareTotalReceived);
        for (const Player* p : playerVector) {
            file << std::left << std::setw(maxNameLength) << p->getName();
            writeDR(p->getTotalDamage(), file);
        }
    }
    else {
        errorLog.write("Could not open" + fileName);
    }
}

/******************/
/* General prints */
/******************/

std::ostream& StatWriter::writeNameHeading(std::string category,
                                           size_t maxNameLength,
                                           std::ostream& os) {
    /* Writes "Name" in the heading. */
    writeName("     " + category, maxNameLength, os);
    return os;
}

std::ostream& StatWriter::writePlace(int place, std::ostream& os) {
    os << place << ". ";

    int placeNumbers = 1;
    while (place /= 10) {
        placeNumbers++;
    }

    switch(placeNumbers) {
        case 1 :
            os << "  ";
            break;
        case 2 :
            os << " ";
            break;
    }
    return os;
}

std::ostream& StatWriter::writeName(std::string name,
                                    size_t maxNameLength,
                                    std::ostream& os) {
    /* Writes the name and replaces as many trailing whitespaces
    as possible for tabs. */
        const int tabSize = 8;
        const int placeSize = 5;

        int charDiff = maxNameLength - name.size();
        int sizeOfFirstTab = tabSize - (name.size() + placeSize) % tabSize;

        int nrOfTabs = charDiff / tabSize; //+ (charDiff % tabSize != 0);
        if (sizeOfFirstTab < (charDiff - nrOfTabs * tabSize)) {
            nrOfTabs++;
        }
        //int sizeOfFirstTab = tabSize - name.size() % tabSize;

        if (nrOfTabs != 0) {
            os << name;
            for (int i = nrOfTabs; i != 0; i--) {
                os << '\t';
            }
            int remainingSpaces = charDiff -
                                  sizeOfFirstTab -
                                  (nrOfTabs - 1) * tabSize;
            for (int i = remainingSpaces; i != 0; i--) {
                os << ' ';
            }
        }
        else {
            os << std::left << std::setw(maxNameLength) << name << std::right;
        }
        return os;
}

/*******************/
/* Damage headings */
/*******************/

std::ostream& StatWriter::writeDDTopListHeadings(size_t maxNameLength,
                                                     std::ostream& os) {
    int width = 8;
    writeNameHeading("Name", maxNameLength, os);
    os << std::setw(width) << "Total" <<
          std::setw(width) << "DPM" << std::endl;
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverviewHeadings(
    size_t maxNameLength,
    std::ostream& os) {

    int width = 8;
    writeNameHeading("Name", maxNameLength, os);
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit%" <<
          std::setw(width) << "Nanobot%" <<
          std::setw(width) << "Miss%" <<
          std::setw(width) << "Deflect%" << std::endl;

    return os;
}

std::ostream& StatWriter::writeDDOnSpecificOpponentHeadings(
    size_t maxNameLength,
    std::ostream& os) {

    writeNameHeading("Name", maxNameLength, os);
    int width = 8;
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit%" <<
          std::setw(width) << "Nanobot%" <<
          std::setw(width) << "Miss%" <<
          std::setw(width) << "Deflect%" << std::endl;
    return os;
}

std::ostream& StatWriter::writeDDPerDamageTypeHeadings (
    size_t maxNameLength,
    std::ostream& os) {

    writeNameHeading("Type", maxNameLength, os);
    int width = 8;
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit%" <<
          std::setw(width) << "Nanobot%" <<
          std::setw(width) << "Miss%" <<
          std::setw(width) << "Deflect%" << std::endl;
    return os;
}

std::ostream& StatWriter::writeDDHeadings(std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "Count" <<
          std::setw(width) << "RegMax" <<
          std::setw(width) << "RegMin" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Crits" <<
          std::setw(width) << "CritMax" <<
          std::setw(width) << "CritMin" <<
          std::setw(width) << "Deflects" <<
          std::setw(width) << "Misses" << std::endl;
    return os;
}

/****************/
/* Damage stats */
/****************/

std::ostream& StatWriter::writeDDTopList(
    const Damage& d,
    std::ostream& os) {

    int width = 8;
    os << std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << damagePerMinute(0, 0) << std::endl;
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverview(
    const Damage& d,
    std::ostream& os) {

    int width = 8;
    double critPercentage = percentage(d.getCountReceivedFromPlayer(),
                                       d.getCritCountReceivedFromPlayer());
    double nanobotDamagePercentage = percentage(
                                      d.getTotalReceivedFromPlayer(),
                                      d.getNanobotTotalReceivedFromPlayer());
    double missPercentage = percentage(d.getCountReceivedFromPlayer(),
                                       d.getMissesReceivedFromPlayer());
    double deflectPercentage = percentage(d.getCountReceivedFromPlayer(),
                                          d.getDeflectsReceivedFromPlayer());

    os << std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << damagePerMinute(0, 0) << std::fixed << std::setprecision(1) <<
          std::setw(width) << critPercentage <<
          std::setw(width) << nanobotDamagePercentage <<
          std::setw(width) << missPercentage <<
          std::setw(width) << deflectPercentage << std::endl;
    return os;
}

std::ostream& StatWriter::writeDR(const Damage& d, std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << d.getTotalDealtOnPlayer() <<
          std::setw(width) << d.getCountDealtOnPlayer() <<
          std::setw(width) << ((d.getRegularMaxDealtOnPlayer() == -1) ?
                            0 : d.getRegularMaxDealtOnPlayer()) <<
          std::setw(width) << ((d.getRegularMinDealtOnPlayer() == std::numeric_limits<int>::max()) ?
                            0 : d.getRegularMinDealtOnPlayer()) <<
          std::setw(width) << d.getCritTotalDealtOnPlayer() <<
          std::setw(width) << d.getCritCountDealtOnPlayer() <<
          std::setw(width) << ((d.getCritMaxDealtOnPlayer() == -1) ?
                            0 : d.getCritMaxDealtOnPlayer()) <<
          std::setw(width) << ((d.getCritMinDealtOnPlayer() == std::numeric_limits<int>::max()) ?
                            0 : d.getCritMinDealtOnPlayer()) <<
          std::setw(width) << d.getDeflectsDealtOnPlayer() <<
          std::setw(width) << d.getMissesDealtOnPlayer() << std::endl;
    return os;
}

std::ostream& StatWriter::writeDDOld(const Damage& d, std::ostream& os) {
    int width = 8;
    os << std::right <<
          std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << d.getCountReceivedFromPlayer() <<
          std::setw(width) << ((d.getRegularMaxReceivedFromPlayer() == -1) ?
                            0 : d.getRegularMaxReceivedFromPlayer()) <<
          std::setw(width) << ((d.getRegularMinReceivedFromPlayer() == std::numeric_limits<int>::max()) ?
                            0 : d.getRegularMinReceivedFromPlayer()) <<
          std::setw(width) << d.getCritTotalReceivedFromPlayer() <<
          std::setw(width) << d.getCritCountReceivedFromPlayer() <<
          std::setw(width) << ((d.getCritMaxReceivedFromPlayer() == -1) ?
                            0 : d.getCritMaxReceivedFromPlayer()) <<
          std::setw(width) << ((d.getCritMinReceivedFromPlayer() == std::numeric_limits<int>::max()) ?
                            0 : d.getCritMinReceivedFromPlayer()) <<
          std::setw(width) << d.getDeflectsReceivedFromPlayer() <<
          std::setw(width) << d.getMissesReceivedFromPlayer() << std::endl;
    return os;
}


/********************/
/* Helper functions */
/********************/

double StatWriter::percentage(int total, int part) {
    if (part == 0) {
        return 0;
    }
    else {
        return (float)part * 100 / total;
    }
}

int StatWriter::damagePerMinute(int totalDamage, int startTime) {
    // get time now
    // diffTimeInMinutes = timeNow - startTime  // Convert to minutes
    // return totalDamage / diffTimeInMinutes

    (void)totalDamage;
    (void)startTime;
    return 0;
}

void StatWriter::sortByDealt(std::vector<std::pair<std::string, Damage>>& v) {
    std::sort(v.begin(), v.end(),
              [](std::pair<std::string, Damage> damagePair1,
                 std::pair<std::string, Damage> damagePair2) {
                     return damagePair1.second.getTotalReceivedFromPlayer() >
                            damagePair2.second.getTotalReceivedFromPlayer();
                 });
}

void StatWriter::sortByReceived(std::vector<std::pair<std::string, Damage>>& v) {
    std::sort(v.begin(), v.end(),
              [](std::pair<std::string, Damage> damagePair1,
                 std::pair<std::string, Damage> damagePair2) {
                     return damagePair1.second.getTotalDealtOnPlayer() >
                            damagePair2.second.getTotalDealtOnPlayer();
                 });
}

/***************/
/* Comparators */
/***************/

bool StatWriter::compareTotalDealt(
    const std::pair<std::string, Damage>& p1,
    const std::pair<std::string, Damage>& p2) {
    return p1.second.getTotalReceivedFromPlayer() >
           p2.second.getTotalReceivedFromPlayer();
}

bool StatWriter::compareTotalDealtOld(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalReceivedFromPlayer() >
           p2->getTotalDamage().getTotalReceivedFromPlayer();
}

bool StatWriter::compareTotalReceived(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalDealtOnPlayer() >
           p2->getTotalDamage().getTotalDealtOnPlayer();
}







void StatWriter::createDDOverviewUnsorted() {
    std::ofstream file("damage_dealt_overview_unsorted");
    if (file.is_open()) {
        size_t maxNameLength = playerVector.getLongestNameLength();

        file << std::left << std::setw(maxNameLength) << "Name";
        writeDDHeadings(file);

        //std::sort(playerVector.begin(), playerVector.end(), compareTotalReceivedFromPlayerOld);
        for (const Player* p : playerVector) {
            file << std::left << std::setw(maxNameLength) << p->getName();
                    writeDDOld(p->getTotalDamage(), file);
        }
    }
    else {
        // Write error
    }
}

//void StatWriter::createDamageDetailsPerDamageType(PlayerVector& playerVector, std::string& playerName) {
//
//    std::string fileName = "damage_per_damage_type_for_" + playerName;
//    std::ofstream file(fileName);
//    if (file.is_open()) {
//        Player* pp = playerVector.getPlayer(playerName);
//        if (pp) {
//            for (auto& player : pp->getDamageMap()) {
//                for (auto& damageType : player.second) {
//                    file << player.first;
//                    Damage d = damageType.second.at(true) + damageType.second.at(false);
//                    d.writeDRFromYou(file);
//                }
//            }
//        }
//    }
//}
