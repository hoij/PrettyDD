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
    damageWriter.createDDPerOpponent(playerName);
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
    file << "<br>" << nl;
}

void StatWriter::createHelp() {
    if (!openFile("pdd1")) {
        return;
    }

    createCommands();
    createInfo();

    closeFile();
}

void StatWriter::createCommands() {

    writeStartOfLink("PDD Commands");
    file << "<br>" << nl <<
            "Begin the command by saying:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd");
    file << "</font>" << nl <<
            "Then add one of the below options.<br>" << nl <<
            "Example:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd dr Sgtcuddle");
    file << "</font>" << nl <<
            "Show the script by typing:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("/pdd<br>");
    file << "</font>" << nl << nl <<
            "Damage dealt:" << "<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("top");
    writeCommand("dtop", "More details");
    writeCommand("[player]", "Overview of player");
    writeCommand("types", "Overview per type");
    writeCommand("dtypes", "Detailed info per type");
    writeCommand("dtypes [player1] [player2]",
                 "Detailed info for damage on player2 by player 1");
    writeCommand("types [player]", "Overview per type for player");
    writeCommand("[player1] [player2]", "Damage on player2 by player1");
    file << "</font>";
    file << "<br>" << nl;

    file << "Damage Received:" << "<br>" << nl;
    file << "<font color = " + lightBlue + ">" << nl;
    writeCommand("dr top");
    writeCommand("dr dtop");
    writeCommand("dr [player]");
    writeCommand("dr types");
    writeCommand("dr types [player]");
    file << "</font>";
    file << "<br>" << nl;

//    file << "Heal:" << "<br>" << nl;
//    writeCommand("h top");
//    writeCommand("h dtop");
//    writeCommand("h dealt") << nl;

//    file << "Nano:" << "<br>" << nl;
//    writeCommand("n top");
//    writeCommand("n dtop");
//    writeCommand("n dealt") << nl;

    file << "Nano Program:" << "<br>" << nl;
    file << "<font color = " + lightBlue + ">" << nl;
    writeCommand("np");
//    writeCommand("np casted");
//    writeCommand("np casted t");
//    writeCommand("np casted [player]");
//    writeCommand("np received");
//    writeCommand("np received t");
//    writeCommand("np received [player]");
    file << "</font>";
    file << "<br>" << nl;

    file << "Experience:" << "<br>" << nl;
    file << "<font color = " + lightBlue + ">" << nl;
    writeCommand("xp");
    file << "</font>";
    file << "<br>" << nl;

    file << "Help:" << "<br>" << nl;
    file << "<font color = " + lightBlue + ">" << nl;
    writeCommand("help");
    file << "</font>";

    writeEndOfLink("PDD Commands");
}

void StatWriter::createInfo() {

    writeStartOfLink("PDD Info");

    file << "<br>" << nl
         << "Crit Hit Rate Info<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "The crit rate is calculated in the following way:<br>" << nl
         << "  nr of crits / (normal hits + crits + normal deflects + normal misses)<br>" << nl
         << "Nanobot, special and shield hits are excluded.<br><br>" << nl << nl
         << "</font>"

         << "Miss/Deflect Rate Info<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "The miss rate is calculated in the following way:<br>" << nl
         << "  nr of misses (or deflects) ( for normals and specials) / <br>"
         << "  (all attacks/deflects/misses except for nanobots and shields)"
         << "<br><br>" << nl << nl
         << "</font>"

         << "Nanobot Info<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "The regular/special/nanobot % shows the % of total damage done<br>." << nl
         << "The following is included in each damage category:<br>" << nl
         << "  Regular: regular hits, crits and regular deflects<br>" << nl
         << "  Special: Special hits and special deflects<br>" << nl
         << "  Nanobot: Nanobot hits<br>" << nl
         << "</font>"

         << "Detailed info per type<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "The regular and nanobot hit% is % of total hits.<br>" << nl
         << "The crit, deflect and miss hit % is % of hits that can<br>" << nl
         << "  crit, i.e. only regular attacks. <br>" << nl
         << "The damage % is % of total damage done.<br><br>" << nl << nl
         << "</font>";

    writeEndOfLink("PDD Info");
}
