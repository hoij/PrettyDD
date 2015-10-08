#include "affected_player.h"
#include "damage.h"
#include "logger.h"
#include "player.h"
#include "stat_writer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <utility>


StatWriter::StatWriter(PlayerVector<Player*>& playerVector,
                       Configuration& config,
                       NanoProgramWriter& nanoProgramWriter,
                       DamageWriter& damageWriter,
                       XPWriter& xpWriter,
                       std::ofstream& file) :
    WriterHelper(config, file),
    playerVector(playerVector),
    config(config),
    nanoProgramWriter(nanoProgramWriter),
    damageWriter(damageWriter),
    xpWriter(xpWriter) {}

/******/
/* DD */
/******/

void StatWriter::createDDTopList() {
    damageWriter.createDDTopList();
}

void StatWriter::createDDDetailedTopList() {
    damageWriter.createDDDetailedTopList();
}

void StatWriter::createDDPerDamageType(std::string playerName) {
    damageWriter.createDDPerDamageType(playerName);
}

void StatWriter::createDDPerDamageTypeDetailed(std::string playerName) {
    damageWriter.createDDPerDamageTypeDetailed(playerName);
}

void StatWriter::createDDPerOpponent(std::string playerName) {
    damageWriter.createDDPerDamageType(playerName);
}

void StatWriter::createDDOnSpecificOpponent(std::string playerName,
                                            std::string opponentName) {
    damageWriter.createDDOnSpecificOpponent(playerName, opponentName);
}

void StatWriter::createDDOnSpecificOpponentDetailed(
    std::string playerName,
    std::string opponentName) {

    damageWriter.createDDOnSpecificOpponentDetailed(playerName, opponentName);
}

/******/
/* DR */
/******/

void StatWriter::createDRTopList() {
    damageWriter.createDRTopList();
}

void StatWriter::createDRDetailedTopList() {
    damageWriter.createDRDetailedTopList();
}

void StatWriter::createDRPerDamageType(std::string playerName) {
    damageWriter.createDRPerDamageType(playerName);
}

void StatWriter::createDRPerOpponent(std::string playerName) {
    damageWriter.createDRPerOpponent(playerName);
}

/*********************/
/* Create heal files */
/*********************/


/*********************/
/* Create nano files */
/*********************/


/******/
/* XP */
/******/

void StatWriter::createXPInfo() {
    xpWriter.createXPInfo();
}

/*****************/
/* Nano Programs */
/*****************/

void StatWriter::createNanoProgramsCastedDetailedList() {
    nanoProgramWriter.createCastedDetailedList();
}

/********/
/* Help */
/********/

void StatWriter::writeCommand(std::string command, std::string description) {
    file << "  " << command;
    if (!description.empty()) {
        file << " --> " << description;
    }
    file << "<br>";
    writeNewlineIfReadableFlagSet();
}

void StatWriter::createHelp() {
    openFile();

    writeStartOfLink("Help");
    file << "Begin the command with \"pdd\" and add one of the following "
            "options: <br>";
    file << "Damage dealt:" << "<br>";
    writeNewlineIfReadableFlagSet();
    writeCommand("top");
    writeCommand("dtop", "More details");
    writeCommand("[player]", "Overview of \"player\"");
    writeCommand("types", "Overview per type");
    writeCommand("dtypes", "Detailed info per type");
    writeCommand("dtypes [player1] [player2]",
                 "Detailed info per type on the damage done to player2 by player 1");
    writeCommand("types [player]", "Overview per type for \"player\"");
    writeCommand("[player1] [player2]", "Damage on \"player2\" by \"player1\"");
    writeNewlineIfReadableFlagSet();

    file << "Damage Received:" << "<br>";
    writeNewlineIfReadableFlagSet();
    writeCommand("dr top");
    writeCommand("dr dtop");
    writeCommand("dr [player]");
    writeCommand("dr types");
    writeCommand("dr types [player]");
    writeNewlineIfReadableFlagSet();

//    file << "Heal:" << "<br>";
//    writeNewlineIfReadableFlagSet();
//    writeCommand("h top");
//    writeCommand("h dtop");
//    writeCommand("h dealt");
//    writeNewlineIfReadableFlagSet();

//    file << "Nano:" << "<br>";
//    writeNewlineIfReadableFlagSet();
//    writeCommand("n top");
//    writeCommand("n dtop");
//    writeCommand("n dealt");
//    writeNewlineIfReadableFlagSet();

    file << "Nano Program:" << "<br>";
    writeNewlineIfReadableFlagSet();
    writeCommand("np");
//    writeCommand("np casted");
//    writeCommand("np casted t");
//    writeCommand("np casted [player]");
//    writeCommand("np received");
//    writeCommand("np received t");
//    writeCommand("np received [player]");
    writeNewlineIfReadableFlagSet();

    file << "Experience:" << "<br>";
    writeNewlineIfReadableFlagSet();
    writeCommand("xp");
    writeNewlineIfReadableFlagSet();

    file << "Help:" << "<br>";
    writeNewlineIfReadableFlagSet();
    writeCommand("help");

    writeEndOfLink("Help");

    closeFile();
}

/********************/
/* Remove when done */
/********************/

void StatWriter::writeDDOld(const Damage& d) {
    const int width = 8;
    file << std::right << std::setfill(fillChar) <<
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
}
