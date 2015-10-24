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

void StatWriter::createDRTopList() {
    damageWriter.createDRTopList();
}

void StatWriter::createDDDetailedTopList() {
    damageWriter.createDDDetailedTopList();
}

void StatWriter::createDRDetailedTopList() {
    damageWriter.createDRDetailedTopList();
}

void StatWriter::createDDPerType(std::string playerName) {
    damageWriter.createDDPerType(playerName);
}

void StatWriter::createDRPerType(std::string playerName) {
    damageWriter.createDRPerType(playerName);
}

void StatWriter::createDDPerTypeDetailed(std::string playerName) {
    damageWriter.createDDPerTypeDetailed(playerName);
}

void StatWriter::createDRPerTypeDetailed(std::string playerName) {
    damageWriter.createDRPerTypeDetailed(playerName);
}

void StatWriter::createDDPerOpponent(std::string playerName) {
    damageWriter.createDDPerOpponent(playerName);
}

void StatWriter::createDRPerOpponent(std::string playerName) {
    damageWriter.createDRPerOpponent(playerName);
}

void StatWriter::createDDPerType(std::string playerName,
                                 std::string opponentName) {
    damageWriter.createDDPerType(playerName, opponentName);
}

void StatWriter::createDRPerType(std::string playerName,
                                 std::string opponentName) {
    damageWriter.createDRPerType(playerName, opponentName);
}

void StatWriter::createDDPerTypeDetailed(std::string playerName,
                                         std::string opponentName) {

    damageWriter.createDDPerTypeDetailed(playerName, opponentName);
}

void StatWriter::createDRPerTypeDetailed(std::string playerName,
                                         std::string opponentName) {

    damageWriter.createDRPerTypeDetailed(playerName, opponentName);
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
    if (!openFile("pdd")) {
        return;
    }

    createCommands1();
    createCommands2();
    createInfo1();
    createInfo2();

    closeFile();
}

void StatWriter::createCommands1() {

    writeStartOfLink("PDD Commands 1(2)");

    file << "<br>" << nl <<
            "Begin a command by saying:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd");
    file << "</font>" << nl <<
            "Then add one of the options in the command list.<br>" << nl <<
            "Example:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd dr Sgtcuddle");
    file << "</font>" << nl <<
            "Show the script by typing:<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("/pdd<br>");
    file << "</font>" << nl << nl <<
            "Damage Dealt:" << "<br>" << nl <<
            "<font color = " + lightBlue + ">" << nl;
    writeCommand("top");
    writeCommand("dtop", "Detailed top list");
    writeCommand("opp [player]", "Damage per opponent for player");
    writeCommand("[player]", "As above");
    writeCommand("types", "Damage per type for you");
    writeCommand("types [player]", "Damage per type for player");
    writeCommand("types [player1] [player2]", "Damage per type on player2 by player1");
    writeCommand("[player1] [player2]", "As above");
    writeCommand("dtypes", "Detailed damage per type for you");
    writeCommand("dtypes [player1]", "Detailed damage per type for player");
    writeCommand("dtypes [player1] [player2]",
                 "Detailed damage per type on player2 by player1");
    file << "</font><br>" << nl;

    writeEndOfLink("PDD Commands 1(2)");
}

void StatWriter::createCommands2() {

    writeStartOfLink("PDD Commands 2(2)");

    file << "Damage Received" << "<br>" << nl;
    file << "All the damage dealt commands are available when showing "
            "damage received. Put a \\\"dr\\\" behind \\\"pdd\\\" then follow "
            "with a command from the damage dealt list. Example:<br>" << nl;
    file << "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd dr dtypes [player1] [player2]", "Which btw is the "
                 "same as \\\"pdd dtypes [player2] [player1]\\\"");
    file << "</font><br>" << nl

         << "Nano Program" << "<br>" << nl
         << "<font color = " + lightBlue + ">" << nl;
    writeCommand("np");
    file << "</font><br>" << nl;

    file << "Experience" << "<br>" << nl
         << "<font color = " + lightBlue + ">" << nl;
    writeCommand("xp");
    file << "</font><br>" << nl;

    file << "Time" << "<br>" << nl
         << "<font color = " + lightBlue + ">" << nl;
    writeCommand("pause", "Or \\\"stop\\\" pauses the timer used to calc DPM");
    writeCommand("resume", "Or \\\"start\\\" resumes the timer");
    writeCommand("reset", "Clears everything logged and starts anew directly");
    file << "</font><br>" << nl;

    file << "Help" << "<br>" << nl
         << "<font color = " + lightBlue + ">" << nl;
    writeCommand("help", "Shows this help");
    file << "</font>";

    writeEndOfLink("PDD Commands 2(2)");
}

void StatWriter::createInfo1() {

    writeStartOfLink("PDD Info 1(2)");

    file << "<br>" << nl
        << "Stats Info<br><br>" << nl << nl

        << "CatHit% (Category Hit %) for Normal, Crit, Deflect and Miss<br>" << nl
        << "<font color = " + lightBlue + ">" << nl
        << "* Category hit rate is calculated in the following way:<br>" << nl
        << "   nr of x / "
        "(normal hits + crits (if existing) + deflects + misses)<br><br>"
        << nl << nl
        << "</font>"

        << "TotHit% (Total Hit %)<br>" << nl
        << "<font color = " + lightBlue + ">" << nl
        << "* Total hit rate is calculated in the following way:<br>" << nl
        << "   nr of x / all attacks for that type<br><br>" << nl << nl
        << "</font>"

        << "DPM<br>" << nl
        << "<font color = " + lightBlue + ">" << nl
        << "* DPM is calculated from the first attack to the last attack"
        "<br><br>" << nl << nl
        << "</font>";

    writeEndOfLink("PDD Info 1(2)");
}

void StatWriter::createInfo2() {

    writeStartOfLink("PDD Info 2(2)");

    file << "<br>" << nl
         << "Damage per Player, Opponent and Detailed Top List<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "* Regular, special, nanobot and shield % is % of total damage<br>" << nl
         << "* Crit, deflect and miss % is % of hits that can "
            "crit, deflect or miss (CatHit%)<br><br>" << nl << nl
         << "</font>"

         << "Damage per Type (types)<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "* Regular and nanobot % is % of total damage<br>" << nl
         << "* Crit, deflect and miss % is % of hits that can "
            "crit, deflect or miss (CatHit%)<br>" << nl
         << "* A blank line means that the stat isn't possible to log "
            "or doesn't exist<br>"
         << "* Misses from regular attacks have no type info and can't "
            "be logged per type. Regular misses are instead "
            "shown as a separate type<br><br>" << nl << nl
         << "</font>"

         << "Detailed Damage per Type (dtypes)<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "* Regular Total is the sum of normal, crit and deflect<br>" << nl
         << "* Damage % is % of total damage<br>" << nl
         << "* TotHit% - See info 1<br>" << nl
         << "* CatHit% - See info 1<br>" << nl
         << "</font>"

         << "XP Info<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "* Min xp gained ignores xp of value 1.<br><br>" << nl << nl
         << "</font>";

    writeEndOfLink("PDD Info 2(2)");
}
