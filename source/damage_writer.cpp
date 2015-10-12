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

    if (!openFile()) {
        return;
    }

    std::string titleBase = "DD Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    // Calculate the number of text links (windows) needed to see all players
    const int playersPerWindow = 15;
    int nrOfPlayers = (int)totalDamageForEachPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                &DamageWriter::writeTopListHeadings,
                &DamageWriter::writeDDTopList);

    closeFile();
}

void DamageWriter::createDDDetailedTopList() {

    if (!openFile()) {
        return;
    }

    std::string titleBase = "DD Detailed Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByDealt(totalDamageForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 10;
    int nrOfPlayers = (int)totalDamageForEachPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                &DamageWriter::writeOverviewHeadingsOthers,
                &DamageWriter::writeDDOverviewOthers);

    closeFile();
}

void DamageWriter::createDDPerDamageType(std::string playerName) {
    std::string titleBase = "DD Per Damage Type By " + playerName;
    createDamagePerDamageType(playerName,
                              titleBase,
                              10,
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
                                   10,
                                   false);
}

void DamageWriter::createDDOnSpecificOpponentDetailed(
    std::string playerName,
    std::string opponentName) {

    std::string titleBase = "Detailed DD On " + opponentName + " By " +
                            playerName;
    createDamageOnSpecificOpponent(playerName,
                                   opponentName,
                                   titleBase,
                                   3,
                                   true);
}

void DamageWriter::createDamageOnSpecificOpponent(std::string playerName,
                                                  std::string opponentName,
                                                  std::string titleBase,
                                                  int typesPerWindow,
                                                  bool detailed) {

    if (!openFile()) {
        return;
    }

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.", file);
        closeFile();
        return;
    }

    // Get the data and sort it:
    std::vector<std::pair<std::string, Damage>>
        allDamageTypesFromAffectedPlayer =
            pp->getAllDamageDealtOnAffectedPlayer(opponentName);
    sortByDealt(allDamageTypesFromAffectedPlayer);

    // Check if the opponent was found.
    // When the player is not found, a map with the key "empty" is returned.
    bool notFound = allDamageTypesFromAffectedPlayer.size() == 1 &&
                    (allDamageTypesFromAffectedPlayer[0].first == "empty");
    if (notFound) {
        createNotFoundMessage(titleBase,
                              opponentName + " not found among " +
                              playerName + "'s opponents.",
                              file);
        closeFile();
        return;
    }

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    setDDWriteMethods(playerName, opponentName, whp, wdp, detailed);

    // Calculate the number of files needed to write all players
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                allDamageTypesFromAffectedPlayer,
                nrOfWindows,
                typesPerWindow,
                whp,
                wdp);

    closeFile();
}

/******/
/* DR */
/******/

void DamageWriter::createDRTopList() {

    if (!openFile()) {
        return;
    }

    std::string titleBase = "Damage Received Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByReceived(totalDamageForEachPlayer);

    // Calculate the number of text links (windows) needed to see all players
    const int playersPerWindow = 15;
    int nrOfPlayers = (int)totalDamageForEachPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfPlayers, playersPerWindow);

    writeToFile(
        titleBase,
        totalDamageForEachPlayer,
        nrOfWindows,
        playersPerWindow,
        &DamageWriter::writeOverviewHeadingsOthers,
        &DamageWriter::writeDRTopList);

    closeFile();
}

void DamageWriter::createDRDetailedTopList() {

    if (!openFile()) {
        return;
    }

    std::string titleBase = "Damage Received Detailed Top List";

    std::vector<std::pair<std::string, Damage>> totalDamageForEachPlayer =
        playerVector.getTotalDamageForEachPlayer();
    sortByReceived(totalDamageForEachPlayer);

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 8;
    int nrOfPlayers = (int)totalDamageForEachPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageForEachPlayer,
                nrOfWindows,
                playersPerWindow,
                &DamageWriter::writeOverviewHeadingsOthers,
                &DamageWriter::writeDROverviewOthers);

    closeFile();
}

void DamageWriter::createDRPerDamageType(std::string playerName) {
    std::string titleBase = "Damage Received Per Damage Type By " +
                            playerName;
    createDamagePerDamageType(playerName,
                              titleBase,
                              10,
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

    if (!openFile()) {
        return;
    }

    std::string pName = checkIfSelf(playerName);
    Player* pp = playerVector.getPlayer(pName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.", file);
        closeFile();
        return;
    }

    std::vector<std::pair<std::string, Damage>> allDamageTypesFromAffectedPlayer;
    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    if (dealt) { // Damage dealt
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeReceivedFromPlayer();
        sortByDealt(allDamageTypesFromAffectedPlayer);
        setDDWriteMethods(playerName, whp, wdp, detailed);
    }
    else { // Damage received
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeDealtOnPlayer();
        sortByReceived(allDamageTypesFromAffectedPlayer);
        setDRWriteMethods(playerName, whp, wdp);
    }

    // Calculate the number of links needed to write all players
    int nrOfTypes = (int)allDamageTypesFromAffectedPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfTypes, typesPerWindow);

    writeToFile(titleBase,
                allDamageTypesFromAffectedPlayer,
                nrOfWindows,
                typesPerWindow,
                whp,
                wdp);

    closeFile();
}

void DamageWriter::createDamagePerOpponent(std::string playerName,
                                           std::string titleBase,
                                           bool dealt,
                                           bool detailed) {
    /* Writes damage received or damage dealt depending on the bool "dealt".
    */
    if (!openFile()) {
        return;
    }

    std::string pName = checkIfSelf(playerName);
    Player* pp = playerVector.getPlayer(pName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.", file);
        closeFile();
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
        setDDWriteMethods(playerName, whp, wdp, detailed);
    }
    else { // Damage received
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageDealtOnPlayerForAllAffectedPlayers();
        sortByReceived(totalDamageForEachAffectedPlayer);
        setDRWriteMethods(playerName, whp, wdp);
    }

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 8;
    int nrOfPlayers = (int)totalDamageForEachAffectedPlayer.size();
    int nrOfWindows = calcNrOFWindows(nrOfPlayers, playersPerWindow);

    writeToFile(titleBase,
                totalDamageForEachAffectedPlayer,
                nrOfWindows,
                playersPerWindow,
                whp,
                wdp);

    closeFile();
}

void DamageWriter::setDDWriteMethods(std::string playerName,
                                     std::string opponentName,
                                     writeHeadingsPointer& whp,
                                     writeDamagePointer& wdp,
                                     bool detailed) {

    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram() ||
        opponentName == "You" ||
        opponentName == config.getplayerRunningProgram()) {
        if (detailed) {
            whp = &DamageWriter::writeOverviewHeadingsDetailed;
            wdp = &DamageWriter::writeDDOverviewDetailedSelf;
        }
        else {
            whp = &DamageWriter::writeOverviewHeadingsSelf;
            wdp = &DamageWriter::writeDDOverviewSelf;
        }
    }
    else {
        if (detailed) {
            whp = &DamageWriter::writeOverviewHeadingsDetailed;
            wdp = &DamageWriter::writeDDOverviewDetailedOthers;
        }
        else {
            whp = &DamageWriter::writeOverviewHeadingsOthers;
            wdp = &DamageWriter::writeDDOverviewOthers;
        }
    }
}

void DamageWriter::setDDWriteMethods(std::string playerName,
                                     writeHeadingsPointer& whp,
                                     writeDamagePointer& wdp,
                                     bool detailed) {
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        if (detailed) {
            whp = &DamageWriter::writeOverviewHeadingsDetailed;
            wdp = &DamageWriter::writeDDOverviewDetailedSelf;
        }
        else {
            whp = &DamageWriter::writeOverviewHeadingsSelf;
            wdp = &DamageWriter::writeDDOverviewSelf;
        }
    }
    else {
        if (detailed) {
            whp = &DamageWriter::writeOverviewHeadingsDetailed;
            wdp = &DamageWriter::writeDDOverviewDetailedOthers;
        }
        else {
            whp = &DamageWriter::writeOverviewHeadingsOthers;
            wdp = &DamageWriter::writeDDOverviewOthers;
        }
    }
}

void DamageWriter::setDRWriteMethods(std::string playerName,
                                     writeHeadingsPointer& whp,
                                     writeDamagePointer& wdp) {
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        whp = &DamageWriter::writeOverviewHeadingsSelf;
        wdp = &DamageWriter::writeDROverviewSelf;
    }
    else {
        whp = &DamageWriter::writeOverviewHeadingsOthers;
        wdp = &DamageWriter::writeDROverviewOthers;
    }
}

void DamageWriter::writeToFile(
    std::string titleBase,
    std::vector<std::pair<std::string, Damage>>& v,
    unsigned int nrOfWindows,
    int typesPerWindow,
    void (DamageWriter::*writeHeadingsPointer)(),
    void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                             const Damage& d,
                                             int place)) {

    /* Sets the link name number and calls the write function for
    each link needed. */
    int place = 1;
    for (unsigned int windowNr = 0; windowNr != nrOfWindows; windowNr++) {
        std::string title =
            appendInterval(titleBase, windowNr, typesPerWindow);

        auto start = v.begin() + windowNr * typesPerWindow;
        auto stop = v.begin();  // Just to set the right type on stop.
        // Stop at either the end or the nr of types per file.
        if (v.size() < (windowNr + 1) * typesPerWindow) {
            stop = v.end();
        }
        else {
            stop = v.begin() + (windowNr + 1) * typesPerWindow;
        }

        writeStats(start,
                   stop,
                   title,
                   place,
                   writeHeadingsPointer,
                   writeDamagePointer);
    }
}

void DamageWriter::writeStats(
    std::vector<std::pair<std::string, Damage>>::iterator start,
    std::vector<std::pair<std::string, Damage>>::iterator stop,
    std::string title,
    int& place,
    void (DamageWriter::*writeHeadingsPointer)(),
    void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                             const Damage& d,
                                             int place)) {

    /* Writes headings and then the DD, place and name for each Damage
    in the vector.  */

    writeStartOfLink(title);

    if (writeHeadingsPointer != nullptr) {
        (this->*writeHeadingsPointer)();
    }

    file << "<font color = " + lime + ">" << nl;
    for (auto it = start; it != stop; it++) {
        (this->*writeDamagePointer)(it->first, it->second, place++);
    }
    file << "</font>";

    writeEndOfLink(title);
}

/************/
/* Headings */
/************/

void DamageWriter::writeTopListHeadings() {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width + 2) << " Total " <<
            std::setw(width) << " DPM " <<
            "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writeOverviewHeadingsOthers() {
    /* Headings without misses as they are not logged by AO for players
    other than yourself. (Unless they are hitting you) */
    writeOverviewHeadings(false);
}

void DamageWriter::writeOverviewHeadingsSelf() {
    /* As the other detailed overview heading but with info on misses. */
    writeOverviewHeadings(true);
}

void DamageWriter::writeOverviewHeadings(bool self) {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width+1) << " Total " <<
            std::setw(6) << "(Cnt) " <<
            std::setw(width-1) << " DPM " <<
            std::setw(width+1) << " Crit " <<
            std::setw(width+1) << " Nanobot ";
    if (self) {
        file << std::setw(width) << " Miss ";
    }
    file << std::setw(width+2) << " Deflect " <<
            "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writeOverviewHeadingsDetailed() {
    const int width = 7;
    const int pcWidth = 7;
    const int nrWidth = 3;
    file << "<font color = " + lightBlue + ">" << nl;
    file << std::setfill(fillChar) << std::right <<
            std::setw(width+1) << "hit%" << " (" <<
            std::setw(nrWidth) << "cnt" << ") " <<
            std::setw(width) << " Max" << "-" << std::left <<
            std::setw(width) << "Min " << std::right <<
            std::setw(pcWidth) << "dmg" << "%<br><br>" <<
            "</font>" << nl;
}

void DamageWriter::writePerDamageTypeHeadings() {

    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width + 1) << " Total " <<
          std::setw(width - 1) << " DPM " <<
          std::setw(width + 1) << " Crit " <<
          std::setw(width + 1) << " Nanobot " <<
          std::setw(width) << " Miss " <<
          std::setw(width + 2) << " Deflect " <<
          "</font><br>" << std::setfill(' ') << nl;
}

void DamageWriter::writeDDHeadings() {
    int width = 9;
    file << std::right << std::setfill(fillChar) <<
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
            std::setfill(' ') << nl;
}

/******************/
/* DD Stat writes */
/******************/

void DamageWriter::writeDDTopList(const std::string& name,
                                  const Damage& d,
                                  int place) {
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
                                  int place) {
    const int width = 9;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalDealtOnPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDOverviewOthers(const std::string& name,
                                         const Damage& d,
                                         int place) {
    writeDDOverview(name, d, place, false);
}

void DamageWriter::writeDDOverviewSelf(const std::string& name,
                                       const Damage& d,
                                       int place) {
    writeDDOverview(name, d, place, true);
}

void DamageWriter::writeDDOverview(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string critPercentage = percentage(d.getCountReceivedFromPlayer(),
                                     d.getCritCountReceivedFromPlayer());
    std::string nanobotPercentage = percentage(
                                        d.getTotalReceivedFromPlayer(),
                                        d.getNanobotTotalReceivedFromPlayer());
    std::string missPercentage;
    if (self) {
        missPercentage = percentage(d.getCountReceivedFromPlayer(),
                                    d.getMissesReceivedFromPlayer());
    }
    std::string deflectPercentage = percentage(
        d.getCountReceivedFromPlayer(),
        d.getRegularDeflectCountReceivedFromPlayer() +
        d.getNanobotDeflectCountReceivedFromPlayer());

    const int width = 8;
    const int nrWidth = 3;
    file << std::setfill(fillChar) <<
            std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountReceivedFromPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(width - 1) << " " + critPercentage << '%' << " " <<
            std::setw(width - 1) << " " + nanobotPercentage << '%' << " ";
    if (self) {
        file << std::setw(width - 1) << " " + missPercentage << '%' << " ";
    }
    file << std::setw(width - 1) << " " + deflectPercentage << '%' << "  " <<
            std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
}

void DamageWriter::writeDDOverviewDetailedOthers(const std::string& name,
                                                 const Damage& d,
                                                 int place) {
    writeDDOverviewDetailed(name, d, place, false);
}

void DamageWriter::writeDDOverviewDetailedSelf(const std::string& name,
                                               const Damage& d,
                                               int place) {
    writeDDOverviewDetailed(name, d, place, true);
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

    bool hasRegularDmg = d.getRegularCountReceivedFromPlayer() ||
                         d.getRegularDeflectCountReceivedFromPlayer() ||
                         d.getCritCountReceivedFromPlayer();
    bool hasNanobotDmg = d.getNanobotCountReceivedFromPlayer() ||
                         d.getNanobotDeflectCountReceivedFromPlayer();

    if (hasRegularDmg && hasNanobotDmg) {
        // Write info on the sum of the regular and nanobot damage only
        // if they both exist. No point writing this otherwise as it would be
        // identical to the total info written in respective branch.
        writeTotalInfo(d.getTotalReceivedFromPlayer(),
                       d.getCountReceivedFromPlayer());
        }

    // If there is any regular damage
    if (hasRegularDmg) {
        writeDetailedRegularInfo(d, self);
    }

    // If there is any nanobot damage
    if (hasNanobotDmg) {
        writeDetailedNanobotInfo(d);
    }

    file << "<br>" << nl << nl;
}

void DamageWriter::writeDetailedRegularInfo(const Damage& d, bool self) {


    if (!d.isSpecial()) {  // Specials can only have regular damage
        file << "<font color = " + lightBlue + ">" << nl <<
            "Regular " << "</font><br>" << nl;
    }

    // Percentage of total hits
    std::string regularHitPercentage = percentage(
        d.getCountReceivedFromPlayer(),
        d.getRegularCountReceivedFromPlayer());

    std::string critHitPercentage = percentage(
        d.getRegularCountReceivedFromPlayer() +
        d.getRegularDeflectCountReceivedFromPlayer(),
        d.getCritCountReceivedFromPlayer());

    std::string regularDeflectHitPercentage = percentage(
        d.getCountReceivedFromPlayer(),
        d.getRegularDeflectCountReceivedFromPlayer());

    std::string missPercentage;
    if (self) {
        missPercentage = percentage(d.getCountReceivedFromPlayer(),
                                    d.getMissesReceivedFromPlayer());
    }

    // Percentage of total damage
    std::string regularDmgPercentage = percentage(
        d.getTotalReceivedFromPlayer(),
        d.getRegularTotalReceivedFromPlayer());

    std::string critDmgPercentage = percentage(
        d.getTotalReceivedFromPlayer(),
        d.getCritTotalReceivedFromPlayer());

    std::string regularDeflectDmgPercentage = percentage(
        d.getTotalReceivedFromPlayer(),
        d.getRegularDeflectTotalReceivedFromPlayer());

    // Min/Max
    std::string regularMin = determineMin(d.getRegularMinReceivedFromPlayer());
    std::string regularMax = determineMax(d.getRegularMaxReceivedFromPlayer());
    std::string critMin = determineMin(d.getCritMinReceivedFromPlayer());
    std::string critMax = determineMax(d.getCritMaxReceivedFromPlayer());
    std::string regularDeflectMax = determineMax(
        d.getRegularDeflectMaxReceivedFromPlayer());
    std::string regularDeflectMin = determineMin(
        d.getRegularDeflectMinReceivedFromPlayer());

    // Write it to file
    writeTotalInfo(d.getRegularTotalReceivedFromPlayer() +
                   d.getRegularDeflectTotalReceivedFromPlayer() +
                   d.getCritTotalReceivedFromPlayer(),
                   d.getRegularCountReceivedFromPlayer() +
                   d.getRegularDeflectCountReceivedFromPlayer() +
                   d.getCritCountReceivedFromPlayer());

    writeDetailedInfoForType("Regular",
                             regularHitPercentage,
                             std::to_string(d.getRegularCountReceivedFromPlayer()),
                             regularMax,
                             regularMin,
                             regularDmgPercentage);

    if (!d.isSpecial()) {
        writeDetailedInfoForType("Crit",
                                 critHitPercentage,
                                 std::to_string(d.getCritCountReceivedFromPlayer()),
                                 critMax,
                                 critMin,
                                 critDmgPercentage);
    }

    writeDetailedInfoForType("Deflect",
                             regularDeflectHitPercentage,
                             std::to_string(d.getRegularDeflectCountReceivedFromPlayer()),
                             regularDeflectMax,
                             regularDeflectMin,
                             regularDeflectDmgPercentage);

    if (self && d.getMissesReceivedFromPlayer()) {
        // Assuming nanobot attacks can't miss.
        // Only including it for regular attacks
        const int pcWidth = 7;
        const int nrWidth = 3;
        file << std::right <<
                std::setw(pcWidth) << " " + missPercentage << "% (" <<
                std::setw(nrWidth) << std::to_string(d.getMissesReceivedFromPlayer())
                                   << ") " << "Misses<br>" << nl;
    }
}

void DamageWriter::writeDetailedNanobotInfo(const Damage& d) {
    file << "<font color = " + lightBlue + ">" << nl <<
            "Nanobot " << "</font><br>" << nl;

    // Percentage of total hits
    std::string nanobotHitPercentage = percentage(
        d.getCountReceivedFromPlayer(),
        d.getNanobotCountReceivedFromPlayer());

    std::string nanobotDeflectHitPercentage = percentage(
        d.getCountReceivedFromPlayer(),
        d.getNanobotDeflectCountReceivedFromPlayer());

    // Percentage of total damage
    std::string nanobotDmgPercentage = percentage(
        d.getTotalReceivedFromPlayer(),
        d.getNanobotTotalReceivedFromPlayer());

    std::string nanobotDeflectDmgPercentage = percentage(
        d.getTotalReceivedFromPlayer(),
        d.getNanobotDeflectTotalReceivedFromPlayer());

    // Min/Max
    std::string nanobotMin = determineMin(d.getNanobotMinReceivedFromPlayer());
    std::string nanobotMax = determineMax(d.getNanobotMaxReceivedFromPlayer());
    std::string nanobotDeflectMax = determineMax(
        d.getNanobotDeflectMaxReceivedFromPlayer());
    std::string nanobotDeflectMin = determineMin(
        d.getNanobotDeflectMinReceivedFromPlayer());

    // Write it to file
    writeTotalInfo(d.getNanobotTotalReceivedFromPlayer() +
                   d.getNanobotDeflectTotalReceivedFromPlayer(),
                   d.getNanobotCountReceivedFromPlayer() +
                   d.getNanobotDeflectCountReceivedFromPlayer());

    writeDetailedInfoForType("Nanobot",
                             nanobotHitPercentage,
                             std::to_string(d.getNanobotCountReceivedFromPlayer()),
                             nanobotMax,
                             nanobotMin,
                             nanobotDmgPercentage);

    if (d.getNanobotDeflectCountReceivedFromPlayer()) {
        // Nanobot attacks can probably not deflect.
        // TODO: Remove this if it turns out to be true.
        writeDetailedInfoForType("Deflect",
                                 nanobotDeflectHitPercentage,
                                 std::to_string(d.getNanobotDeflectCountReceivedFromPlayer()),
                                 nanobotDeflectMax,
                                 nanobotDeflectMin,
                                 nanobotDeflectDmgPercentage);
    }
}

void DamageWriter::writeTotalInfo(int total, int cnt) {
    const int width = 7;
    const int nrWidth = 3;
    file << std::right << std::setfill(fillChar) <<
            std::setw(width+1) << " " + std::to_string(total)
                               << " (" <<
            std::setw(nrWidth) << std::to_string(cnt)
                               << ") " << "Total " << "<br>" << nl;
}

void DamageWriter::writeDetailedInfoForType(std::string type,
                                            std::string hitPercent,
                                            std::string nrOfHits,
                                            std::string maxHit,
                                            std::string minHit,
                                            std::string dmgPercent) {
    const int width = 7;
    const int pcWidth = 7;
    const int nrWidth = 3;
    file << std::setw(pcWidth) << " " + hitPercent << "% (" <<
            std::setw(nrWidth) << nrOfHits << ") " <<
            std::setw(width) << " " + maxHit << "-" << std::left <<
            std::setw(width) << minHit + " " << std::right <<
            std::setw(pcWidth) << dmgPercent << "% " << type << "<br>" << nl;
}

/******************/
/* DR Stat writes */
/******************/

void DamageWriter::writeDROverviewOthers(const std::string& name,
                                         const Damage& d,
                                         int place) {
    writeDROverview(name, d, place, false);
}

void DamageWriter::writeDROverviewSelf(const std::string& name,
                                       const Damage& d,
                                       int place) {
    writeDROverview(name, d, place, true);
}

void DamageWriter::writeDROverview(const std::string& name,
                                   const Damage& d,
                                   int place,
                                   bool self) {
    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string critPercentage = percentage(d.getCountDealtOnPlayer(),
                                         d.getCritCountDealtOnPlayer());
    std::string nanobotPercentage = percentage(
                                        d.getTotalDealtOnPlayer(),
                                        d.getNanobotTotalDealtOnPlayer());
    std::string missPercentage;
    if (self) {
        missPercentage = percentage(d.getCountDealtOnPlayer(),
                                    d.getMissesDealtOnPlayer());
    }
    std::string deflectPercentage = percentage(
        d.getCountDealtOnPlayer(),
        d.getRegularDeflectCountDealtOnPlayer() +
        d.getNanobotDeflectCountDealtOnPlayer());

    const int width = 8;
    const int nrWidth = 3;
    file << std::setfill(fillChar) <<
            std::setw(width) << " " + std::to_string(d.getTotalDealtOnPlayer())
                             << " (" <<
            std::setw(nrWidth) << std::to_string(d.getCountDealtOnPlayer())
                             << ") " <<
            std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer())
                             << " " <<
            std::fixed << std::setprecision(1) <<
            std::setw(width - 1) << " " + critPercentage << '%' << " " <<
            std::setw(width - 1) << " " + nanobotPercentage << '%' << " ";
    if (self) {
        file << std::setw(width - 1) << " " + missPercentage << '%' << " ";
    }
    file << std::setw(width - 1) << " " + deflectPercentage << '%' << "  " <<
            std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>" << nl;
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

std::string DamageWriter::determineMin(int minDmg) {
    return std::to_string((minDmg == std::numeric_limits<int>::max()) ?
                           0 : minDmg);
}

std::string DamageWriter::determineMax(int maxDmg) {
    return std::to_string((maxDmg == -1) ? 0 : maxDmg);
}


