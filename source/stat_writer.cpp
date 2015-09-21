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


StatWriter::StatWriter(PlayerVector<Player*>& playerVector,
                       Configuration& config) :
    playerVector(playerVector),
    config(config) {}

/*******************/
/* Create DD files */
/*******************/

void StatWriter::createDDTopList() {

    std::string titleBase = "DD Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    // Calculate the number of text links (windows) needed to see all players
    const int playersPerWindow = 15;
    int nrOfPlayers = (int)playerVector.size();
    int nrOfWindows = nrOfPlayers / playersPerWindow +
                      (nrOfPlayers % playersPerWindow != 0);

    size_t maxNameLength = playerVector.getLongestNameLength();

    writeContentsToFile(
        titleBase,
        totalDamageForEachPlayer,
        nrOfWindows,
        playersPerWindow,
        maxNameLength,
        &StatWriter::writeDDTopListHeadings,
        &StatWriter::writeDDTopList);
}

void StatWriter::createDDDetailedTopList() {

    std::string titleBase = "DD Detailed Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 10;
    int nrOfPlayers = (int)playerVector.size();
    int nrOfWindows = nrOfPlayers / playersPerWindow +
                   (nrOfPlayers % playersPerWindow != 0);

    size_t maxNameLength = playerVector.getLongestNameLength();

    writeContentsToFile(
        titleBase,
        totalDamageForEachPlayer,
        nrOfWindows,
        playersPerWindow,
        maxNameLength,
        &StatWriter::writeDDDetailedOverviewHeadings,
        &StatWriter::writeDDDetailedOverview);
}

void StatWriter::createDDPerDamageType(std::string playerName) {

    std::string titleBase = "DD Per Damage Type By " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase,
                              playerName + " not found.");
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageType();
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // Calculate the number of files needed to write all players
    const int typesPerFile = 10;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = nrOfTypes / typesPerFile +
                   (nrOfTypes % typesPerFile != 0);

    // TODO: This should be the length of the longest damage type
    size_t longestTypeNameLength = 20;

    writeContentsToFile(
        titleBase,
        allDamageTypesFromAffectedPlayer,
        nrOfWindows,
        typesPerFile,
        longestTypeNameLength,
        &StatWriter::writeDDPerDamageTypeHeadings,
        &StatWriter::writeDDDetailedOverview);
}

void StatWriter::createDDPerOpponent(std::string playerName) {

    std::string titleBase = "DD Per Opponent For Player " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase,
                              playerName + " not found.");
        return;
    }

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 10;
    int nrOfPlayers = (int)pp->nrOfAffectedPlayers();
    int nrOfWindows = nrOfPlayers / playersPerWindow +
                   (nrOfPlayers % playersPerWindow != 0);

    std::vector<std::pair<std::string, Damage>>
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageForAllAffectedPlayers();
    sortByDealt(totalDamageForEachAffectedPlayer);

    size_t maxNameLength = pp->getLongestAffectedPlayerNameLength();

    writeContentsToFile(
        titleBase,
        totalDamageForEachAffectedPlayer,
        nrOfWindows,
        playersPerWindow,
        maxNameLength,
        &StatWriter::writeDDDetailedOverviewHeadings,
        &StatWriter::writeDDDetailedOverview);
}

void StatWriter::createDDOnSpecificOpponent(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase =
        "DD On " + opponentName + " By " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase,
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
        createNotFoundMessage(titleBase,
                              opponentName + " not found among " +
                              playerName + "'s opponents.");
        return;
    }

    // Calculate the number of files needed to write all players
    const int typesPerFile = 10;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = nrOfTypes / typesPerFile +
                   (nrOfTypes % typesPerFile != 0);

    size_t maxNameLength = 20;

    writeContentsToFile(
        titleBase,
        allDamageTypesFromAffectedPlayer,
        nrOfWindows,
        typesPerFile,
        maxNameLength,
        &StatWriter::writeDDOnSpecificOpponentHeadings,
        &StatWriter::writeDDDetailedOverview);
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

        std::sort(playerVector.begin(),
                  playerVector.end(),
                  compareTotalReceived);
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
    os << std::setfill(fillChar) <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverviewHeadings(
    size_t maxNameLength,
    std::ostream& os) {

    int width = 8;
    writeNameHeading("Name", maxNameLength, os);
    os << std::setfill(fillChar) << std::right <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Nanobot" <<
          std::setw(width) << "Miss" <<
          std::setw(width) << "Deflect" << "<br>" <<
          std::setfill(' ');

    return os;
}

std::ostream& StatWriter::writeDDOnSpecificOpponentHeadings(
    size_t maxNameLength,
    std::ostream& os) {

    writeNameHeading("Name", maxNameLength, os);
    int width = 8;
    os << std::right << std::setfill(fillChar) <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Nanobot" <<
          std::setw(width) << "Miss" <<
          std::setw(width) << "Deflect" << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDPerDamageTypeHeadings (size_t maxNameLength,
                                                        std::ostream& os) {

    writeNameHeading("Type", maxNameLength, os);
    int width = 8;
    os << std::right << std::setfill(fillChar) <<
          std::setw(width) << "Total" <<
          std::setw(width) << "DPM" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Nanobot" <<
          std::setw(width) << "Miss" <<
          std::setw(width) << "Deflect" << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDHeadings(std::ostream& os) {
    int width = 8;
    os << std::right << std::setfill(fillChar) <<
          std::setw(width) << "Total" <<
          std::setw(width) << "Count" <<
          std::setw(width) << "RegMax" <<
          std::setw(width) << "RegMin" <<
          std::setw(width) << "Crit" <<
          std::setw(width) << "Crits" <<
          std::setw(width) << "CritMax" <<
          std::setw(width) << "CritMin" <<
          std::setw(width) << "Deflects" <<
          std::setw(width) << "Misses" << "<br>" <<
          std::setfill(' ');
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

    const int width = 8;
    os << std::setfill(fillChar) <<
          std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << d.getDPMReceivedFromPlayer() << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverview(const Damage& d,
                                                  std::ostream& os) {

    double critPercentage = percentage(d.getCountReceivedFromPlayer(),
                                       d.getCritCountReceivedFromPlayer());
    double nanobotDamagePercentage = percentage(
                                      d.getTotalReceivedFromPlayer(),
                                      d.getNanobotTotalReceivedFromPlayer());
    double missPercentage = percentage(d.getCountReceivedFromPlayer(),
                                       d.getMissesReceivedFromPlayer());
    double deflectPercentage = percentage(d.getCountReceivedFromPlayer(),
                                          d.getDeflectsReceivedFromPlayer());
    const int width = 8;
    const int critOffset = 1;
    os << std::setfill(fillChar) <<
          std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << d.getDPMReceivedFromPlayer() <<
          std::fixed << std::setprecision(1) <<
          std::setw(width-critOffset) << critPercentage << '%' <<
          std::setw(width-critOffset) << nanobotDamagePercentage << '%' <<
          std::setw(width-critOffset) << missPercentage << '%' <<
          std::setw(width-critOffset) << deflectPercentage << '%' << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDR(const Damage& d, std::ostream& os) {
    const int width = 8;
    os << std::right << std::setfill(fillChar) <<
          std::setw(width) << d.getTotalDealtOnPlayer() <<
          std::setw(width) << d.getCountDealtOnPlayer() <<
          std::setw(width) << ((d.getRegularMaxDealtOnPlayer() == -1) ?
                                0 : d.getRegularMaxDealtOnPlayer()) <<
          std::setw(width) << ((d.getRegularMinDealtOnPlayer() ==
                                std::numeric_limits<int>::max()) ?
                                0 : d.getRegularMinDealtOnPlayer()) <<
          std::setw(width) << d.getCritTotalDealtOnPlayer() <<
          std::setw(width) << d.getCritCountDealtOnPlayer() <<
          std::setw(width) << ((d.getCritMaxDealtOnPlayer() == -1) ?
                                0 : d.getCritMaxDealtOnPlayer()) <<
          std::setw(width) << ((d.getCritMinDealtOnPlayer() ==
                                std::numeric_limits<int>::max()) ?
                                0 : d.getCritMinDealtOnPlayer()) <<
          std::setw(width) << d.getDeflectsDealtOnPlayer() <<
          std::setw(width) << d.getMissesDealtOnPlayer() << "<br>" <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDOld(const Damage& d, std::ostream& os) {
    const int width = 8;
    os << std::right << std::setfill(fillChar) <<
          std::setw(width) << d.getTotalReceivedFromPlayer() <<
          std::setw(width) << d.getCountReceivedFromPlayer() <<
          std::setw(width) << ((d.getRegularMaxReceivedFromPlayer() == -1) ?
                                0 : d.getRegularMaxReceivedFromPlayer()) <<
          std::setw(width) << ((d.getRegularMinReceivedFromPlayer() ==
                                std::numeric_limits<int>::max()) ?
                                0 : d.getRegularMinReceivedFromPlayer()) <<
          std::setw(width) << d.getCritTotalReceivedFromPlayer() <<
          std::setw(width) << d.getCritCountReceivedFromPlayer() <<
          std::setw(width) << ((d.getCritMaxReceivedFromPlayer() == -1) ?
                                0 : d.getCritMaxReceivedFromPlayer()) <<
          std::setw(width) << ((d.getCritMinReceivedFromPlayer() ==
                                std::numeric_limits<int>::max()) ?
                                0 : d.getCritMinReceivedFromPlayer()) <<
          std::setw(width) << d.getDeflectsReceivedFromPlayer() <<
          std::setw(width) << d.getMissesReceivedFromPlayer() << "<br>" <<
          std::setfill(' ');
    return os;
}

/*****************/
/* Common writes */
/*****************/

void StatWriter::createNotFoundMessage(std::string title,
                                       std::string message) {
    std::ofstream file("pdd");
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create \"pdd"
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
    file << "<a href=\"text://" + message + "<br>" <<
            "\">" + title + "</a>";
}

void StatWriter::writeContentsToFile(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>>& v,
    unsigned int nrOfWindows,
    int typesPerWindow,
    size_t maxNameLength,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os),
    std::ostream& (StatWriter::*writeDD)
        (const Damage& d, std::ostream& os)) {

    // Open the script file pdd here
    std::ofstream file("pdd");
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create \"pdd"
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }

    /* Sets the link name number and calls the write function for
    each link needed. */
    int place = 1;
    for (unsigned int windowNr = 0; windowNr != nrOfWindows; windowNr++) {
        // Append the interval to the title
        std::string interval = std::to_string(windowNr * typesPerWindow + 1) +
                               "-" + std::to_string((windowNr + 1) *
                                                    typesPerWindow);
        std::string title = titleBase + " " + interval;

        auto start = v.begin() + windowNr * typesPerWindow;
        auto stop = v.begin();
        // Stop at either the end or the nr of types per file.
        if (v.size() < (windowNr + 1) * typesPerWindow) {
            stop = v.end();
        }
        else {
            stop = v.begin() + (windowNr + 1) * typesPerWindow;
        }

        // Do the actual writing:
//        writeContents(start,
//                      stop,
//                      file,
//                      title,
//                      maxNameLength,
//                      place,
//                      writeHeadings,
//                      writeDD);
        writeContentsReadable(start,
                              stop,
                              file,
                              title,
                              maxNameLength,
                              place,
                              writeHeadings,
                              writeDD);
    }
}

void StatWriter::writeContents(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::ostream& file,
    std::string title,
    size_t maxNameLength,
    int& place,
    std::ostream& (StatWriter::*writeHeadings)
        (size_t maxNameLength, std::ostream& os),
    std::ostream& (StatWriter::*writeDD)
        (const Damage& d, std::ostream& os)) {

    /* Writes headings and then the name and DD for each Damage
    in the vector. */

    file << "<a href=\"text://<font color = #FFFF00>" <<
            title << "</font>:<br>" <<
            "<font color = #00FF00>";

    (this->*writeHeadings)(maxNameLength, file);

    for (auto it = start; it != stop; it++) {
        writePlace(place++, file);
        writeName(it->first, maxNameLength, file);
        (this->*writeDD)(it->second, file);
    }
    file << "</font>\">" + title + "</a>";
}

void StatWriter::writeContentsReadable(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::ostream& file,
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

    file << "<a href=\"text://<font color = #FFFF00>" + title <<
            "</font>:<br>" << std::endl <<
            "<font color = #00FF00>" << std::endl;

    (this->*writeHeadings)(maxNameLength, file);
    file << std::endl;

    for (auto it = start; it != stop; it++) {
        writePlace(place++, file);
        writeName(it->first, maxNameLength, file);
        (this->*writeDD)(it->second, file);
        file << std::endl;
    }
    file << "</font>\">" + title + "</a>" << std::endl;
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

            os << std::left << std::setfill(fillChar) <<
                  std::setw(maxNameLength) << name << std::right;
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

void StatWriter::sortByReceived(
    std::vector<std::pair<std::string, Damage>>& v) {

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

        std::sort(playerVector.begin(),
                  playerVector.end(),
                  compareTotalDealtOld);
        for (const Player* p : playerVector) {
            file << std::left << std::setw(maxNameLength) << p->getName();
            writeDDOld(p->getTotalDamage(), file);
        }
    }
    else {
        // Write error
    }
}
