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
    int nrOfPlayers = (int)playerVector.size();
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
    int nrOfPlayers = (int)playerVector.size();
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
                              true);
}

void DamageWriter::createDDPerOpponent(std::string playerName) {
    std::string titleBase = "DD Per Opponent For " + playerName;
    createDamagePerOpponent(playerName, titleBase, true);
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
                                                  unsigned int typesPerWindow,
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
    int nrOfPlayers = (int)playerVector.size();
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
    const int playersPerWindow = 10;
    int nrOfPlayers = (int)playerVector.size();
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
                              false);
}

void DamageWriter::createDRPerOpponent(std::string playerName) {
    std::string titleBase = "Damage Received Per Opponent For " + playerName;
    createDamagePerOpponent(playerName, titleBase, false);
}

/*****************/
/* Common writes */
/*****************/


void DamageWriter::createDamagePerDamageType(std::string playerName,
                                             std::string titleBase,
                                             bool dealt) {

    if (!openFile()) {
        return;
    }

    Player* pp = playerVector.getPlayer(playerName);
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
        setDDWriteMethods(playerName, whp, wdp);
    }
    else { // Damage received
        allDamageTypesFromAffectedPlayer =
            pp->getTotalDamageForEveryDamageTypeDealtOnPlayer();
        sortByReceived(allDamageTypesFromAffectedPlayer);
        setDRWriteMethods(playerName, whp, wdp);
    }

    // Calculate the number of links needed to write all players
    const int typesPerWindow = 10;
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
                                           bool dealt) {
    /* Writes damage received or damage dealt depending on the bool "dealt".
    */
    if (!openFile()) {
        return;
    }

    Player* pp = playerVector.getPlayer(playerName);
    if (pp == nullptr) {
        createNotFoundMessage(titleBase, playerName + " not found.", file);
        closeFile();
        return;
    }

    std::vector<std::pair<std::string, Damage>>
        totalDamageForEachAffectedPlayer =
            pp->getTotalDamageForAllAffectedPlayers();

    writeDamagePointer wdp;
    writeHeadingsPointer whp;
    if (dealt) { // Damage dealt
        sortByDealt(totalDamageForEachAffectedPlayer);
        setDDWriteMethods(playerName, whp, wdp);
    }
    else { // Damage received
        sortByReceived(totalDamageForEachAffectedPlayer);
        setDRWriteMethods(playerName, whp, wdp);
    }

    // Calculate the number of files needed to write all players
    const int playersPerWindow = 10;
    int nrOfPlayers = (int)pp->nrOfAffectedPlayers();
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
            whp = nullptr;
            wdp = &DamageWriter::writeDDOverviewDetailedSelf;
        }
        else {
            whp = &DamageWriter::writeOverviewHeadingsSelf;
            wdp = &DamageWriter::writeDDOverviewSelf;
        }
    }
    else {
        if (detailed) {
            whp = nullptr;
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
                                     writeDamagePointer& wdp) {
    if (playerName == "You" ||
        playerName == config.getplayerRunningProgram()) {
        whp = &DamageWriter::writeOverviewHeadingsSelf;
        wdp = &DamageWriter::writeDDOverviewSelf;
    }
    else {
        whp = &DamageWriter::writeOverviewHeadingsOthers;
        wdp = &DamageWriter::writeDDOverviewOthers;
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
                                             unsigned int place)) {

    /* Sets the link name number and calls the write function for
    each link needed. */
    unsigned int place = 1;
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
    unsigned int& place,
    void (DamageWriter::*writeHeadingsPointer)(),
    void (DamageWriter::*writeDamagePointer)(const std::string& name,
                                             const Damage& d,
                                             unsigned int place)) {

    /* Writes headings and then the DD, place and name for each Damage
    in the vector.  */

    writeStartOfLink(title);

    if (writeHeadingsPointer != nullptr) {
        (this->*writeHeadingsPointer)();
    }

    file << "<font color = " + lime + ">";
    writeNewlineIfReadableFlagSet();
    for (auto it = start; it != stop; it++) {
        (this->*writeDamagePointer)(it->first, it->second, place++);
    }

    writeEndOfLink(title);
}

/************/
/* Headings */
/************/

void DamageWriter::writeTopListHeadings() {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">";
    writeNewlineIfReadableFlagSet();

    file << std::setw(width + 2) << " Total " <<
            std::setw(width) << " DPM " <<
            "</font><br>" << std::setfill(' ');
    writeNewlineIfReadableFlagSet();
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
          "<font color = " + lightBlue + ">";
    writeNewlineIfReadableFlagSet();

    file << std::setw(width+1) << " Total " <<
          std::setw(width-1) << " DPM " <<
          std::setw(width+1) << " Crit " <<
          std::setw(width+1) << " Nanobot ";
    if (self) {
        file << std::setw(width) << " Miss ";
    }
    file << std::setw(width+2) << " Deflect " <<
          "</font><br>" << std::setfill(' ');
    writeNewlineIfReadableFlagSet();
}

void DamageWriter::writeOverviewHeadingsDetailedOthers() {
    /* Headings without misses as they are not logged by AO for players
    other than yourself. (Unless they are hitting you) */
    writeOverviewHeadingsDetailed(false);
}

void DamageWriter::writeOverviewHeadingsDetailedSelf() {
    /* As the other detailed overview heading but with info on misses. */
    writeOverviewHeadingsDetailed(true);
}

void DamageWriter::writeOverviewHeadingsDetailed(bool self) {

    // TODO: FIX

    int width = 9;
    file << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    writeNewlineIfReadableFlagSet();

    file << std::setw(width+1) << " Total " <<
          std::setw(width-1) << " DPM " <<
          std::setw(width+1) << " Crit " <<
          std::setw(width+1) << " Nanobot ";
    if (self) {
        file << std::setw(width) << " Miss ";
    }
    file << std::setw(width+2) << " Deflect " <<
          "</font><br>" << std::setfill(' ');
    writeNewlineIfReadableFlagSet();
}

void DamageWriter::writePerDamageTypeHeadings() {

    int width = 9;
    file << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    writeNewlineIfReadableFlagSet();

    file << std::setw(width + 1) << " Total " <<
          std::setw(width - 1) << " DPM " <<
          std::setw(width + 1) << " Crit " <<
          std::setw(width + 1) << " Nanobot " <<
          std::setw(width) << " Miss " <<
          std::setw(width + 2) << " Deflect " <<
          "</font><br>" << std::setfill(' ');
    writeNewlineIfReadableFlagSet();
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
            std::setfill(' ');
    writeNewlineIfReadableFlagSet();
}

/******************/
/* DD Stat writes */
/******************/

void DamageWriter::writeDDTopList(const std::string& name,
                                  const Damage& d,
                                  unsigned int place) {
    const int width = 9;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>";
    writeNewlineIfReadableFlagSet();
}

void DamageWriter::writeDRTopList(const std::string& name,
                                  const Damage& d,
                                  unsigned int place) {
    const int width = 9;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalDealtOnPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer()) << " " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>";
    writeNewlineIfReadableFlagSet();
}

void DamageWriter::writeDDOverviewOthers(const std::string& name,
                                         const Damage& d,
                                         unsigned int place) {
    writeDDOverview(name, d, place, false);
}

void DamageWriter::writeDDOverviewSelf(const std::string& name,
                                       const Damage& d,
                                       unsigned int place) {
    writeDDOverview(name, d, place, true);
}

void DamageWriter::writeDDOverview(const std::string& name,
                                   const Damage& d,
                                   unsigned int place,
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
    std::string deflectPercentage = percentage(d.getCountReceivedFromPlayer(),
                                               d.getDeflectsReceivedFromPlayer());

    const int width = 8;
    const int critOffset = 1;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMReceivedFromPlayer()) << " " <<
          std::fixed << std::setprecision(1) <<
          std::setw(width - critOffset) << " " + critPercentage << '%' << " " <<
          std::setw(width - critOffset) << " " + nanobotPercentage << '%' << " ";
    if (self) {
        file << std::setw(width - critOffset) << " " + missPercentage << '%' << " ";
    }
    file << std::setw(width - critOffset) << " " + deflectPercentage << '%' << "  " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>";
    writeNewlineIfReadableFlagSet();
}

void DamageWriter::writeDDOverviewDetailedOthers(const std::string& name,
                                                 const Damage& d,
                                                 unsigned int place) {
    writeDDOverviewDetailed(name, d, place, false);
}

void DamageWriter::writeDDOverviewDetailedSelf(const std::string& name,
                                               const Damage& d,
                                               unsigned int place) {
    writeDDOverviewDetailed(name, d, place, true);
}

void DamageWriter::writeDDOverviewDetailed(const std::string& name,
                                           const Damage& d,
                                           unsigned int place,
                                           bool self) {

    // TODO: FIX

    /* If the bool self is set then the details will contain misses.
    Misses are not not logged by AO for players other than yourself.
    (Unless they are hitting you) */

    std::string regularPercentage = percentage(d.getCountReceivedFromPlayer(),
                                        d.getRegularCountReceivedFromPlayer());
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
    std::string deflectPercentage = percentage(d.getCountReceivedFromPlayer(),
                                        d.getDeflectsReceivedFromPlayer());

    std::string regularMin = std::to_string((d.getRegularMinReceivedFromPlayer() ==
                                             std::numeric_limits<int>::max()) ?
                                                 0 : d.getRegularMinReceivedFromPlayer());
    std::string regularMax = std::to_string((d.getRegularMaxReceivedFromPlayer() == -1) ?
                                                 0 : d.getRegularMaxReceivedFromPlayer());
    std::string critMin = std::to_string((d.getCritMinReceivedFromPlayer() ==
                                          std::numeric_limits<int>::max()) ?
                                              0 : d.getCritMinReceivedFromPlayer());
    std::string critMax = std::to_string((d.getCritMaxReceivedFromPlayer() == -1) ?
                                              0 : d.getCritMaxReceivedFromPlayer());
    std::string nanobotMin = std::to_string((d.getNanobotMinReceivedFromPlayer() ==
                                             std::numeric_limits<int>::max()) ?
                                                 0 : d.getNanobotMinReceivedFromPlayer());
    std::string nanobotMax = std::to_string((d.getNanobotMaxReceivedFromPlayer() == -1) ?
                                              0 : d.getNanobotMaxReceivedFromPlayer());
    const int width = 8;
    const int pcWidth = 7;
    const int nrWidth = 3;
    //writePlace(place);
    (void)place;
    //writeName(name);
    //file << "<br>";

    file << name << "<br>";
    writeNewlineIfReadableFlagSet();
    file << std::setfill(fillChar) << std::right <<
            std::setw(width) << " " + std::to_string(d.getTotalReceivedFromPlayer())
                               << " (" <<
            std::setw(nrWidth) << " " + std::to_string(d.getCountReceivedFromPlayer())
                               << ") " << "<br>";
    writeNewlineIfReadableFlagSet();

    file << std::setw(pcWidth) << " " + regularPercentage << "% (" <<
            std::setw(nrWidth) << " " + std::to_string(d.getCritCountReceivedFromPlayer())
                               << ") " <<
            std::setw(width) << " " + regularMin << " - " <<
            std::left <<
            std::setw(width) << regularMax + " " << " Regular<br>";
    writeNewlineIfReadableFlagSet();

    file << std::setw(pcWidth) << std::right << " " + critPercentage << "% (" <<
            std::setw(nrWidth) << " " + std::to_string(d.getCritCountReceivedFromPlayer())
                               << ") " <<
            std::setw(width) << " " + critMin << " - " <<
            std::setw(width) << std::left << critMax + " " << " Crit<br>";
    writeNewlineIfReadableFlagSet();

    file << std::right <<
            std::setw(pcWidth) << std::right << " " + nanobotPercentage << "% (" <<
            std::setw(nrWidth) << " " + std::to_string(d.getNanobotCountReceivedFromPlayer())
                               << ") " <<
            std::setw(width) << " " + nanobotMin << " - " <<
            std::left <<
            std::setw(width) << nanobotMax + " " << " Nanobot<br>";
    writeNewlineIfReadableFlagSet();

    if (self) {
        file << std::right <<
                std::setw(pcWidth) << " " + missPercentage << "% (" <<
                std::setw(nrWidth) << " " + std::to_string(d.getMissesReceivedFromPlayer())
                                   << ") " << "Misses<br>";
                writeNewlineIfReadableFlagSet();
    }
    file << std::right <<
            std::setw(pcWidth) << " " + deflectPercentage << "% (" <<
            std::setw(nrWidth) << " " + std::to_string(d.getMissesReceivedFromPlayer())
                               << ") " << "Deflects" <<
            std::setfill(' ');

    file << "<br><br>";
    writeNewlineIfReadableFlagSet();
    writeNewlineIfReadableFlagSet();
}

/******************/
/* DR Stat writes */
/******************/

void DamageWriter::writeDROverviewOthers(const std::string& name,
                                         const Damage& d,
                                         unsigned int place) {
    writeDROverview(name, d, place, false);
}

void DamageWriter::writeDROverviewSelf(const std::string& name,
                                       const Damage& d,
                                       unsigned int place) {
    writeDROverview(name, d, place, true);
}

void DamageWriter::writeDROverview(const std::string& name,
                                   const Damage& d,
                                   unsigned int place,
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
    std::string deflectPercentage = percentage(d.getCountDealtOnPlayer(),
                                               d.getDeflectsDealtOnPlayer());

    const int width = 8;
    const int critOffset = 1;
    file << std::setfill(fillChar) <<
          std::setw(width) << " " + std::to_string(d.getTotalDealtOnPlayer()) << " " <<
          std::setw(width) << " " + std::to_string(d.getDPMDealtOnPlayer()) << " " <<
          std::fixed << std::setprecision(1) <<
          std::setw(width - critOffset) << " " + critPercentage << '%' << " " <<
          std::setw(width - critOffset) << " " + nanobotPercentage << '%' << " ";
    if (self) {
        file << std::setw(width - critOffset) << " " + missPercentage << '%' << " ";
    }
    file << std::setw(width - critOffset) << " " + deflectPercentage << '%' << "  " <<
          std::setfill(' ');

    writePlace(place);
    writeName(name);
    file << "<br>";
    writeNewlineIfReadableFlagSet();
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
