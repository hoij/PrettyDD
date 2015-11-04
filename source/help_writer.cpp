#include "help_writer.h"


HelpWriter::HelpWriter(Configuration& config,
                       std::ofstream& file) :
                       WriterHelper(config, file) {}

void HelpWriter::createHelp(std::string fileName) {
    if (!openFile(fileName)) {
        return;
    }

    createInfo();
    createCommands1();
    createCommands2();


    closeFile();
}

void HelpWriter::createInfo() {

    writeStartOfLink("Welcome to PrettyDD!");
    file << "PrettyDD generates script files based on commands spoken into chat channels.<br>" << nl
         << "<br>" << nl
         << "<font color = " + yellow + ">" << nl
         << "Usage"
         << "</font><br>" << nl
         << "Begin a command with:<br>" << nl;
    writeCommand("pdd");
    file << "<br>" << nl

         << "Then add one of the options from the command list.<br>" << nl
         << "Examples:<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "  pdd top<br>" << nl
         << "  pdd dtop<br>" << nl
         << "  pdd types" << nl
         << "</font><br><br>" << nl << nl

         << "Show the result by typing:<br>" << nl;
    writeCommand("/pdd");
    file << "<br>" << nl
         << "<font color = " + yellow + ">" << nl
         << "----------------------------------" << nl
         << "</font><br><br>" << nl << nl

         << "You can show this help by typing:<br>" << nl;
    writeCommand("/pddhelp");
    file << "<br>" << nl
         << "For a complete list of commands, see the \\\"commands\\\" section.<br>" << nl
         << "For detailed information, see the README.";

    writeEndOfLink("Info");
}

void HelpWriter::createCommands1() {

    writeStartOfLink("Commands 1(2)");

    file << "<br>" << nl;
    writeCommand("top","Simple list of damage dealers");
    writeCommand("dtop", "Detailed top list");
    writeCommand("[Player]", "Damage per opponent by the specified player");
    writeCommand("types", "Damage per type by you");
    writeCommand("types [Player1] [Player2]", "Damage per type by specified player(s)");
    writeCommand("dtypes", "Detailed damage per type by you");
    writeCommand("dtypes [Player1] [Player2]", "Detailed damage per type by specified player(s)");
    file << "<br>" << nl
         << "<font color = " + yellow + ">" << nl
         << "----------------------------------" << nl
         << "</font><br><br>" << nl << nl

         << "You can also show damage received by adding <font color = #3399FF>dr</font> after pdd.<br>" << nl
         << "Examples:<br>" << nl
         << "<font color = " + lightBlue + ">" << nl
         << "  pdd dr top<br>" << nl
         << "  pdd dr Player<br>" << nl
         << "  pdd dr types Player1 \\\"The Beast\\\"" << nl
         << "</font><br><br>" << nl << nl
    
         << "Note that player names are case sensitive.";

    writeEndOfLink("Commands 1(2)");
}

void HelpWriter::createCommands2() {

    writeStartOfLink("Commands 2(2)");

    file << "<br>" << nl;
    writeCommand("np", "Shows a detailed list of casted nanoprograms");
    file << "<br>" << nl;
    writeCommand("xp", "Shows information on XP/SK/PVP Score gained<br>");
    writeCommand("pause", "Or \\\"stop\\\" pauses the timer used to calculate DPM");
    writeCommand("resume", "Or \\\"start\\\" resumes the timer");
    writeCommand("reset", "Resets current session");
    writeCommand("quit", "Closes PrettyDD");
    file << "<br>" << nl;
    writeCommand("help", "Shows this help");

    writeEndOfLink("Commands 2(2)");
}

void HelpWriter::writeCommand(std::string command, std::string description) {
    file << "<font color = " + lightBlue + ">" << nl
         << "  " << command << "</font>";
    if (!description.empty()) {
        file << " --> " << description;
    }
    file << "<br>" << nl;
}