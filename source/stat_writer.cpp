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


/********************/
/* Remove when done */
/********************/

void StatWriter::writeDDOld(const Damage& d, std::ostream& os) {
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
}
