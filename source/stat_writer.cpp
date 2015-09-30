#include "affected_player.h"
#include "damage.h"
#include "logger.h"
#include "player.h"
#include "stat_writer.h"
#include "xp.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
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

    writeDDToFile(
        titleBase,
        totalDamageForEachPlayer,
        nrOfWindows,
        playersPerWindow,
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

    writeDDToFile(
        titleBase,
        totalDamageForEachPlayer,
        nrOfWindows,
        playersPerWindow,
        &StatWriter::writeDDDetailedOverviewHeadingsOthers,
        &StatWriter::writeDDDetailedOverviewOthers);
}

void StatWriter::createDDPerDamageType(std::string playerName) {

    std::string titleBase = "DD Per Damage Type By " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.");
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeReceivedFromPlayer();
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // Calculate the number of files needed to write all players
    const int typesPerWindow = 10;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = nrOfTypes / typesPerWindow +
                   (nrOfTypes % typesPerWindow != 0);

    writeDDPointer wddp;
    writeHeadingsPointer whp;
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        wddp = &StatWriter::writeDDDetailedOverviewSelf;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsSelf;
    }
    else {
        wddp = &StatWriter::writeDDDetailedOverviewOthers;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsOthers;
    }

    writeDDToFile(
        titleBase,
        allDamageTypesFromAffectedPlayer,
        nrOfWindows,
        typesPerWindow,
        whp,
        wddp);
}

void StatWriter::createDDPerOpponent(std::string playerName) {

    std::string titleBase = "DD Per Opponent For " + playerName;

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

    writeDDPointer wddp;
    writeHeadingsPointer whp;
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        wddp = &StatWriter::writeDDDetailedOverviewSelf;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsSelf;
    }
    else {
        wddp = &StatWriter::writeDDDetailedOverviewOthers;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsOthers;
    }

    writeDDToFile(
        titleBase,
        totalDamageForEachAffectedPlayer,
        nrOfWindows,
        playersPerWindow,
        whp,
        wddp);
}

void StatWriter::createDDOnSpecificOpponent(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase =
        "DD On " + opponentName + " By " + playerName;

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.");
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getAllDamageDealtOnAffectedPlayer(opponentName);
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
    const int typesPerWindow = 10;
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = nrOfTypes / typesPerWindow +
                     (nrOfTypes % typesPerWindow != 0);

    writeDDPointer wddp;
    writeHeadingsPointer whp;
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        wddp = &StatWriter::writeDDDetailedOverviewSelf;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsSelf;
    }
    else {
        wddp = &StatWriter::writeDDDetailedOverviewOthers;
        whp = &StatWriter::writeDDDetailedOverviewHeadingsOthers;
    }

    writeDDToFile(
        titleBase,
        allDamageTypesFromAffectedPlayer,
        nrOfWindows,
        typesPerWindow,
        whp,
        wddp);
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

void StatWriter::createXPInfo() {

    std::string title = "XP Info";

    Player* pp = playerVector.getPlayer("You");
    if (pp == nullptr) {
        createNotFoundMessage(title, "You not found.");
        return;
    }

    XP xp = pp->getXp();

    std::ofstream file(config.getScriptsPath() + "pdd");
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create \"pdd"
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }

    file << "<a href=\"text://<font color = " + yellow + ">" <<
            title << "</font><br>";
    if (config.shouldWriteReadable()) {
        file << std::endl;
    }

    writeXPHeadingsOverall(file);

    file << "<font color = " + lime + ">";
    if (config.shouldWriteReadable()) {
        file << std::endl;
    }
    std::vector<std::string> types = xp.getTypes();
    for (const auto& type : types) {
        writeXPStatsOverview(xp, type, file);
    }
    file << "</font>";

    writeXPHeadingsDetailed(file);

    file << "<font color = " + lime + ">";
    if (config.shouldWriteReadable()) {
        file << std::endl;
    }
    for (const auto& type : types) {
        writeXPStatsDetailed(xp, type, file);
    }

    file << "</font>\">" + title + " for %m</a>";
    if (config.shouldWriteReadable()) {
        file << std::endl;
    }
}

/*****************************/
/* Create nano program files */
/*****************************/


/*****************/
/* Common writes */
/*****************/

void StatWriter::createNotFoundMessage(std::string title,
    std::string message) {
    std::ofstream file(config.getScriptsPath() + "pdd");
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create \"pdd"
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error: " + config.getScriptsPath());
    }
    file << "<a href=\"text://" + message + "<br>" <<
        "\">" + title + "</a>";
}

void StatWriter::writeDDToFile(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>>& v,
    unsigned int nrOfWindows,
    int typesPerWindow,
    std::ostream& (StatWriter::*writeHeadingsPointer)(std::ostream& os),
    std::ostream& (StatWriter::*writeDDPointer)
        (const Damage& d, std::ostream& os)) {

    std::ofstream file(config.getScriptsPath() + "pdd");
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
        if (config.shouldWriteReadable()) {
            writeDDStatsReadable(start,
                                 stop,
                                 file,
                                 title,
                                 place,
                                 writeHeadingsPointer,
                                 writeDDPointer);
        }
        else {
            writeDDStats(start,
                         stop,
                         file,
                         title,
                         place,
                         writeHeadingsPointer,
                         writeDDPointer);
        }
    }
}

void StatWriter::writeDDStats(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::ostream& file,
    std::string title,
    int& place,
    std::ostream& (StatWriter::*writeHeadingsPointer)(std::ostream& os),
    std::ostream& (StatWriter::*writeDDPointer)
        (const Damage& d, std::ostream& os)) {

    /* Writes headings and then the DD, place and name for each Damage
    in the vector.  */

    file << "<a href=\"text://<font color = " + yellow + ">" <<
            title << "</font><br>";

    (this->*writeHeadingsPointer)(file);

    file << "<font color = " + lime + ">";
    for (auto it = start; it != stop; it++) {
        (this->*writeDDPointer)(it->second, file);
        writePlace(place++, file);
        writeName(it->first, file);
        file << "<br>";
    }
    file << "</font>\">" + title + "</a>" << std::endl;
}

void StatWriter::writeDDStatsReadable(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::ostream& file,
    std::string title,
    int& place,
    std::ostream& (StatWriter::*writeHeadingsPointer)(std::ostream& os),
    std::ostream& (StatWriter::*writeDDPointer)
        (const Damage& d, std::ostream& os)) {

    /* Writes headings and then the DD, place and name for each Damage
    in the vector. This method places endl's at the end to make
    it human readable. */

    file << "<a href=\"text://<font color = " + yellow + ">" <<
            title << "</font><br>" << std::endl;

    (this->*writeHeadingsPointer)(file);

    file << std::endl << "<font color = " + lime + ">" << std::endl;
    for (auto it = start; it != stop; it++) {
        (this->*writeDDPointer)(it->second, file);
        writePlace(place++, file);
        writeName(it->first, file);
        file << "<br>" << std::endl;
    }
    file << "</font>\">" + title + "</a>" << std::endl;
}

std::ostream& StatWriter::writePlace(int place, std::ostream& os) {
    os << std::right << std::setw(3) << std::setfill(fillChar) <<
        " " + std::to_string(place) <<
        std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeName(std::string name, std::ostream& os) {
    /* Write the name or %m which is the name of the player executing
    the script. */
    os << ((name == "You") ? " %m" : " " + name);
    return os;
}

/*******************/
/* Damage headings */
/*******************/

std::ostream& StatWriter::writeDDTopListHeadings(std::ostream& os) {
    int width = 9;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">" <<
          std::setw(width + 2) << " Total " <<
          std::setw(width) << " DPM " <<
          "</font><br>" << std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverviewHeadingsOthers(
    std::ostream& os) {
    /* Headings without misses as they are not logged by AO for players
    other than yourself. (Unless they are hitting you) */
    return writeDDDetailedOverviewHeadings(os, false);
}

std::ostream& StatWriter::writeDDDetailedOverviewHeadingsSelf(
    std::ostream& os) {
    /* As the other detailed overview heading but with info on misses. */
    return writeDDDetailedOverviewHeadings(os, true);
}

std::ostream& StatWriter::writeDDDetailedOverviewHeadings(
    std::ostream& os, bool self) {

    int width = 9;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setw(width+1) << " Total " <<
          std::setw(width-1) << " DPM " <<
          std::setw(width+1) << " Crit " <<
          std::setw(width+1) << " Nanobot ";
    if (self) {
        os << std::setw(width) << " Miss ";
    }
    os << std::setw(width+2) << " Deflect " <<
          "</font><br>" << std::setfill(' ');

    return os;
}

std::ostream& StatWriter::writeDDPerDamageTypeHeadings(std::ostream& os) {

    int width = 9;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setw(width + 1) << " Total " <<
          std::setw(width - 1) << " DPM " <<
          std::setw(width + 1) << " Crit " <<
          std::setw(width + 1) << " Nanobot " <<
          std::setw(width) << " Miss " <<
          std::setw(width + 2) << " Deflect " <<
          "</font><br>" << std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDHeadings(std::ostream& os) {
    int width = 9;
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

std::ostream& StatWriter::writeXPHeadingsOverall(std::ostream& os) {
    int width = 9;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setw(width+3) << " Total " <<
          std::setw(width) << " XPH " <<
          "</font><br>" << std::setfill(' ');
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    return os;
}

std::ostream& StatWriter::writeXPHeadingsDetailed(std::ostream& os) {
    int width = 9;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setw(width + 3) << " Total " <<
          std::setw(width) << " XPH " <<
          std::setw(width + 1) << " Max " <<
          std::setw(width + 1) << " Min " <<
          "</font><br>" << std::setfill(' ');
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    return os;
}

/*******************/
/* Common headings */
/*******************/


/*****************/
/* Damage writes */
/*****************/

std::ostream& StatWriter::writeDDTopList(const Damage& d, std::ostream& os) {

    const int width = 9;
    os << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer()) << " " <<
          std::setfill(' ');
    return os;
}

std::ostream& StatWriter::writeDDDetailedOverviewOthers(const Damage& d,
                                                        std::ostream& os) {
    /* Details without misses as they are not logged by AO for players
    other than yourself. (Unless they are hitting you) */
    return writeDDDetailedOverview(d, os, false);
}

std::ostream& StatWriter::writeDDDetailedOverviewSelf(const Damage& d,
                                                      std::ostream& os) {
    /* As the other detailed overview but with info on misses. */
    return writeDDDetailedOverview(d, os, true);
}

std::ostream& StatWriter::writeDDDetailedOverview(const Damage& d,
                                                  std::ostream& os,
                                                  bool self) {
    double critPercentage = percentage(d.getCountReceivedFromPlayer(),
                                       d.getCritCountReceivedFromPlayer());
    double nanobotDamagePercentage = percentage(
                                      d.getTotalReceivedFromPlayer(),
                                      d.getNanobotTotalReceivedFromPlayer());
    std::string misses;
    if (self) {
        double missPercentage = percentage(d.getCountReceivedFromPlayer(),
                                           d.getMissesReceivedFromPlayer());
        misses = dblToString(missPercentage);
    }
    double deflectPercentage = percentage(d.getCountReceivedFromPlayer(),
                                          d.getDeflectsReceivedFromPlayer());
    std::string crit = dblToString(critPercentage);
    std::string nanobot = dblToString(nanobotDamagePercentage);

    std::string deflect = dblToString(deflectPercentage);

    const int width = 8;
    const int critOffset = 1;
    os << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer()) << " " <<
          std::fixed << std::setprecision(1) <<
          std::setw(width - critOffset) << " " + crit << '%' << " " <<
          std::setw(width - critOffset) << " " + nanobot << '%' << " ";
    if (self) {
        os << std::setw(width - critOffset) << " " + misses << '%' << " ";
    }
    os << std::setw(width - critOffset) << " " + deflect << '%' << "  " <<
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

/*************/
/* XP writes */
/*************/

std::ostream& StatWriter::writeXPStatsOverview(const XP& xp,
                                               std::string type,
                                               std::ostream& os) {
    const int width = 9;
    os << std::setfill(fillChar) <<
          std::setw(width + 1) << " " + std::to_string(xp.getTotal(type)) <<
                                  " " <<
          std::setw(width) << " " + std::to_string(xp.getXPH(type)) << " " <<
          std::setfill(' ');
    writeName(type, os);
    os << "<br>";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    return os;
}

std::ostream& StatWriter::writeXPStatsDetailed(const XP& xp,
                                               std::string type,
                                               std::ostream& os) {

    int maxGained = (xp.getMaxGained(type) == -1) ?
                     0 : xp.getMaxGained(type);
    int minGained = (xp.getMinGained(type) ==
                     std::numeric_limits<int>::max()) ?
                         0 : xp.getMinGained(type);
    int maxLost = (xp.getMaxLost(type) == -1) ?
                   0 : xp.getMaxLost(type);
    int minLost = (xp.getMinLost(type) ==
                   std::numeric_limits<int>::max()) ?
                       0 : xp.getMinLost(type);

    const int width = 9;
    os << std::setfill(fillChar) <<
          std::setw(width + 1) << " " + std::to_string(xp.getTotalGained(type))
                           << " " <<
          std::setw(width) << " " + std::to_string(xp.getXPHGained(type))
                           << " " <<
          std::setw(width) << " " + std::to_string(maxGained) << " " <<
          std::setw(width) << " " + std::to_string(minGained) << " " <<
          std::setfill(' ');
    writeName(type + " gained", os);
    os << "<br>";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setfill(fillChar) <<
          std::setw(width + 1) << " " + std::to_string(xp.getTotalLost(type))
                           << " " <<
          std::setw(width) << " N/A" << " " <<
          std::setw(width) << " " + std::to_string(maxLost) << " " <<
          std::setw(width) << " " + std::to_string(minLost) << " " <<
          std::setfill(' ');
    writeName(type + " lost", os);
    os << "<br>";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
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

std::string StatWriter::dblToString(const double d) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << d;
    return out.str();
}



// Remove when done:

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
