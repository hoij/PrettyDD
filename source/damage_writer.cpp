#include "damage.h"
#include "damage_writer.h"

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
    std::string titleBase = "Damage Dealt Top List";
    std::vector<std::pair<std::string, Damage>> totalDamageDealtPerPlayer =
        playerVector.getTotalDamageDealtPerPlayer();
    createTopList(titleBase, totalDamageDealtPerPlayer);
}

void DamageWriter::createDRTopList() {
    std::string titleBase = "Damage Received Top List";
    std::vector<std::pair<std::string, Damage>> totalDamageReceivedPerPlayer =
        playerVector.getTotalDamageReceivedPerPlayer();
    createTopList(titleBase, totalDamageReceivedPerPlayer);
}

void DamageWriter::createDDDetailedTopList() {
    std::string titleBase = "Detailed Damage Dealt Top List";
    std::vector<std::pair<std::string, Damage>> totalDamageDealtPerPlayer =
        playerVector.getTotalDamageDealtPerPlayer();
    createDetailedTopList(titleBase, totalDamageDealtPerPlayer);
}

void DamageWriter::createDRDetailedTopList() {
    std::string titleBase = "Detailed Damage Received Top List";
    std::vector<std::pair<std::string, Damage>> totalDamageReceivedPerPlayer =
        playerVector.getTotalDamageReceivedPerPlayer();
    createDetailedTopList(titleBase, totalDamageReceivedPerPlayer);
}

void DamageWriter::createDDPerType(std::string playerName) {
    std::string titleBase = "Damage Dealt Per Type By " + playerName;
    std::vector<std::pair<std::string, Damage>> damageDealtPerType =
        playerVector.getDamageDealtPerType(playerName);
    createDamagePerType(playerName,
                        titleBase,
                        damageDealtPerType,
                        8,
                        false);
}

void DamageWriter::createDRPerType(std::string playerName) {
    std::string titleBase = "Damage Received Per Type By " + playerName;
    std::vector<std::pair<std::string, Damage>> damageReceivedPerType =
        playerVector.getDamageDealtPerType(playerName);
    createDamagePerType(playerName,
                        titleBase,
                        damageReceivedPerType,
                        8,
                        false);
}

void DamageWriter::createDDPerTypeDetailed(std::string playerName) {
    std::string titleBase = "Detailed Damage Dealt Per Type By " + playerName;
    std::vector<std::pair<std::string, Damage>> damageDealtPerType =
        playerVector.getDamageDealtPerType(playerName);
    createDamagePerType(playerName,
                        titleBase,
                        damageDealtPerType,
                        2,
                        true);
}

void DamageWriter::createDRPerTypeDetailed(std::string playerName) {
    std::string titleBase = "Detailed Damage Received Per Type By " +
                            playerName;
    std::vector<std::pair<std::string, Damage>> damageReceivedPerType =
        playerVector.getDamageReceivedPerType(playerName);
    createDamagePerType(playerName,
                        titleBase,
                        damageReceivedPerType,
                        2,
                        true);
}

void DamageWriter::createDDPerOpponent(std::string playerName) {
    std::string titleBase = "Damage Dealt Per Opponent For " + playerName;
    std::vector<std::pair<std::string, Damage>> damageDealtPerOpponent =
        playerVector.getDamageDealtPerOpponent(playerName);
    createDamagePerOpponent(playerName,
                            titleBase,
                            damageDealtPerOpponent,
                            false);
}

void DamageWriter::createDRPerOpponent(std::string playerName) {
    std::string titleBase = "Damage Received Per Opponent For " + playerName;
    std::vector<std::pair<std::string, Damage>> damageReceivedPerOpponent =
        playerVector.getDamageReceivedPerOpponent(playerName);
    createDamagePerOpponent(playerName,
                            titleBase,
                            damageReceivedPerOpponent,
                            false);
}

void DamageWriter::createDDPerType(std::string playerName,
                                   std::string opponentName) {
    std::string titleBase = "Damage Dealt On " + opponentName + " By " +
                            playerName;
    createDamagePerType(titleBase, playerName, opponentName);
}

void DamageWriter::createDRPerType(std::string playerName,
    std::string opponentName) {
    std::string titleBase = "Damage Received by " + playerName + " From " +
                            opponentName;
    createDamagePerType(titleBase, opponentName, playerName);
}

void DamageWriter::createDamagePerType(std::string titleBase,
                                       std::string playerName,
                                       std::string opponentName) {

    std::vector<std::pair<std::string, Damage>> damageDealtPerType =
        playerVector.getDamageDealtPerType(playerName, opponentName);
    createDamagePerType(playerName,
                        opponentName,
                        titleBase,
                        damageDealtPerType,
                        7,
                        false);
}

void DamageWriter::createDDPerTypeDetailed(std::string playerName,
                                           std::string opponentName) {
    std::string titleBase = "Detailed Damage Dealt On " +
                            opponentName + " By " + playerName;
    createDamagePerTypeDetailed(titleBase, playerName, opponentName);
}

void DamageWriter::createDRPerTypeDetailed(std::string playerName,
                                           std::string opponentName) {
    std::string titleBase = "Detailed Damage Received By " +
                            playerName + " From " + opponentName;
    createDamagePerTypeDetailed(titleBase, opponentName, playerName);
}

void DamageWriter::createDamagePerTypeDetailed(
    std::string titleBase,
    std::string playerName,
    std::string opponentName) {

    std::vector<std::pair<std::string, Damage>> damageDealtPerType =
        playerVector.getDamageDealtPerType(playerName, opponentName);
    createDamagePerType(playerName,
                        opponentName,
                        titleBase,
                        damageDealtPerType,
                        2,
                        true);
}

/*****************/
/* Common writes */
/*****************/

void DamageWriter::createTopList(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer) {

    sortByTotal(totalDamagePerPlayer);

    // Calculate the number of text links (windows) needed to see all players
    const int playersPerWindow = 15;
    int nrOfPlayers = (int)totalDamagePerPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamagePerPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writeTopListHeadings,
                &DamageWriter::writeTopList);
}

void DamageWriter::createDetailedTopList(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>> totalDamagePerPlayer) {

    sortByTotal(totalDamagePerPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 7;
    int nrOfPlayers = (int)totalDamagePerPlayer.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamagePerPlayer,
                nrOfWindows,
                playersPerWindow,
                false,
                &DamageWriter::writePerPlayerHeadings,
                &DamageWriter::writePerPlayer);
}

void DamageWriter::createDamagePerType(
    std::string playerName,
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>> damagePerType,
    int typesPerWindow,
    bool detailed) {

    if (!playerFound(titleBase, playerName,damagePerType)) {
        return;
    }

    sortByTotal(damagePerType);

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    setPerTypeWriteMethods(whp, wdp, detailed);

    bool self = isSelf(playerName);

    // Calculate the number of links needed to write all players
    int nrOfTypes = (int)damagePerType.size();
    int nrOfWindows = calcNrOfWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                damagePerType,
                nrOfWindows,
                typesPerWindow,
                self,
                whp,
                wdp);
}

void DamageWriter::createDamagePerType(
    std::string playerName,
    std::string opponentName,
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>> damagePerType,
    int typesPerWindow,
    bool detailed) {

    if (!playerFound(titleBase, playerName, damagePerType)) {
        return;
    }

    if (!opponentFound(titleBase, playerName, opponentName, damagePerType)) {
        return;
    }

    sortByTotal(damagePerType);

    bool self = isSelf(playerName, opponentName);

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    setPerTypeWriteMethods(whp, wdp, detailed);

    // Calculate the number of files needed to write all players
    int nrOfTypes = (int)damagePerType.size();
    int nrOfWindows = calcNrOfWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                damagePerType,
                nrOfWindows,
                typesPerWindow,
                self,
                whp,
                wdp);
}

void DamageWriter::createDamagePerOpponent(
    std::string playerName,
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>> damagePerOpponent,
    bool detailed) {

    if (!playerFound(titleBase, playerName,damagePerOpponent)) {
        return;
    }

    sortByTotal(damagePerOpponent);

    bool self = isSelf(playerName);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 6;
    int nrOfPlayers = (int)damagePerOpponent.size();
    int nrOfWindows = calcNrOfWindows(nrOfPlayers, playersPerWindow);

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    setPerPlayerWriteMethods(whp, wdp, detailed);

    writeToFile(titleBase,
                damagePerOpponent,
                nrOfWindows,
                playersPerWindow,
                self,
                whp,
                wdp);
}

void DamageWriter::setPerPlayerWriteMethods(writeHeadingsPointer& whp,
                                              writeDamagePointer& wdp,
                                              bool detailed) {
    if (detailed) {
        whp = &DamageWriter::writePerTypeDetailedHeadings;
        wdp = &DamageWriter::writePerTypeDetailed;
    }
    else {
        whp = &DamageWriter::writePerPlayerHeadings;
        wdp = &DamageWriter::writePerPlayer;
    }
}

void DamageWriter::setPerTypeWriteMethods(writeHeadingsPointer& whp,
                                          writeDamagePointer& wdp,
                                          bool detailed) {
    if (detailed) {
        whp = &DamageWriter::writePerTypeDetailedHeadings;
        wdp = &DamageWriter::writePerTypeDetailed;
    }
    else {
        whp = &DamageWriter::writePerTypeHeadings;
        wdp = &DamageWriter::writePerType;
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

    /* The script file limit in AO is 4kb. So if the nrOfWindows is
    greater than 4 a new file will be created (as each window link
    is designed to (worst case) be as close to 1kb as possible). */

    int place = 1;
    const unsigned int windowsPerFile = 4;
    unsigned int nrOfFiles = calcNrOfFiles(nrOfWindows, windowsPerFile);
    for (unsigned int fileNr = 1; fileNr <= nrOfFiles; fileNr++) {

        std::string fileName = nameFile(fileNr);

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

        writeExecutionOfNextScript(fileNr, nrOfFiles);

        closeFile();
    }
}

std::string DamageWriter::nameFile(int fileNr) {
    std::string fileName = "pdd";
    if (fileNr > 1) {
        fileName += std::to_string(fileNr);
    }
    return fileName;
}

void DamageWriter::writeExecutionOfNextScript(int fileNr, int nrOfFiles) {
    if (fileNr < nrOfFiles) {
        file << "/delay 3000" << std::endl;
        file << "/pdd" << std::to_string(fileNr + 1) << std::endl;
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

    (this->*writeHeadingsPointer)(self);

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

void DamageWriter::writeTopList(const std::string& name,
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

void DamageWriter::writePerType(const std::string& name,
                                const Damage& d,
                                int place,
                                bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    // Don't write stats about the type "Regular Miss" for
    // players other than you.
    if (!self && name == "Regular Miss") {
        return;
    }

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

void DamageWriter::writePerPlayer(const std::string& name,
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
            std::string missPercentage = calcMissPercentage(d);
            file << std::setw(pcWidth) << " " + missPercentage << '%' << " ";
    }
    file << std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writePerTypeDetailed(const std::string& name,
                                        const Damage& d,
                                        int place,
                                        bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    // Don't write stats about the type "Regular Miss" for
    // players other than you.
    if (!self && name == "Regular Miss") {
        return;
    }

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

bool DamageWriter::playerFound(
    std::string titleBase,
    std::string playerName,
    const std::vector<std::pair<std::string, Damage>>& v) {
    /* PlayerVector returns an empty vector if the player can't
    be found. */
    if (v.size() == 0) {
        createNotFoundMessage("No data logged for " + playerName,
                              "Tried to display " + titleBase + " but " +
                              playerName + " was not found.");
        return false;
    }
    return true;
}

bool DamageWriter::opponentFound(
    std::string titleBase,
    std::string playerName,
    std::string opponentName,
    const std::vector<std::pair<std::string, Damage>>& v) {
    /* When the opponent is not found, a map with the key "empty"
    is returned from the Player that should contain the opponent.
    Player vector passes this "empty" vector on. */
    bool notFound = v.size() == 1 &&
                   (v[0].first == "empty");
    if (notFound) {
        createNotFoundMessage(opponentName + " not found among " +
                              playerName + "'s opponents",
                              "Tried to display " + titleBase + " but " +
                              opponentName + " was not found among " +
                              playerName + "'s opponents.");
        return false;
    }
    return true;
}

bool DamageWriter::isSelf(std::string playerName, std::string opponentName) {
    /* Check if player or opponent are you. */
    return playerName == "You" ||
           playerName == config.getPlayerRunningProgram() ||
           opponentName == "You" ||
           opponentName == config.getPlayerRunningProgram();
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
