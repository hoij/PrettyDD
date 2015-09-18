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


StatWriter::StatWriter(PlayerVector<Player*>& playerVector, Configuration& config) :
    playerVector(playerVector),
    config(config) {}

/*******************/
/* Create DD files */
/*******************/

void StatWriter::createDDTopList() {

    std::string titleBase = "DD Top List";
    std::string fileNameBase = "DDTop";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    size_t maxNameLength = playerVector.getLongestNameLength();

    // Calculate the number of files needed to write all players
    const int playersPerFile = 20;
    int nrOfPlayers = (int)playerVector.size();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    std::vector<std::string> fileNames = writeContentsToFile(
        titleBase,
        fileNameBase,
        totalDamageForEachPlayer,
        nrOfFiles,
        playersPerFile,
        maxNameLength,
        &StatWriter::writeDDTopListHeadings,
        &StatWriter::writeDDTopList);

    createMainScriptFile(fileNames);
}

void StatWriter::createDDDetailedTopList() {
    
    std::string titleBase = "DD Detailed Top List";
    std::string fileNameBase = "DDDetailedTop";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    size_t maxNameLength = playerVector.getLongestNameLength();

    // Calculate the number of files needed to write all players
    const int playersPerFile = 10;
    int nrOfPlayers = (int)playerVector.size();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    std::vector<std::string> fileNames = writeContentsToFile(
        titleBase,
        fileNameBase,
        totalDamageForEachPlayer,
        nrOfFiles,
        playersPerFile,
        maxNameLength,
        &StatWriter::writeDDDetailedOverviewHeadings,
        &StatWriter::writeDDDetailedOverview);

    createMainScriptFile(fileNames);
}

void StatWriter::createDDPerDamageType(std::string playerName) {

    std::string titleBase = "DD Per Damage Type By " + playerName;
    std::string fileNameBase = "DDPerDamageTypeByPlayer";

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        std::vector<std::string> fileNames = {fileNameBase + "1"};
        createNotFoundMessage(fileNameBase + "1",
                              titleBase,
                              playerName + " not found.");
        createMainScriptFile(fileNames);
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageType();
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // Calculate the number of files needed to write all players
    const int typesPerFile = 12;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfFiles = nrOfTypes / typesPerFile +
                   (nrOfTypes % typesPerFile != 0);

    // TODO: This should be the length of the longest damage type
    size_t longestTypeNameLength = 20;

    std::vector<std::string> fileNames = writeContentsToFile(
        titleBase,
        fileNameBase,
        allDamageTypesFromAffectedPlayer,
        nrOfFiles,
        typesPerFile,
        longestTypeNameLength,
        &StatWriter::writeDDPerDamageTypeHeadings,
        &StatWriter::writeDDDetailedOverview);

    createMainScriptFile(fileNames);
}

void StatWriter::createDDPerOpponent(std::string playerName) {

    std::string titleBase = "DD Per Opponent For Player " + playerName;
    std::string fileNameBase = "DDPerOpponentForPlayer";

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        std::vector<std::string> fileNames = {fileNameBase};
        createNotFoundMessage(fileNameBase + "1",
                              titleBase,
                              playerName + " not found.");
        createMainScriptFile(fileNames);
        return;
    }

    // Calculate the number of files needed to write all players
    const int playersPerFile = 12;
    int nrOfPlayers = (int)pp->nrOfAffectedPlayers();
    int nrOfFiles = nrOfPlayers / playersPerFile +
                   (nrOfPlayers % playersPerFile != 0);

    std::vector<std::pair<std::string, Damage>>
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageForAllAffectedPlayers();
    sortByDealt(totalDamageForEachAffectedPlayer);

    size_t maxNameLength = pp->getLongestAffectedPlayerNameLength();

    std::vector<std::string> fileNames = writeContentsToFile(
        titleBase,
        fileNameBase,
        totalDamageForEachAffectedPlayer,
        nrOfFiles,
        playersPerFile,
        maxNameLength,
        &StatWriter::writeDDDetailedOverviewHeadings,
        &StatWriter::writeDDDetailedOverview);

    createMainScriptFile(fileNames);
}

void StatWriter::createDDOnSpecificOpponent(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase =
        "DD On " + opponentName + " By " + playerName;
    std::string fileNameBase = "DDOnPlayerByPlayer";

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(fileNameBase + "1",
                              titleBase,
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
                              titleBase,
                              opponentName + " not found among " +
                              playerName + "'s opponents.");
        return;
    }

    // Calculate the number of files needed to write all players
    const int typesPerFile = 12;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfFiles = nrOfTypes / typesPerFile +
                   (nrOfTypes % typesPerFile != 0);

    size_t maxNameLength = 20;

    std::vector<std::string> fileNames = writeContentsToFile(
        titleBase,
        fileNameBase,
        allDamageTypesFromAffectedPlayer,
        nrOfFiles,
        typesPerFile,
        maxNameLength,
        &StatWriter::writeDDOnSpecificOpponentHeadings,
        &StatWriter::writeDDDetailedOverview);

    createMainScriptFile(fileNames);
}

/*******************/
/* Create DR files */
/*******************/

void StatWriter::createDRDetailedTopList() {
    std::string fileName = "DamageReceivedOverview";
    std::ofstream file(config.getScriptsPath() + fileName);
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
        errorLog.write("Error: Could not open/create \"" + fileName +
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
}

/*********************/
/* Create heal files */
/*********************/


/*********************/
/* Create nano files */
/*********************/


/*******************/
/* Create XP files */
/*******************/


/*****************************/
/* Create nano program files */
/*****************************/


/*******************/
/* Damage headings */
/*******************/

std::ostream& StatWriter::writeDDTopListHeadings(size_t maxNameLength,
                                                 std::ostream& os) {
    int width = 8;
    writeNameHeading("Name", maxNameLength, os);
    os << std::setw(width) << "Total" <<
          std::setw(width) << "DPM" << "<br>";
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
          std::setw(width) << "Deflect%" << "<br>";

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
          std::setw(width) << "Deflect%" << "<br>";
    return os;
}

std::ostream& StatWriter::writeDDPerDamageTypeHeadings (size_t maxNameLength,
                                                        std::ostream& os) {

    writeNameHeading("Type", maxNameLength, os);
    int width = 8;
    os << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit%" <<
          std::setw(width) << "Nanobot%" <<
          std::setw(width) << "Miss%" <<
          std::setw(width) << "Deflect%" << "<br>";
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
          std::setw(width) << "Misses" << "<br>";
    return os;
}

/*******************/
/* Common headings */
/*******************/

std::ostream& StatWriter::writeNameHeading(std::string category,
                                           size_t maxNameLength,
                                           std::ostream& os) {
    /* Writes "Name" in the heading. */
    writeName("     " + category, maxNameLength + 5, os);
    return os;
}

/*****************/
/* Damage writes */
/*****************/

std::ostream& StatWriter::writeDDTopList(const Damage& d, std::ostream& os) {

    int width = 8;
    os << std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << d.getDPMReceivedFromPlayer() << "<br>";
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverview(const Damage& d,
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
          std::setw(width) << d.getDPMReceivedFromPlayer() <<
          std::fixed << std::setprecision(1) <<
          std::setw(width) << critPercentage <<
          std::setw(width) << nanobotDamagePercentage <<
          std::setw(width) << missPercentage <<
          std::setw(width) << deflectPercentage << "<br>";
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
          std::setw(width) << d.getMissesDealtOnPlayer() << "<br>";
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
          std::setw(width) << d.getMissesReceivedFromPlayer() << "<br>";
    return os;
}

/*****************/
/* Common writes */
/*****************/

void StatWriter::createNotFoundMessage(std::string fileName,
                                       std::string title,
                                       std::string message) {
    std::ofstream file(config.getScriptsPath() + fileName);
    if (file.is_open()) {
        file << "<a href=\"text://" + message + "<br>" <<
                "\">" + title + "</a>";
    }
    else {
        // Write error.
    }
}

std::vector<std::string> StatWriter::writeContentsToFile(
    std::string titleBase,
    std::string fileNameBase,
    std::vector<std::pair<std::string, Damage>>& v,
    unsigned int nrOfFiles,
    int typesPerFile,
    size_t maxNameLength,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os),
    std::ostream& (StatWriter::*writeDD)
        (const Damage& d, std::ostream& os)) {

    /* Sets the file name number and calls the write function for
    each file needed. Returns the file names. */

    std::vector<std::string> fileNames;

    int place = 1;
    for (unsigned int fileNr = 0; fileNr != nrOfFiles; fileNr++) {
        // Append the interval to the title
        std::string interval = std::to_string(fileNr * typesPerFile + 1) + "-" +
            std::to_string((fileNr + 1) * typesPerFile);
        std::string title = titleBase + " " + interval;
        // Append the file nr to the file name
        std::string fileName = fileNameBase + std::to_string(fileNr+1);
        fileNames.push_back(fileName);

        auto start = v.begin() + fileNr * typesPerFile;
        auto stop = v.begin();
        // Stop at either the end or the nr of types per file.
        if (v.size() < (fileNr + 1) * typesPerFile) {
            stop = v.end();
        }
        else {
            stop = v.begin() + (fileNr + 1) * typesPerFile;
        }
        // Do the actual writing:
        writeContents(start,
            stop,
            fileName,
            title,
            maxNameLength,
            place,
            writeHeadings,
            writeDD);
        //writeContentsReadable(start,
        //              stop,
        //              fileName,
        //              title,
        //              maxNameLength,
        //              place,
        //              writeHeadings,
        //              writeDD);
    }
    return fileNames;
}

void StatWriter::writeContents(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::string fileName,
    std::string title,
    size_t maxNameLength,
    int& place,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os),
    std::ostream& (StatWriter::*writeDD)
        (const Damage& d, std::ostream& os)) {

    /* Writes headings and then the name and DD for each Damage
    in the vector. */

    std::ofstream file(config.getScriptsPath() + fileName);
    if (file.is_open()) {

        file << "<a href=\"text://<font color = #FFFF00>" + title + "</font>:<br>";
        file << "<font color = #00FF00>";
        (this->*writeHeadings)(maxNameLength, file);

        for (auto it = start; it != stop; it++) {
            writePlace(place++, file);
            writeName(it->first, maxNameLength, file);
            (this->*writeDD)(it->second, file);
        }

        file << "</font>\">" + title + "</a>";
    }
    else {
        errorLog.write("Error: Could not open/create \"" + fileName +
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
}

void StatWriter::writeContentsReadable(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::string fileName,
    std::string title,
    size_t maxNameLength,
    int& place,
    std::ostream& (StatWriter::*writeHeadings)
    (size_t maxNameLength, std::ostream& os),
    std::ostream& (StatWriter::*writeDD)
    (const Damage& d, std::ostream& os)) {

    /* Writes headings and then the name and DD for each Damage
    in the vector. This method places endl's at the end to make
    it human readable. */

    std::ofstream file(config.getScriptsPath() + fileName);
    if (file.is_open()) {

        file << "<a href=\"text://" + title + ":<br>" << std::endl;

        (this->*writeHeadings)(maxNameLength, file);
        file << std::endl;

        for (auto it = start; it != stop; it++) {
            writePlace(place++, file);
            writeName(it->first, maxNameLength, file);
            (this->*writeDD)(it->second, file);
            file << std::endl;
        }

        file << "\">" + title + "</a>" << std::endl;
    }
    else {
        errorLog.write("Error: Could not open/create \"" + fileName +
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
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
        //const int tabSize = 8;
        //const int placeSize = 5;

        //int charDiff = (int)maxNameLength - (int)name.size();
        //int sizeOfFirstTab = tabSize - (name.size() + placeSize) % tabSize;

        //int nrOfTabs = charDiff / tabSize; //+ (charDiff % tabSize != 0);
        //if (sizeOfFirstTab < (charDiff - nrOfTabs * tabSize)) {
        //    nrOfTabs++;
        //}

        //if (nrOfTabs != 0) {
        //    os << name;
        //    for (int i = nrOfTabs; i != 0; i--) {
        //        os << '\t';
        //    }
        //    int remainingSpaces = charDiff -
        //                          sizeOfFirstTab -
        //                          (nrOfTabs - 1) * tabSize;
        //    for (int i = remainingSpaces; i != 0; i--) {
        //        os << ' ';
        //    }
        //}
        //else {
            os << std::left << std::setw(maxNameLength) << name << std::right;
        //}
        return os;
}

void StatWriter::createMainScriptFile(std::vector<std::string> fileNames) {

    std::ofstream file(config.getScriptsPath() + "pdd");
    if (file.is_open()) {
        for (const auto& fileName : fileNames) {
            file << "/" << fileName << " /delay 250" << std::endl;
        }
    }
    else {
        errorLog.write("Error: Could not open/create the main script file "
                       "\"pdd\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
}

/********************/
/* Helper functions */
/********************/

double StatWriter::percentage(int total, int part) {
    if (part == 0) {
        return 0;
    }
    else {
        return (double)part * 100 / total;
    }
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

bool StatWriter::compareTotalDealtOld(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalReceivedFromPlayer() >
           p2->getTotalDamage().getTotalReceivedFromPlayer();
}

bool StatWriter::compareTotalReceived(const Player* p1, const Player* p2) {
    return p1->getTotalDamage().getTotalDealtOnPlayer() >
           p2->getTotalDamage().getTotalDealtOnPlayer();
}




// Remove when done:

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
        errorLog.write("Error: Could not open/create the main script file "
            "\"pdd\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
}

void StatWriter::createOldDDOverview() {
    std::ofstream file(config.getScriptsPath() + "old_damage_dealt_overview");
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
