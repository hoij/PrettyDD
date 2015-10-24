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
    sortByTotal(totalDamageDealtForEachPlayer);

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
    sortByTotal(totalDamageDealtForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 7;
    int nrOfPlayers = (int)totalDamageDealtForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageDealtForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writePerPlayerHeadings,
                &DamageWriter::writeDDPerPlayer);

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

void DamageWriter::createDDPerTypeOnSpecificOpponent(std::string playerName,
                                              std::string opponentName) {
    std::string titleBase = "DD On " + opponentName + " By " + playerName;
    createDamagePerTypeOnSpecificOpponent(playerName,
                                          opponentName,
                                          titleBase,
                                          7,
                                          false);
}

void DamageWriter::createDDPerTypeDetailedOnSpecificOpponent(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase = "Detailed DD Per Damage Type On " +
                            opponentName + " By " +
                            playerName;
    createDamagePerTypeOnSpecificOpponent(playerName,
                                   opponentName,
                                   titleBase,
                                   2,
                                   true);
}

void DamageWriter::createDamagePerTypeOnSpecificOpponent(
    std::string playerName,
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
            pp->getDamageDealtPerType(oppName);
    sortByTotal(allDamageTypesFromAffectedPlayer);

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
    sortByTotal(totalDamageReceivedForEachPlayer);

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
    sortByTotal(totalDamageReceivedForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 7;
    int nrOfPlayers = (int)totalDamageReceivedForEachPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageReceivedForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writePerPlayerHeadings,
                &DamageWriter::writeDRPerPlayer);
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
            pp->getTotalDamageDealtPerType();
        sortByTotal(allDamageTypesFromAffectedPlayer);
        setDDPerTypeWriteMethods(whp, wdp, detailed);
    }
    else { // Damage received
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageReceivedPerType();
        sortByTotal(allDamageTypesFromAffectedPlayer);
        whp = &DamageWriter::writePerTypeHeadings;
        wdp = &DamageWriter::writeDRPerType;
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
            pp->getTotalDamageDealtPerAffectedPlayer();
        sortByTotal(totalDamageForEachAffectedPlayer);
        setDDWriteMethods(whp, wdp, detailed);
    }
    else { // Damage received
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageReceivedPerAffectedPlayer();
        sortByTotal(totalDamageForEachAffectedPlayer);
        whp = &DamageWriter::writePerPlayerHeadings;
        wdp = &DamageWriter::writeDRPerPlayer;
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
        whp = &DamageWriter::writePerTypeDetailedHeadings;
        wdp = &DamageWriter::writeDDPerTypeDetailed;
    }
    else {
        whp = &DamageWriter::writePerPlayerHeadings;
        wdp = &DamageWriter::writeDDPerPlayer;
    }
}

void DamageWriter::setDDPerTypeWriteMethods(writeHeadingsPointer& whp,
                                            writeDamagePointer& wdp,
                                            bool detailed) {
    if (detailed) {
        whp = &DamageWriter::writePerTypeDetailedHeadings;
        wdp = &DamageWriter::writeDDPerTypeDetailed;
    }
    else {
        whp = &DamageWriter::writePerTypeHeadings;
        wdp = &DamageWriter::writeDDPerType;
    }
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

void DamageWriter::writePerPlayerHeadings(bool self) {
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

void DamageWriter::writePerTypeHeadings(bool self) {
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

void DamageWriter::writePerTypeDetailedHeadings(bool self) {
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
          std::setw(width) << " " + std::to_string(d.getTotal()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPM()) << " " <<
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
          std::setw(width) << " " + std::to_string(d.getTotal()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPM()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDPerType(const std::string& name,
                                          const Damage& d,
                                          int place,
                                          bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string deflectHitPercentage =
        calcDeflectHitPercentage(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotal())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCount())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPM())
                             << " " <<
            std::fixed << std::setprecision(1);
    if (d.hasSpecial()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
        if (self) {
            std::string missPercentage = calcMissPercentage(d);
            file << std::setw(pcWidth) << " " + missPercentage << '%' << " ";
        }
    }
    else if (d.hasShield() ||
             d.hasRegularMiss()) {
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
            calcRegularDmgPercentage(d);
        std::string nanobotDmgPercentage =
            calcNanobotDmgPercentage(d);
        std::string critHitPercentage =
            calcCritHitPercentage(d);
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

void DamageWriter::writeDRPerType(const std::string& name,
                                          const Damage& d,
                                          int place,
                                          bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string deflectHitPercentage =
        calcDeflectHitPercentage(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotal())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCount())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPM())
                             << " " <<
            std::fixed << std::setprecision(1);
    if (d.hasSpecial()) {
        file << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << na << "% "
             << std::setw(pcWidth) << " " + deflectHitPercentage << "% ";
        if (self) {
            std::string missPercentage = calcMissPercentage(d);
            file << std::setw(pcWidth) << " " + missPercentage << "% ";
        }
    }
    else if (d.hasShield() ||
             d.hasRegularMiss()) {
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
            calcRegularDmgPercentage(d);
        std::string nanobotDmgPercentage =
            calcNanobotDmgPercentage(d);
        std::string critHitPercentage =
            calcCritHitPercentage(d);
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

void DamageWriter::writeDDPerPlayer(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string regularDmgPercentage = calcRegularDmgPercentage(d);
    std::string specialDmgPercentage = calcSpecialDmgPercentage(d);
    std::string nanobotDmgPercentage = calcNanobotDmgPercentage(d);
    std::string shieldDmgPercentage = calcShieldDmgPercentage(d);

    std::string critHitPercentage = calcCritHitPercentage(d);
    std::string deflectHitPercentage = calcDeflectHitPercentage(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotal())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCount())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPM())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(pcWidth) << " " + regularDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + specialDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + nanobotDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + shieldDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + critHitPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
    if (self) {
        if (d.hasSpecial()) {
            std::string missPercentage = calcMissPercentage(d);
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

void DamageWriter::writeDRPerPlayer(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string regularDmgPercentage = calcRegularDmgPercentage(d);
    std::string specialDmgPercentage = calcSpecialDmgPercentage(d);
    std::string nanobotDmgPercentage = calcNanobotDmgPercentage(d);
    std::string shieldDmgPercentage = calcShieldDmgPercentage(d);

    std::string critHitPercentage = calcCritHitPercentage(d);
    std::string deflectHitPercentage = calcDeflectHitPercentage(d);

    const int width = 8;
    const int pcWidth = 6;
    const int nrWidth = 4;
    std::string na = " __._";
    file << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(d.getTotal())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCount())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPM())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(pcWidth) << " " + regularDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + specialDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + nanobotDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + shieldDmgPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + critHitPercentage << '%' << " " <<
            std::setw(pcWidth) << " " + deflectHitPercentage << '%' << " ";
    if (self) {
        if (d.hasSpecial()) {
            std::string missPercentage = calcMissPercentage(d);
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

void DamageWriter::writeDDPerTypeDetailed(const std::string& name,
                                           const Damage& d,
                                           int place,
                                           bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    file << "<font color = " + lightBlue + ">" << nl <<
            place << ". " << name << "</font><br>" << nl;

    if (d.hasRegular() &&
        d.hasNanobot()) {
        // Write info on the sum of the regular and nanobot damage only
        // if they both exist. No point writing this otherwise as it would be
        // identical to the total info written in respective branch.
        writeTotalInfo(d.getTotal(),
                       d.getCount(),
                       "");
        file << "<br>" << nl;
    }

    // If there is any regular damage
    if (d.hasRegular()) {
        writeDetailedRegularInfo(d);
    }

    // If there is any nanobot damage
    if (d.hasNanobot()) {
        writeDetailedNanobotInfo(d);
    }

    if (d.hasSpecial()) {
        writeDetailedSpecialInfo(d, self);
    }

    if (d.hasShield()) {
        writeDetailedShieldInfo(d);
    }

    if (d.hasRegularMiss()) {
        writeDetailedMissInfo(d, self);
    }

    file << "<br>" << nl << nl;
}

void DamageWriter::writeDetailedRegularInfo(const Damage& d) {

    int totalRegularDmg = d.getNormalTotal() +
                          d.getRegularDeflectTotal() +
                          d.getCritTotal();
    int totalRegularCount = d.getNormalCount() +
                            d.getRegularDeflectCount() +
                            d.getCritCount() +
                            d.getRegularMisses();
    writeTotalInfo(totalRegularDmg, totalRegularCount, "Regular");

    // Normal hits. Regular hits that are neither crits nor deflects.
    if (d.getNormalCount() > 0) {
        // Percentage of total hits for the type
        std::string normalHitPercentageOfType = percentage(
            d.getCount(),
            d.getNormalCount());
        // Percentage of regular hits
        std::string normalHitPercentageOfCategory = percentage(
            d.getNormalCount() +
            d.getCritCount() +
            d.getRegularDeflectCount() +
            d.getRegularMisses(),
            d.getNormalCount());
        // Percentage of total damage for the type
        std::string normalDmgPercentageOfType = percentage(
            d.getTotal(),
            d.getNormalTotal());
        std::string normalMin = determineMin(d.getNormalMin());
        std::string normalMax = determineMax(d.getNormalMax());
        writeDetailedInfoForType("Normal",
                                 d.getNormalTotal(),
                                 d.getNormalCount(),
                                 normalDmgPercentageOfType,
                                 normalMax,
                                 normalMin,
                                 normalHitPercentageOfType,
                                 normalHitPercentageOfCategory);
    }

    // Crit
    if (d.getCritCount() > 0) {
        // Percentage of total hits for the type
        std::string critHitPercentageOfType = percentage(
            d.getCount(),
            d.getCritCount());
        // Percentage of regular hits
        std::string critHitPercentageOfCategory =
            calcCritHitPercentage(d);
        // Percentage of total damage for the type
        std::string critDmgPercentageOfType = percentage(
            d.getTotal(),
            d.getCritTotal());
        std::string critMin = determineMin(d.getCritMin());
        std::string critMax = determineMax(d.getCritMax());
        writeDetailedInfoForType("Crit",
                                 d.getCritTotal(),
                                 d.getCritCount(),
                                 critDmgPercentageOfType,
                                 critMax,
                                 critMin,
                                 critHitPercentageOfType,
                                 critHitPercentageOfCategory);
    }

    // Deflect
    if (d.getRegularDeflectCount() > 0) {
        // Percentage of total hits for the type
        std::string deflectHitPercentageOfType = percentage(
            d.getCount(),
            d.getRegularDeflectCount());
        // Percentage of regular hits
        std::string deflectHitPercentageOfCategory =
            calcDeflectHitPercentage(d);
        // Percentage of total damage for the type
        std::string deflectDmgPercentageOfType = percentage(
            d.getTotal(),
            d.getRegularDeflectTotal());
        std::string regularDeflectMax = determineMax(
            d.getRegularDeflectMax());
        std::string regularDeflectMin = determineMin(
            d.getRegularDeflectMin());
        writeDetailedInfoForType("Deflect",
                                 d.getRegularDeflectTotal(),
                                 d.getRegularDeflectCount(),
                                 deflectDmgPercentageOfType,
                                 regularDeflectMax,
                                 regularDeflectMin,
                                 deflectHitPercentageOfType,
                                 deflectHitPercentageOfCategory);
    }

    if (d.hasNanobot()) {
        // Write an extra line to separate regular from nanobot.
        file << "<br>" << nl;
    }
}

void DamageWriter::writeDetailedNanobotInfo(const Damage& d) {

    // Percentage of total hits for the type/category (the same in this case)
    std::string nanobotHitPercentageOfType = percentage(
        d.getCount(),
        d.getNanobotCount());

    // Percentage of total damage for the type
    std::string nanobotDmgPercentageOfType = calcNanobotDmgPercentage(d);

    std::string nanobotMin = determineMin(d.getNanobotMin());
    std::string nanobotMax = determineMax(d.getNanobotMax());

    writeDetailedInfoForType("Nanobot",
                             d.getNanobotTotal(),
                             d.getNanobotCount(),
                             nanobotDmgPercentageOfType,
                             nanobotMax,
                             nanobotMin,
                             nanobotHitPercentageOfType);
}

void DamageWriter::writeDetailedSpecialInfo(const Damage& d, bool self) {

    // Only write the total info incase both normal and deflected
    // special damage exists. Otherwise the total displays duplicate info.
    if (d.hasSpecial() &&
        d.getSpecialDeflectTotal() > 0) {

        writeTotalInfo(d.getSpecialTotal() +
                       d.getSpecialDeflectTotal(),
                       d.getSpecialCount() +
                       d.getSpecialDeflectCount() +
                       d.getSpecialMisses(),
                       "");
    }

    if (d.getSpecialCount() > 0) {
        // Percentage of total hits for the type/category (the same in this case)
        std::string specialHitPercentageOfType = percentage(
            d.getCount(),
            d.getSpecialCount());
        // Percentage of total damage for the type
        std::string specialDmgPercentage = percentage(
            d.getTotal(),
            d.getSpecialTotal());
        // Min/Max
        std::string specialMin = determineMin(d.getSpecialMin());
        std::string specialMax = determineMax(d.getSpecialMax());
        writeDetailedInfoForType("Normal",
                                 d.getSpecialTotal(),
                                 d.getSpecialCount(),
                                 specialDmgPercentage,
                                 specialMax,
                                 specialMin,
                                 specialHitPercentageOfType);
    }

    if (d.getSpecialDeflectCount() > 0) {
        // Percentage of total hits for the type/category (the same in this case)
        std::string deflectHitPercentageOfType =
            calcSpecialDeflectHitPercentage(d);
        // Percentage of total damage for the category
        std::string deflectDmgPercentage = percentage(
            d.getTotal(),
            d.getSpecialDeflectTotal());

        std::string specialDeflectMax = determineMax(
            d.getSpecialDeflectMax());
        std::string specialDeflectMin = determineMin(
            d.getSpecialDeflectMin());
        writeDetailedInfoForType("Deflect",
                                 d.getSpecialDeflectTotal(),
                                 d.getSpecialDeflectCount(),
                                 deflectDmgPercentage,
                                 specialDeflectMax,
                                 specialDeflectMin,
                                 deflectHitPercentageOfType);
    }

    int misses = d.getSpecialMisses();
    if (self && misses > 0) {
        std::string missPercentage = calcSpecialMissPercentage(d);
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

    std::string shieldDmg = std::to_string(d.getShieldTotal());
    int shieldCount = d.getShieldCount();
    std::string minHit = determineMin(d.getShieldMin());
    std::string maxHit = determineMax(d.getShieldMax());
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
    int misses = d.getRegularMisses();
    if (self && misses > 0) {
        // Only including this for regular attacks
        std::string missPercentage = calcRegularMissPercentage(d);
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
        file << std::setw(pcWidth) << " " + hitPercentOfType << "% ";
    }
    if (!hitPercentOfCategory.empty()) {
        file << std::setw(pcWidth) << " " + hitPercentOfCategory << "% ";
    }
    file << type << "<br>" << nl;
}

/********************/
/* Helper functions */
/********************/

void DamageWriter::sortByTotal(std::vector<std::pair<std::string, Damage>>& v) {
    std::sort(v.begin(), v.end(),
              [](std::pair<std::string, Damage> damagePair1,
                 std::pair<std::string, Damage> damagePair2) {
                     return damagePair1.second.getTotal() >
                            damagePair2.second.getTotal();
                 });
}

std::string DamageWriter::calcCritHitPercentage(const Damage&d) {
    /* Based on attacks that have the potential to crit */
    int regularHits = d.getNormalCount() +
                      d.getCritCount() +
                      d.getRegularDeflectCount() +
                      d.getRegularMisses();
    int crits = d.getCritCount();

    return percentage(regularHits, crits);
}

std::string DamageWriter::calcDeflectHitPercentage(const Damage&d) {
    int regularHits = calcRegularAndSpecialHits(d);
    int deflects = d.getRegularDeflectCount() +
                   d.getSpecialDeflectCount();

    return percentage(regularHits, deflects);
}

std::string DamageWriter::calcSpecialDeflectHitPercentage(const Damage&d) {
    int specialHits = d.getSpecialCount() +
                      d.getSpecialDeflectCount() +
                      d.getSpecialMisses();
    int deflects = d.getSpecialDeflectCount();

    return percentage(specialHits, deflects);
}

std::string DamageWriter::calcMissPercentage(const Damage&d) {
    int regularHits = calcRegularAndSpecialHits(d);
    int misses = d.getRegularMisses() +
                 d.getSpecialMisses();

    return percentage(regularHits, misses);
}

std::string DamageWriter::calcRegularMissPercentage(const Damage&d) {
    int regularHits = d.getNormalCount() +
                      d.getCritCount() +
                      d.getRegularDeflectCount() +
                      d.getRegularMisses();
    int misses = d.getRegularMisses();

    return percentage(regularHits, misses);
}

std::string DamageWriter::calcSpecialMissPercentage(const Damage&d) {
    int regularHits = d.getSpecialCount() +
                      d.getSpecialDeflectCount() +
                      d.getSpecialMisses();
    int misses = d.getSpecialMisses();

    return percentage(regularHits, misses);
}

int DamageWriter::calcRegularAndSpecialHits(const Damage&d) {
    return d.getNormalCount() +
           d.getCritCount() +
           d.getRegularDeflectCount() +
           d.getSpecialCount() +
           d.getSpecialDeflectCount() +
           d.getRegularMisses() +
           d.getSpecialMisses();
}

std::string DamageWriter::calcRegularDmgPercentage(const Damage&d) {
    int totalDmg = d.getTotal();
    int regularDmg = d.getNormalTotal() +
                     d.getRegularDeflectTotal() +
                     d.getCritTotal();

    return percentage(totalDmg, regularDmg);
}

std::string DamageWriter::calcSpecialDmgPercentage(const Damage&d) {
    int totalDmg = d.getTotal();
    int specialDmg = d.getSpecialTotal() +
                     d.getSpecialDeflectTotal();

    return percentage(totalDmg, specialDmg);
}

std::string DamageWriter::calcNanobotDmgPercentage(const Damage&d) {
    int totalDmg = d.getTotal();
    int nanobotDmg = d.getNanobotTotal();

    return percentage(totalDmg, nanobotDmg);
}

std::string DamageWriter::calcShieldDmgPercentage(const Damage&d) {
    int totalDmg = d.getTotal();
    int shieldDmg = d.getShieldTotal();

    return percentage(totalDmg, shieldDmg);
}
