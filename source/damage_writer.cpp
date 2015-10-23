#include "damage.h"
#include "damage_writer.h"
#include "player.h"

#include <algorithm>
#include <iomanip>

DamageWriter::DamageWriter(PlayerVector<Player*>& playerVector,
                           Configuration& config,
                           std::ofstream& file) :
                           WriterHelper(config, file),
                           playerVector(playerVector) {}

/******/
/* DD */
/******/

void DamageWriter::createDDTopList() {

    std::string titleBase = "DD Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageDealtForEachPlayer =
        playerVector.getTotalDamageDealtForEachPlayer();
    sortByDealt(totalDamageDealtForEachPlayer);

    // Calculate the number of text links (windows) needed to see all players
    const int playersPerWindow = 15;
    int nrOfPlayers = (int)totalDamageDealtForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageDealtForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writeTopListHeadings,
                &DamageWriter::writeDDTopList);
}

void DamageWriter::createDDDetailedTopList() {

    std::string titleBase = "DD Detailed Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageDealtForEachPlayer =
        playerVector.getTotalDamageDealtForEachPlayer();
    sortByDealt(totalDamageDealtForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 7;
    int nrOfPlayers = (int)totalDamageDealtForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageDealtForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writeOverviewHeadings,
                &DamageWriter::writeDDOverview);

}

void DamageWriter::createDDPerDamageType(std::string playerName) {
    std::string titleBase = "DD Per Damage Type By " + playerName;
    createDamagePerDamageType(playerName,
                              titleBase,
                              8,
                              true,
                              false);
}

void DamageWriter::createDDPerDamageTypeDetailed(std::string playerName) {
    std::string titleBase = "Detailed DD Per Damage Type By " + playerName;
    createDamagePerDamageType(playerName,
                              titleBase,
                              2,
                              true,
                              true);
}

void DamageWriter::createDDPerOpponent(std::string playerName) {
    std::string titleBase = "DD Per Opponent For " + playerName;
    createDamagePerOpponent(playerName, titleBase, true, false);
}

void DamageWriter::createDDOnSpecificOpponent(std::string playerName,
                                              std::string opponentName) {
    std::string titleBase = "DD On " + opponentName + " By " + playerName;
    createDamageOnSpecificOpponent(playerName,
                                   opponentName,
                                   titleBase,
                                   7,
                                   false);
}

void DamageWriter::createDDOnSpecificOpponentDetailed(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase = "Detailed DD Per Damage Type On " +
                            opponentName + " By " +
                            playerName;
    createDamageOnSpecificOpponent(playerName,
                                   opponentName,
                                   titleBase,
                                   2,
                                   true);
}

void DamageWriter::createDamageOnSpecificOpponent(std::string playerName,
                                                  std::string opponentName,
                                                  std::string titleBase,
                                                  int typesPerWindow,
                                                  bool detailed) {

    std::string pName = renameIfSelf(playerName);
    Player* pp = playerVector.getPlayer(pName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.");
        return;
    }

    // Get the data and sort it:
    std::string oppName = renameIfSelf(opponentName);
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getAllDamageDealtOnAffectedPlayer(oppName);
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // Check if the opponent was found.
    // When the player is not found, a map with the key "empty" is returned.
    bool notFound = allDamageTypesFromAffectedPlayer.size() == 1 &&
                    (allDamageTypesFromAffectedPlayer[0].first == "empty");
    if (notFound) {
        createNotFoundMessage(titleBase,
                              opponentName + " not found among " +
                              playerName + "'s opponents.");
        return;
    }

    // Check if the names contain you who are running the program.
    bool self = playerName == "You" ||
                playerName == config.getplayerRunningProgram() ||
                opponentName == "You" ||
                opponentName == config.getplayerRunningProgram();

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    setDDPerTypeWriteMethods(whp, wdp, detailed);

    // Calculate the number of files needed to write all players
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                allDamageTypesFromAffectedPlayer,
                nrOfWindows,
                typesPerWindow,
                self,
                whp,
                wdp);
}

/******/
/* DR */
/******/

void DamageWriter::createDRTopList() {

    std::string titleBase = "Damage Received Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageReceivedForEachPlayer =
        playerVector.getTotalDamageReceivedForEachPlayer();
    sortByReceived(totalDamageReceivedForEachPlayer);

    const int playersPerWindow = 15;
    int nrOfPlayers = (int)totalDamageReceivedForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(
        titleBase,
        totalDamageReceivedForEachPlayer,
        nrOfWindows,
        playersPerWindow,
        false,
        &DamageWriter::writeTopListHeadings,
        &DamageWriter::writeDRTopList);
}

void DamageWriter::createDRDetailedTopList() {

    std::string titleBase = "Damage Received Detailed Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageReceivedForEachPlayer =
        playerVector.getTotalDamageReceivedForEachPlayer();
    sortByReceived(totalDamageReceivedForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 7;
    int nrOfPlayers = (int)totalDamageReceivedForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageReceivedForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writeOverviewHeadings,
                &DamageWriter::writeDROverview);
}

void DamageWriter::createDRPerDamageType(std::string playerName) {
    std::string titleBase = "Damage Received Per Damage Type By " +
                            playerName;
    createDamagePerDamageType(playerName,
                              titleBase,
                              8,
                              false,
                              false);
}

void DamageWriter::createDRPerOpponent(std::string playerName) {
    std::string titleBase = "Damage Received Per Opponent For " + playerName;
    createDamagePerOpponent(playerName, titleBase, false, false);
}

/*****************/
/* Common writes */
/*****************/


void DamageWriter::createDamagePerDamageType(std::string playerName,
                                             std::string titleBase,
                                             int typesPerWindow,
                                             bool dealt,
                                             bool detailed) {

    std::string pName = renameIfSelf(playerName);
    Player* pp = playerVector.getPlayer(pName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.");
        return;
    }

    std::vector<std::pair<std::string, Damage>> allDamageTypesFromAffectedPlayer;
    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    if (dealt) { // Damage dealt
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeReceivedFromPlayer();
        sortByDealt(allDamageTypesFromAffectedPlayer);
        setDDPerTypeWriteMethods(whp, wdp, detailed);
    }
    else { // Damage received
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeDealtOnPlayer();
        sortByReceived(allDamageTypesFromAffectedPlayer);
        setDRPerTypeWriteMethods(whp, wdp);
    }

    // Check if the name is you who are running the program.
    bool self = playerName == "You" ||
                playerName == config.getplayerRunningProgram();

    // Calculate the number of links needed to write all players
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                allDamageTypesFromAffectedPlayer,
                nrOfWindows,
                typesPerWindow,
                self,
                whp,
                wdp);
}

void DamageWriter::createDamagePerOpponent(std::string playerName,
                                           std::string titleBase,
                                           bool dealt,
                                           bool detailed) {
    /* Writes damage received or damage dealt depending on the bool "dealt".
    */

    std::string pName = renameIfSelf(playerName);
    Player* pp = playerVector.getPlayer(pName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.");
        return;
    }

    std::vector<std::pair<std::string, Damage>>
        totalDamageForEachAffectedPlayer;
    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    if (dealt) { // Damage dealt
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageReceivedFromPlayerForAllAffectedPlayers();
        sortByDealt(totalDamageForEachAffectedPlayer);
        setDDWriteMethods(whp, wdp, detailed);
    }
    else { // Damage received
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageDealtOnPlayerForAllAffectedPlayers();
        sortByReceived(totalDamageForEachAffectedPlayer);
        setDRWriteMethods(whp, wdp);
    }

    // Check if the name is you who are running the program.
    bool self = playerName == "You" ||
                playerName == config.getplayerRunningProgram();

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 6;
    int nrOfPlayers = (int)totalDamageForEachAffectedPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageForEachAffectedPlayer,
                nrOfWindows,
                playersPerWindow,
                self,
                whp,
                wdp);
}

void DamageWriter::setDDWriteMethods(writeHeadingsPointer& whp,
                                     writeDamagePointer& wdp,
                                     bool detailed) {
    if (detailed) {
        whp = &DamageWriter::writeOverviewHeadingsDetailed;
        wdp = &DamageWriter::writeDDOverviewDetailed;
    }
    else {
        whp = &DamageWriter::writeOverviewHeadings;
        wdp = &DamageWriter::writeDDOverview;
    }
}

void DamageWriter::setDDPerTypeWriteMethods(writeHeadingsPointer& whp,
                                            writeDamagePointer& wdp,
                                            bool detailed) {
    if (detailed) {
        whp = &DamageWriter::writeOverviewHeadingsDetailed;
        wdp = &DamageWriter::writeDDOverviewDetailed;
    }
    else {
        whp = &DamageWriter::writePerTypeOverviewHeadings;
        wdp = &DamageWriter::writeDDPerTypeOverview;
    }
}

void DamageWriter::setDRPerTypeWriteMethods(writeHeadingsPointer& whp,
                                            writeDamagePointer& wdp) {
        whp = &DamageWriter::writePerTypeOverviewHeadings;
        wdp = &DamageWriter::writeDRPerTypeOverview;
}

void DamageWriter::setDRWriteMethods(writeHeadingsPointer& whp,
                                     writeDamagePointer& wdp) {
        whp = &DamageWriter::writeOverviewHeadings;
        wdp = &DamageWriter::writeDROverview;
}

void DamageWriter::writeToFile(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>>& v,
    unsigned int nrOfWindows,
    int typesPerWindow,
    bool self,
    void (DamageWriter::*writeHeadingsPointer)(bool self),
    void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                             const Damage& d,
                                             int place,
                                             bool self)) {


    // The script file limit in AO is 4kb. So if the nrOfWindows is
    // greater than 4 a new file will be created (as each window link
    // is designed to (worst case) be as close to 1kb as possible).
    int place = 1;
    const unsigned int windowsPerFile = 4;
    unsigned int nrOfFiles = calcNrOfFiles(nrOfWindows, windowsPerFile);
    for (unsigned int fileNr = 1; fileNr <= nrOfFiles; fileNr++) {

        std::string fileName = "pdd";
        if (fileNr > 1) {
            fileName += std::to_string(fileNr);
        }

        if (!openFile(fileName)) {
            return;
        }

        /* Sets the link name number and calls the write function for
        each link needed. */
        for (unsigned int windowNr = 0; windowNr != windowsPerFile &&
                                        windowNr != nrOfWindows &&
                                        place <= (int)v.size(); windowNr++) {

            // TODO: Break out into a function:
            int startOffset = windowNr * typesPerWindow +
                              (fileNr - 1) * typesPerWindow * windowsPerFile;
            auto start = v.begin() + startOffset;
            auto stop = v.begin();  // Just to set the right type on stop.
            // Stop at either the end or the nr of types per file.
            int stopOffset = startOffset + typesPerWindow;
            if ((int)v.size() < stopOffset) {
                stop = v.end();
            }
            else {
                stop = v.begin() + stopOffset;
            }

            std::string title =
                appendInterval(titleBase, startOffset, stopOffset);

            writeStats(start,
                       stop,
                       title,
                       place,
                       self,
                       writeHeadingsPointer,
                       writeDamagePointer);

            // To prevent AO's flood protection from kicking in
            file << "/delay 1000" << std::endl;
        }

        // Write the command to execute the next file
        // (if it exists) at the end of the current file.
        if (fileNr < nrOfFiles) {
            file << "/delay 3000" << std::endl;
            file << "/pdd" << std::to_string(fileNr + 1) << std::endl;
        }

        closeFile();
    }
}

void DamageWriter::writeStats(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::string title,
    int& place,
    bool self,
    void (DamageWriter::*writeHeadingsPointer)(bool self),
    void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                             const Damage& d,
                                             int place,
                                             bool self)) {

    /* Writes headings and then the DD, place and name for each Damage
    in the vector.  */

    writeStartOfLink(title);

    if (writeHeadingsPointer != nullptr) {
        (this->*writeHeadingsPointer)(self);
    }

    file << "<font color = " + lime + ">" << nl;
    for (auto it = start; it != stop; it++) {
        (this->*writeDamagePointer)(it->first, it->second, place++, self);
    }
    file << "</font>";

    writeEndOfLink(title);
}

/************/
/* Headings */
/************/

void DamageWriter::writeTopListHeadings(bool self) {
    (void)self;
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width + 2) << " Total " <<
            std::setw(width) << " DPM " <<
            "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writeOverviewHeadings(bool self) {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width+2) << " Total " <<
            std::setw(6) << "(_Cnt) " <<
            std::setw(width-1) << " DPM " <<
            std::setw(width) << " Regular " <<
            std::setw(width+1) << " Special " <<
            std::setw(width) << " Nanobot " <<
            std::setw(width) << " Shield " <<
            std::setw(width) << " Crit " <<
            std::setw(width) << " Deflect ";
    if (self) {
        file << std::setw(width) << " Miss ";
    }
    file << "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writePerTypeOverviewHeadings(bool self) {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width+2) << " Total " <<
            std::setw(6) << "(_Cnt) " <<
            std::setw(width-1) << " DPM " <<
            std::setw(width) << " Regular " <<
            std::setw(width) << " Nanobot " <<
            std::setw(width) << " Crit " <<
            std::setw(width+1) << " Deflect ";
    if (self) {
        file << std::setw(width-1) << " Miss ";
    }
    file << "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writeOverviewHeadingsDetailed(bool self) {
    (void)self;
    const int width = 7;
    const int pcWidth = 6;
    const int nrWidth = 4;
    file << "<font color = " + lightBlue + ">" << nl;
    file << std::setfill(fillChar) << std::right <<
            std::setw(width+1) << " Dmg" << " (" <<
            std::setw(nrWidth) << "_Cnt" << ") " <<
            std::setw(pcWidth-1) << " Dmg" << "%  " <<
            std::setw(width-1) << " Max" << "-" << std::left <<
            std::setw(width) << "Min " << std::right <<
            std::setw(pcWidth+1) << " TotHit" << "%" <<
            std::setw(pcWidth+1) << "  CatHit" << "%<br><br>" <<
            "</font>" << nl;
}

/******************/
/* Stat writes */
/******************/

void DamageWriter::writeDDTopList(const std::string& name,
                                  const Damage& d,
                                  int place,
                                  bool self) {
    (void)self;
    const int width = 9;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDRTopList(const std::string& name,
                                  const Damage& d,
                                  int place,
                                  bool self) {
    (void)self;
    const int width = 9;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalDealtOnPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDPerTypeOverview(const std::string& name,
                                          const Damage& d,
                                          int place,
                                          bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string deflectHitPercentage =
        calcDeflectHitPercentageReceivedFromPlayer(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotalReceivedFromPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountReceivedFromPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer())
                             << " " <<
            std::fixed << std::setprecision(1);
    if (d.hasSpecialReceivedFromPlayer()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
        if (self) {
            std::string missPercentage = calcMissPercentageReceivedFromPlayer(d);
            file << std::setw(pcWidth) << " " + missPercentage << '%' << " ";
        }
    }
    else if (d.hasShieldReceivedFromPlayer() ||
             d.hasRegularMissReceivedFromPlayer()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% ";
        if (self) {
            file << std::setw(pcWidth) << na << "% ";
        }
    }
    else {
        std::string regularDmgPercentage =
            calcRegularDmgPercentageReceivedFromPlayer(d);
        std::string nanobotDmgPercentage =
            calcNanobotDmgPercentageReceivedFromPlayer(d);
        std::string critHitPercentage =
            calcCritHitPercentageReceivedFromPlayer(d);
        file << std::setw(pcWidth) << " " + regularDmgPercentage << '%' << " "
             << std::setw(pcWidth) << " " + nanobotDmgPercentage << '%' << " "
             << std::setw(pcWidth) << " " + critHitPercentage << '%' << " "
             << std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
        if (self) {
            file << std::setw(pcWidth) << na << "% ";
        }
    }

    file << std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDRPerTypeOverview(const std::string& name,
                                          const Damage& d,
                                          int place,
                                          bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string deflectHitPercentage =
        calcDeflectHitPercentageDealtOnPlayer(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotalDealtOnPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountDealtOnPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer())
                             << " " <<
            std::fixed << std::setprecision(1);
    if (d.hasSpecialDealtOnPlayer()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << " " + deflectHitPercentage << "% ";
        if (self) {
            std::string missPercentage = calcMissPercentageDealtOnPlayer(d);
            file << std::setw(pcWidth) << " " + missPercentage << "% ";
        }
    }
    else if (d.hasShieldDealtOnPlayer() ||
             d.hasRegularMissDealtOnPlayer()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% ";
        if (self) {
            file << std::setw(pcWidth) << na << "% ";
        }
    }
    else {
        std::string regularDmgPercentage =
            calcRegularDmgPercentageDealtOnPlayer(d);
        std::string nanobotDmgPercentage =
            calcNanobotDmgPercentageDealtOnPlayer(d);
        std::string critHitPercentage =
            calcCritHitPercentageDealtOnPlayer(d);
        file << std::setw(pcWidth) << " " + regularDmgPercentage << "% "
             << std::setw(pcWidth) << " " + nanobotDmgPercentage << "% "
             << std::setw(pcWidth) << " " + critHitPercentage << "% "
             << std::setw(pcWidth) << " " + deflectHitPercentage << "% ";
        if (self) {
            file << std::setw(pcWidth) << na << "% ";
        }
    }

    file << std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDOverview(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string regularDmgPercentage = calcRegularDmgPercentageReceivedFromPlayer(d);
    std::string specialDmgPercentage = calcSpecialDmgPercentageReceivedFromPlayer(d);
    std::string nanobotDmgPercentage = calcNanobotDmgPercentageReceivedFromPlayer(d);
    std::string shieldDmgPercentage = calcShieldDmgPercentageReceivedFromPlayer(d);

    std::string critHitPercentage = calcCritHitPercentageReceivedFromPlayer(d);
    std::string deflectHitPercentage = calcDeflectHitPercentageReceivedFromPlayer(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotalReceivedFromPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountReceivedFromPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(pcWidth) << " " + regularDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + specialDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + nanobotDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + shieldDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + critHitPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
    if (self) {
        if (d.hasSpecialReceivedFromPlayer()) {
            std::string missPercentage = calcMissPercentageReceivedFromPlayer(d);
            file << std::setw(pcWidth) << " " + missPercentage << '%' << " ";
        }
        else {
            file << std::setw(pcWidth) << na << "% ";
        }
    }
    file << std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDROverview(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string regularDmgPercentage = calcRegularDmgPercentageDealtOnPlayer(d);
    std::string specialDmgPercentage = calcSpecialDmgPercentageDealtOnPlayer(d);
    std::string nanobotDmgPercentage = calcNanobotDmgPercentageDealtOnPlayer(d);
    std::string shieldDmgPercentage = calcShieldDmgPercentageDealtOnPlayer(d);

    std::string critHitPercentage = calcCritHitPercentageDealtOnPlayer(d);
    std::string deflectHitPercentage = calcDeflectHitPercentageDealtOnPlayer(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotalDealtOnPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountDealtOnPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(pcWidth) << " " + regularDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + specialDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + nanobotDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + shieldDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + critHitPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
    if (self) {
        if (d.hasSpecialDealtOnPlayer()) {
            std::string missPercentage = calcMissPercentageDealtOnPlayer(d);
            file << std::setw(pcWidth) << " " + missPercentage << '%' << " ";
        }
        else {
            file << std::setw(pcWidth) << na << "% ";
        }
    }
    file << std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDOverviewDetailed(const std::string& name,
                                           const Damage& d,
                                           int place,
                                           bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    file << "<font color = " + lightBlue + ">" << nl <<
            place << ". " << name << "</font><br>" << nl;

    if (d.hasRegularReceivedFromPlayer() &&
        d.hasNanobotReceivedFromPlayer()) {
        // Write info on the sum of the regular and nanobot damage only
        // if they both exist. No point writing this otherwise as it would be
        // identical to the total info written in respective branch.
        writeTotalInfo(d.getTotalReceivedFromPlayer(),
                       d.getCountReceivedFromPlayer(),
                       "");
        file << "<br>" << nl;
    }

    // If there is any regular damage
    if (d.hasRegularReceivedFromPlayer()) {
        writeDetailedRegularInfo(d);
    }

    // If there is any nanobot damage
    if (d.hasNanobotReceivedFromPlayer()) {
        writeDetailedNanobotInfo(d);
    }

    if (d.hasSpecialReceivedFromPlayer()) {
        writeDetailedSpecialInfo(d, self);
    }

    if (d.hasShieldReceivedFromPlayer()) {
        writeDetailedShieldInfo(d);
    }

    if (d.hasRegularMissReceivedFromPlayer()) {
        writeDetailedMissInfo(d, self);
    }

    file << "<br>" << nl << nl;
}

void DamageWriter::writeDetailedRegularInfo(const Damage& d) {

    int totalRegularDmg = d.getNormalTotalReceivedFromPlayer() +
                          d.getRegularDeflectTotalReceivedFromPlayer() +
                          d.getCritTotalReceivedFromPlayer();
    int totalRegularCount = d.getNormalCountReceivedFromPlayer() +
                            d.getRegularDeflectCountReceivedFromPlayer() +
                            d.getCritCountReceivedFromPlayer() +
                            d.getRegularMissesReceivedFromPlayer();
    writeTotalInfo(totalRegularDmg, totalRegularCount, "Regular");

    // Normal hits. Regular hits that are neither crits nor deflects.
    if (d.getNormalCountReceivedFromPlayer() > 0) {
        // Percentage of total hits for the type
        std::string normalHitPercentageOfType = percentage(
            d.getCountReceivedFromPlayer(),
            d.getNormalCountReceivedFromPlayer());
        // Percentage of regular hits
        std::string normalHitPercentageOfCategory = percentage(
            d.getNormalCountReceivedFromPlayer() +
            d.getCritCountReceivedFromPlayer() +
            d.getRegularDeflectCountReceivedFromPlayer() +
            d.getRegularMissesReceivedFromPlayer(),
            d.getNormalCountReceivedFromPlayer());
        // Percentage of total damage for the type
        std::string normalDmgPercentageOfType = percentage(
            d.getTotalReceivedFromPlayer(),
            d.getNormalTotalReceivedFromPlayer());
        std::string normalMin = determineMin(d.getNormalMinReceivedFromPlayer());
        std::string normalMax = determineMax(d.getNormalMaxReceivedFromPlayer());
        writeDetailedInfoForType("Normal",
                                 d.getNormalTotalReceivedFromPlayer(),
                                 d.getNormalCountReceivedFromPlayer(),
                                 normalDmgPercentageOfType,
                                 normalMax,
                                 normalMin,
                                 normalHitPercentageOfType,
                                 normalHitPercentageOfCategory);
    }

    // Crit
    if (d.getCritCountReceivedFromPlayer() > 0) {
        // Percentage of total hits for the type
        std::string critHitPercentageOfType = percentage(
            d.getCountReceivedFromPlayer(),
            d.getCritCountReceivedFromPlayer());
        // Percentage of regular hits
        std::string critHitPercentageOfCategory =
            calcCritHitPercentageReceivedFromPlayer(d);
        // Percentage of total damage for the type
        std::string critDmgPercentageOfType = percentage(
            d.getTotalReceivedFromPlayer(),
            d.getCritTotalReceivedFromPlayer());
        std::string critMin = determineMin(d.getCritMinReceivedFromPlayer());
        std::string critMax = determineMax(d.getCritMaxReceivedFromPlayer());
        writeDetailedInfoForType("Crit",
                                 d.getCritTotalReceivedFromPlayer(),
                                 d.getCritCountReceivedFromPlayer(),
                                 critDmgPercentageOfType,
                                 critMax,
                                 critMin,
                                 critHitPercentageOfType,
                                 critHitPercentageOfCategory);
    }

    // Deflect
    if (d.getRegularDeflectCountReceivedFromPlayer() > 0) {
        // Percentage of total hits for the type
        std::string deflectHitPercentageOfType = percentage(
            d.getCountReceivedFromPlayer(),
            d.getRegularDeflectCountReceivedFromPlayer());
        // Percentage of regular hits
        std::string deflectHitPercentageOfCategory =
            calcDeflectHitPercentageReceivedFromPlayer(d);
        // Percentage of total damage for the type
        std::string deflectDmgPercentageOfType = percentage(
            d.getTotalReceivedFromPlayer(),
            d.getRegularDeflectTotalReceivedFromPlayer());
        std::string regularDeflectMax = determineMax(
            d.getRegularDeflectMaxReceivedFromPlayer());
        std::string regularDeflectMin = determineMin(
            d.getRegularDeflectMinReceivedFromPlayer());
        writeDetailedInfoForType("Deflect",
                                 d.getRegularDeflectTotalReceivedFromPlayer(),
                                 d.getRegularDeflectCountReceivedFromPlayer(),
                                 deflectDmgPercentageOfType,
                                 regularDeflectMax,
                                 regularDeflectMin,
                                 deflectHitPercentageOfType,
                                 deflectHitPercentageOfCategory);
    }

    if (d.hasNanobotReceivedFromPlayer()) {
        // Write an extra line to separate regular from nanobot.
        file << "<br>" << nl;
    }
}

void DamageWriter::writeDetailedNanobotInfo(const Damage& d) {

    // Percentage of total hits for the type/category (the same in this case)
    std::string nanobotHitPercentageOfType = percentage(
        d.getCountReceivedFromPlayer(),
        d.getNanobotCountReceivedFromPlayer());

    // Percentage of total damage for the type
    std::string nanobotDmgPercentageOfType = calcNanobotDmgPercentageReceivedFromPlayer(d);

    std::string nanobotMin = determineMin(d.getNanobotMinReceivedFromPlayer());
    std::string nanobotMax = determineMax(d.getNanobotMaxReceivedFromPlayer());

    writeDetailedInfoForType("Nanobot",
                             d.getNanobotTotalReceivedFromPlayer(),
                             d.getNanobotCountReceivedFromPlayer(),
                             nanobotDmgPercentageOfType,
                             nanobotMax,
                             nanobotMin,
                             nanobotHitPercentageOfType);
}

void DamageWriter::writeDetailedSpecialInfo(const Damage& d, bool self) {

    // Only write the total info incase both normal and deflected
    // special damage exists. Otherwise the total displays duplicate info.
    if (d.hasSpecialReceivedFromPlayer() &&
        d.getSpecialDeflectTotalReceivedFromPlayer() > 0) {

        writeTotalInfo(d.getSpecialTotalReceivedFromPlayer() +
                       d.getSpecialDeflectTotalReceivedFromPlayer(),
                       d.getSpecialCountReceivedFromPlayer() +
                       d.getSpecialDeflectCountReceivedFromPlayer() +
                       d.getSpecialMissesReceivedFromPlayer(),
                       "");
    }

    if (d.getSpecialCountReceivedFromPlayer() > 0) {
        // Percentage of total hits for the type/category (the same in this case)
        std::string specialHitPercentageOfType = percentage(
            d.getCountReceivedFromPlayer(),
            d.getSpecialCountReceivedFromPlayer());
        // Percentage of total damage for the type
        std::string specialDmgPercentage = percentage(
            d.getTotalReceivedFromPlayer(),
            d.getSpecialTotalReceivedFromPlayer());
        // Min/Max
        std::string specialMin = determineMin(d.getSpecialMinReceivedFromPlayer());
        std::string specialMax = determineMax(d.getSpecialMaxReceivedFromPlayer());
        writeDetailedInfoForType("Normal",
                                 d.getSpecialTotalReceivedFromPlayer(),
                                 d.getSpecialCountReceivedFromPlayer(),
                                 specialDmgPercentage,
                                 specialMax,
                                 specialMin,
                                 specialHitPercentageOfType);
    }

    if (d.getSpecialDeflectCountReceivedFromPlayer() > 0) {
        // Percentage of total hits for the type/category (the same in this case)
        std::string deflectHitPercentageOfType =
            calcSpecialDeflectHitPercentageReceivedFromPlayer(d);
        // Percentage of total damage for the category
        std::string deflectDmgPercentage = percentage(
            d.getTotalReceivedFromPlayer(),
            d.getSpecialDeflectTotalReceivedFromPlayer());

        std::string specialDeflectMax = determineMax(
            d.getSpecialDeflectMaxReceivedFromPlayer());
        std::string specialDeflectMin = determineMin(
            d.getSpecialDeflectMinReceivedFromPlayer());
        writeDetailedInfoForType("Deflect",
                                 d.getSpecialDeflectTotalReceivedFromPlayer(),
                                 d.getSpecialDeflectCountReceivedFromPlayer(),
                                 deflectDmgPercentage,
                                 specialDeflectMax,
                                 specialDeflectMin,
                                 deflectHitPercentageOfType);
    }

    int misses = d.getSpecialMissesReceivedFromPlayer();
    if (self && misses > 0) {
        std::string missPercentage = calcSpecialMissPercentageReceivedFromPlayer(d);
        std::string na = "__._";
        writeDetailedInfoForType("Miss",
                                 -1,
                                 misses,
                                 na,
                                 "",
                                 "",
                                 missPercentage);
    }
}

void DamageWriter::writeDetailedShieldInfo(const Damage& d) {

    std::string shieldDmg = std::to_string(d.getShieldTotalReceivedFromPlayer());
    int shieldCount = d.getShieldCountReceivedFromPlayer();
    std::string minHit = determineMin(d.getShieldMinReceivedFromPlayer());
    std::string maxHit = determineMax(d.getShieldMaxReceivedFromPlayer());
    std::string na = "__._";

    writeDetailedInfoForType("Normal",
                             -1,
                             shieldCount,
                             na,
                             maxHit,
                             minHit);
}

void DamageWriter::writeDetailedMissInfo(const Damage& d, bool self) {
    // TODO: Is this self bool really necessary? Won't this only be called
    // if there is any miss in which case the player is self?
    int misses = d.getRegularMissesReceivedFromPlayer();
    if (self && misses > 0) {
        // Only including this for regular attacks
        std::string missPercentage = calcRegularMissPercentageReceivedFromPlayer(d);
        const int width = 7;
        const int nrWidth = 4;
        file << std::right <<
                std::setw(width) << "" << "  (" <<
                std::setw(nrWidth) << std::to_string(misses)
                                   << ") " << "Miss<br>" << nl;
    }
}

void DamageWriter::writeTotalInfo(int total, int count, std::string category) {
    const int width = 7;
    const int nrWidth = 4;
    file << std::right << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(total)
                               << " (" <<
            std::setw(nrWidth) << std::to_string(count)
                               << ") " << category << " Total" << "<br>" << nl;
}

void DamageWriter::writeDetailedInfoForType(std::string type,
                                            int total,
                                            int nrOfHits,
                                            std::string dmgPercent,
                                            std::string maxHit,
                                            std::string minHit,
                                            std::string hitPercentOfType,
                                            std::string hitPercentOfCategory) {
    /* hitPercentOfCategory should be the hit rate of normal, crit,
    deflect and miss within the category. This is only applicable to
    the "regular" category. For nanobots and specials, the
    hitPercentOfCategory will be identical to the hitPercentOfType
    and in that case, no stat will be shown for hitPercentOfType.
    hitPercentOfType should for regular attacks be the hit rate per
    regular and nanobot attacks combined. (If they both exist within
    the type). */

    const int width = 7;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string tot = total == -1 ? "" : std::to_string(total);
    
    std::string hits = std::to_string(nrOfHits);
    file << std::setw(width+1) << " " + tot << " (" <<
            std::setw(nrWidth) << hits << ") " <<
            std::setw(pcWidth) << " " + dmgPercent << "% " <<
            std::setw(width) << " " + maxHit << "-" << std::left <<
            std::setw(width) << minHit + " " << " " << std::right;
    if (!hitPercentOfType.empty()) {
        file << std::setw(pcWidth) << " " + hitPercentOfType << "% " << nl;
    }
    if (!hitPercentOfCategory.empty()) {
        file << std::setw(pcWidth) << " " + hitPercentOfCategory << "% ";
    }
    file << type << "<br>" << nl;
}

/********************/
/* Helper functions */
/********************/

void DamageWriter::sortByDealt(std::vector<std::pair<std::string, Damage>>& v) {
    std::sort(v.begin(), v.end(),
              [](std::pair<std::string, Damage> damagePair1,
                 std::pair<std::string, Damage> damagePair2) {
                     return damagePair1.second.getTotalReceivedFromPlayer() >
                            damagePair2.second.getTotalReceivedFromPlayer();
                 });
}

void DamageWriter::sortByReceived(
    std::vector<std::pair<std::string, Damage>>& v) {

    std::sort(v.begin(), v.end(),
              [](std::pair<std::string, Damage> damagePair1,
                 std::pair<std::string, Damage> damagePair2) {
                     return damagePair1.second.getTotalDealtOnPlayer() >
                            damagePair2.second.getTotalDealtOnPlayer();
                 });
}

std::string DamageWriter::calcCritHitPercentageReceivedFromPlayer(const Damage&d) {
    /* Based on attacks that have the potential to crit */
    int regularHits = d.getNormalCountReceivedFromPlayer() +
                      d.getCritCountReceivedFromPlayer() +
                      d.getRegularDeflectCountReceivedFromPlayer() +
                      d.getRegularMissesReceivedFromPlayer();
    int crits = d.getCritCountReceivedFromPlayer();

    return percentage(regularHits, crits);
}

std::string DamageWriter::calcCritHitPercentageDealtOnPlayer(const Damage&d) {
    /* Based on attacks that have the potential to crit */
    int regularHits = d.getNormalCountDealtOnPlayer() +
                      d.getCritCountDealtOnPlayer() +
                      d.getRegularDeflectCountDealtOnPlayer() +
                      d.getRegularMissesDealtOnPlayer();
    int crits = d.getCritCountDealtOnPlayer();

    return percentage(regularHits, crits);
}

std::string DamageWriter::calcDeflectHitPercentageReceivedFromPlayer(const Damage&d) {
    int regularHits = calcRegularAndSpecialHitsReceivedFromPlayer(d);
    int deflects = d.getRegularDeflectCountReceivedFromPlayer() +
                   d.getSpecialDeflectCountReceivedFromPlayer();

    return percentage(regularHits, deflects);
}

std::string DamageWriter::calcDeflectHitPercentageDealtOnPlayer(const Damage&d) {
    int regularHits = calcRegularAndSpecialHitsDealtOnPlayer(d);
    int deflects = d.getRegularDeflectCountDealtOnPlayer() +
                   d.getSpecialDeflectCountDealtOnPlayer();

    return percentage(regularHits, deflects);
}

std::string DamageWriter::calcSpecialDeflectHitPercentageReceivedFromPlayer(const Damage&d) {
    int specialHits = d.getSpecialCountReceivedFromPlayer() +
                      d.getSpecialDeflectCountReceivedFromPlayer() +
                      d.getSpecialMissesReceivedFromPlayer();
    int deflects = d.getSpecialDeflectCountReceivedFromPlayer();

    return percentage(specialHits, deflects);
}

std::string DamageWriter::calcMissPercentageReceivedFromPlayer(const Damage&d) {
    int regularHits = calcRegularAndSpecialHitsReceivedFromPlayer(d);
    int misses = d.getRegularMissesReceivedFromPlayer() +
                 d.getSpecialMissesReceivedFromPlayer();

    return percentage(regularHits, misses);
}

std::string DamageWriter::calcMissPercentageDealtOnPlayer(const Damage&d) {
    int regularHits = calcRegularAndSpecialHitsDealtOnPlayer(d);
    int misses = d.getRegularMissesDealtOnPlayer() +
                 d.getSpecialMissesDealtOnPlayer();

    return percentage(regularHits, misses);
}

std::string DamageWriter::calcRegularMissPercentageReceivedFromPlayer(const Damage&d) {
    int regularHits = d.getNormalCountReceivedFromPlayer() +
                      d.getCritCountReceivedFromPlayer() +
                      d.getRegularDeflectCountReceivedFromPlayer() +
                      d.getRegularMissesReceivedFromPlayer();
    int misses = d.getRegularMissesReceivedFromPlayer();

    return percentage(regularHits, misses);
}

std::string DamageWriter::calcSpecialMissPercentageReceivedFromPlayer(const Damage&d) {
    int regularHits = d.getSpecialCountReceivedFromPlayer() +
                      d.getSpecialDeflectCountReceivedFromPlayer() +
                      d.getSpecialMissesReceivedFromPlayer();
    int misses = d.getSpecialMissesReceivedFromPlayer();

    return percentage(regularHits, misses);
}

int DamageWriter::calcRegularAndSpecialHitsReceivedFromPlayer(const Damage&d) {
    return d.getNormalCountReceivedFromPlayer() +
           d.getCritCountReceivedFromPlayer() +
           d.getRegularDeflectCountReceivedFromPlayer() +
           d.getSpecialCountReceivedFromPlayer() +
           d.getSpecialDeflectCountReceivedFromPlayer() +
           d.getRegularMissesReceivedFromPlayer() +
           d.getSpecialMissesReceivedFromPlayer();
}

int DamageWriter::calcRegularAndSpecialHitsDealtOnPlayer(const Damage&d) {
    return d.getNormalCountDealtOnPlayer() +
           d.getCritCountDealtOnPlayer() +
           d.getRegularDeflectCountDealtOnPlayer() +
           d.getSpecialCountDealtOnPlayer() +
           d.getSpecialDeflectCountDealtOnPlayer() +
           d.getRegularMissesDealtOnPlayer() +
           d.getSpecialMissesDealtOnPlayer();
}

std::string DamageWriter::calcRegularDmgPercentageReceivedFromPlayer(const Damage&d) {
    int totalDmg = d.getTotalReceivedFromPlayer();
    int regularDmg = d.getNormalTotalReceivedFromPlayer() +
                     d.getRegularDeflectTotalReceivedFromPlayer() +
                     d.getCritTotalReceivedFromPlayer();

    return percentage(totalDmg, regularDmg);
}

std::string DamageWriter::calcRegularDmgPercentageDealtOnPlayer(const Damage&d) {
    int totalDmg = d.getTotalDealtOnPlayer();
    int regularDmg = d.getNormalTotalDealtOnPlayer() +
                     d.getRegularDeflectTotalDealtOnPlayer() +
                     d.getCritTotalDealtOnPlayer();

    return percentage(totalDmg, regularDmg);
}

std::string DamageWriter::calcSpecialDmgPercentageReceivedFromPlayer(const Damage&d) {
    int totalDmg = d.getTotalReceivedFromPlayer();
    int specialDmg = d.getSpecialTotalReceivedFromPlayer() +
                     d.getSpecialDeflectTotalReceivedFromPlayer();

    return percentage(totalDmg, specialDmg);
}

std::string DamageWriter::calcSpecialDmgPercentageDealtOnPlayer(const Damage&d) {
    int totalDmg = d.getTotalDealtOnPlayer();
    int specialDmg = d.getSpecialTotalDealtOnPlayer() +
                     d.getSpecialDeflectTotalDealtOnPlayer();

    return percentage(totalDmg, specialDmg);
}

std::string DamageWriter::calcNanobotDmgPercentageReceivedFromPlayer(const Damage&d) {
    int totalDmg = d.getTotalReceivedFromPlayer();
    int nanobotDmg = d.getNanobotTotalReceivedFromPlayer();

    return percentage(totalDmg, nanobotDmg);
}

std::string DamageWriter::calcNanobotDmgPercentageDealtOnPlayer(const Damage&d) {
    int totalDmg = d.getTotalDealtOnPlayer();
    int nanobotDmg = d.getNanobotTotalDealtOnPlayer();

    return percentage(totalDmg, nanobotDmg);
}

std::string DamageWriter::calcShieldDmgPercentageReceivedFromPlayer(const Damage&d) {
    int totalDmg = d.getTotalReceivedFromPlayer();
    int shieldDmg = d.getShieldTotalReceivedFromPlayer();

    return percentage(totalDmg, shieldDmg);
}

std::string DamageWriter::calcShieldDmgPercentageDealtOnPlayer(const Damage&d) {
    int totalDmg = d.getTotalDealtOnPlayer();
    int shieldDmg = d.getShieldTotalDealtOnPlayer();

    return percentage(totalDmg, shieldDmg);
}
