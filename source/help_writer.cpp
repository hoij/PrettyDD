#include "help_writer.h"


HelpWriter::HelpWriter(Configuration& config,
                       std::ofstream& file) :
                       WriterHelper(config, file) {}

void HelpWriter::createHelp(std::string fileName) {
    if (!openFile(fileName)) {
        return;
    }

    createCommands1();
    createCommands2();
    createInfo1();
    createInfo2();

    closeFile();
}

void HelpWriter::createCommands1() {

    writeStartOfLink("PDD Commands 1(2)");

    file << "<br>" << nl <<
        "PrettyDD generates script files based on commands spoken into "
        "chat channels.<br>" << nl <<
        "Each command begins with \\\"pdd\\\" and is followed with options "
        "from the command list.<br>" << nl <<
        "Example:<br>" << nl <<
        "<font color = " + lightBlue + ">" << nl;
    writeCommand("pdd dr Sgtcuddle");
    file << "</font>" << nl <<
        "To show the generated script, type:<br>" << nl <<
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

void HelpWriter::createCommands2() {

    writeStartOfLink("PDD Commands 2(2)");

    file << "<br>" <<
        "Damage Received" << "<br>" << nl;
    file << "All the damage dealt commands are available when showing "
        "damage received.<br>" << nl <<
        "Put a \\\"dr\\\" behind \\\"pdd\\\" then follow "
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

    file << "General" << "<br>" << nl
        << "<font color = " + lightBlue + ">" << nl;
    writeCommand("pause", "Or \\\"stop\\\" pauses the timer used to calc DPM");
    writeCommand("resume", "Or \\\"start\\\" resumes the timer");
    writeCommand("reset", "Clears everything logged and starts anew directly");
    writeCommand("quit");
    file << "</font><br>" << nl;

    file << "Help" << "<br>" << nl
        << "<font color = " + lightBlue + ">" << nl;
    writeCommand("help", "Shows this help");
    file << "</font>";

    writeEndOfLink("PDD Commands 2(2)");
}

void HelpWriter::createInfo1() {

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
        << "* DPM is calculated individually from the first action "
           "(attack, heal, nano cast, etc.) to the last action."
           "<br><br>" << nl << nl
        << "</font>";

    writeEndOfLink("PDD Info 1(2)");
}

void HelpWriter::createInfo2() {

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
        << "* CatHit% - See info 1<br><br>" << nl
        << "</font>"

        << "XP<br>" << nl
        << "<font color = " + lightBlue + ">" << nl
        << "* Min xp gained ignores xp of value 1.<br><br>" << nl << nl
        << "</font>";

    writeEndOfLink("PDD Info 2(2)");
}

void HelpWriter::writeCommand(std::string command, std::string description) {
    file << "  " << command;
    if (!description.empty()) {
        file << " --> " << description;
    }
    file << "<br>" << nl;
}