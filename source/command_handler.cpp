#include "command_handler.h"

#include <sstream>
#include <vector>


void CommandHandler::execute(std::string command) {
    std::vector<std::string> commandSections = splitCommand(command);
    if (commandSections[1] == "someOption") {
        // Do something
    }
    else {
        statWriter.createDDTopList();
        statWriter.createDDDetailedOverview();
        statWriter.createDDPerOpponent("You");
        statWriter.createDDPerOpponent("Nonexisting");
        statWriter.createDDOnSpecificOpponent("You", "asshole");
        statWriter.createDDOnSpecificOpponent("You", "Predator Rogue");
        statWriter.createDDPerDamageType("You");
        //statWriter.createDDPerOpponentDetailed("Predator Rogue", "You");
        statWriter.createDRDetailedOverview();
        statWriter.createOldDDOverview();
    }

    /*
    else {
        errorLog.write("Unknown command: " + command);
    }
    */
}

std::vector<std::string> CommandHandler::splitCommand(std::string command) {
    std::vector<std::string> commandSections;
    std::string section;
    std::stringstream ss(command);
    while (std::getline(ss, section, ' ')) {
        commandSections.push_back(section);
    }
    return commandSections;
}
