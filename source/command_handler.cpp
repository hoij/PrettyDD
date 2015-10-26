#include "command_handler.h"
#include "line_info.h"
#include "my_time_interface.h"

#include <sstream>
#include <vector>


bool CommandHandler::execute(const LineInfo& li) {

    bool shouldContinue = true;

    // Avoid executing old commands
    if (wasCommandTypedLongAgo(li.time)) {
        return shouldContinue;
    }

    std::string command = li.command;
    std::vector<std::string> commandParts =
        mergeQuotedText(splitCommand(command));

    int nrOfOptions = (int)commandParts.size() - 1;

    // TODO: Split into several methods and eventually switch to
    // some other way of handling this.
    if (nrOfOptions == 0) {
        // Write the detailed top list by defualt:
        statWriter.createDDDetailedTopList();
    }
    else if (nrOfOptions == 1) {
        // No extra options on top/dtop/types will print damage dealt
        // as it's the most common thing to want.
        if (commandParts[1] == "top") {
            statWriter.createDDTopList();
        }
        else if (commandParts[1] == "dtop") {
            statWriter.createDDDetailedTopList();
        }
        else if (commandParts[1] == "types") {
            statWriter.createDDPerType("You");
        }
        else if (commandParts[1] == "dtypes") {
            statWriter.createDDPerTypeDetailed("You");
        }
        else if (commandParts[1] == "np") {
            statWriter.createNanoProgramsCastedDetailedList();
        }
        else if (commandParts[1] == "xp") {
            statWriter.createXPInfo();
        }
        else if (commandParts[1] == "help") {
            statWriter.createHelp();
        }
        else if (commandParts[1] == "start" ||
                 commandParts[1] == "resume") {
            playerVector.startLogging();
        }
        else if (commandParts[1] == "stop" ||
                 commandParts[1] == "pause") {
            playerVector.stopLogging();
        }
        else if (commandParts[1] == "reset") {
                playerVector.reset();
        }
        else if (commandParts[1] == "quit") {
            shouldContinue = false;
        }
        else {
            statWriter.createDDPerOpponent(commandParts[1]);
        }
    }
    else if (nrOfOptions == 2) {
        if (commandParts[1] == "opp") {
            statWriter.createDDPerOpponent(commandParts[2]);
        }
        else if (commandParts[1] == "dtypes") {
            statWriter.createDDPerTypeDetailed(commandParts[2]);
        }
        else if (commandParts[1] == "dr") {
            if (commandParts[2] == "top") {
                statWriter.createDRTopList();
            }
            else if (commandParts[2] == "dtop") {
                statWriter.createDRDetailedTopList();
            }
            else if (commandParts[2] == "types") {
                statWriter.createDRPerType("You");
            }
            else if (commandParts[2] == "dtypes") {
                statWriter.createDRPerTypeDetailed("You");
            }
            else {
                statWriter.createDRPerOpponent(commandParts[2]);
            }
        }
        else if (commandParts[1] == "h") {  // Heal
            if (commandParts[2] == "top") {
                //statWriter.createHealReceivedTopList();
            }
            else if (commandParts[2] == "dtop") {
                //statWriter.createHealReceivedDetailedTopList();
            }
            else if (commandParts[2] == "dealt") {
                //statWriter.createHealDealtPerReceiver();
            }
        }
        else if (commandParts[1] == "n") {  // Nano
            if (commandParts[2] == "top") {
                //statWriter.createNanoReceivedTopList();
            }
            else if (commandParts[2] == "dtop") {
                //statWriter.createNanoReceivedDetailedTopList();
            }
            else if (commandParts[2] == "dealt") {
                //statWriter.createNanoDealtPerReceiver();
            }
        }
        else if (commandParts[1] == "np") {  // Nano Program
            if (commandParts[2] == "casted") {
                statWriter.createNanoProgramsCastedDetailedList();
            }
            else if (commandParts[2] == "received") {
                //statWriter.createNanoProgramsReceived();
            }
        }
        else if (commandParts[1] == "types") {
                statWriter.createDDPerType(commandParts[2]);
        }
        else {
            statWriter.createDDPerType(commandParts[1], commandParts[2]);
        }
    }
    else if (nrOfOptions == 3) {
        if (commandParts[1] == "dr") {
            if (commandParts[2] == "types") {
                statWriter.createDRPerType(commandParts[3]);
            }
            else if (commandParts[2] == "opp") {
                statWriter.createDRPerOpponent(commandParts[3]);
            }
            else {
                statWriter.createDRPerType(commandParts[2], commandParts[3]);
            }
        }
        else if (commandParts[1] == "np") {  // Nano Program
            if (commandParts[2] == "casted" &&
                commandParts[3] == "t") {
                //statWriter.createNanoProgramsCastedPerTarget();
            }
            else if (commandParts[2] == "received" &&
                     commandParts[3] == "t") {
                //statWriter.createNanoProgramsReceivedPerTarget();
            }
            else if (commandParts[2] == "casted") {
                //statWriter.createNanoProgramsCastedOnTarget(commandParts[3]);
            }
            else if (commandParts[2] == "received") {
                //statWriter.createNanoProgramsReceivedFromTarget(
                //    commandParts[3]);
            }
        }
        else if (commandParts[1] == "types") {
            statWriter.createDDPerType(commandParts[2], commandParts[3]);
        }
        else if (commandParts[1] == "dtypes") {
            statWriter.createDDPerTypeDetailed(commandParts[2],
                                               commandParts[3]);
        }
    }
    else if (nrOfOptions == 4) {
        if (commandParts[1] == "dr") {
            if (commandParts[2] == "types") {
                statWriter.createDRPerType(commandParts[3], commandParts[4]);
            }
            else if (commandParts[2] == "dtypes") {
                statWriter.createDRPerTypeDetailed(commandParts[3],
                                                   commandParts[4]);
            }
        }
    }
    else {
        errorLog.write("Too many commands: " + command);
    }

    return shouldContinue;
}

std::vector<std::string> CommandHandler::splitCommand(std::string command) {
    std::vector<std::string> commandParts;
    std::string section;
    std::stringstream ss(command);
    while (std::getline(ss, section, ' ')) {
        commandParts.push_back(section);
    }
    return commandParts;
}

std::vector<std::string> CommandHandler::mergeQuotedText(
    std::vector<std::string> commandParts) {
    /* Merges text within the same quotation marks.
    Can not handle names beginning with a space. */
    // TODO: Fix this backwards implementation. Maybe do it C-style.
    std::vector<std::string> result;
    std::string quotedString;
    bool insideQuote = false;

    for (auto& part : commandParts) {
        if (part.front() == '"' && part.back() != '"') {
            insideQuote = !insideQuote;
            part.erase(0, 1);
            quotedString += part + " ";
        }
        else if (part.front() == '"' &&
                 part.back() == '"' &&
                 part.size() != 1 &&
                 !insideQuote) {
            part.erase(0, 1);
            part.erase(part.size() -1);
            result.push_back(part);
        }
        else if (part.front() == '"' && part.size() == 1 && !insideQuote) {
            insideQuote = !insideQuote;
            quotedString += part + " ";
        }
        else if (part.front() == '"' && part.size() == 1 && insideQuote) {
            quotedString += part + " ";
            result.push_back(quotedString);
            insideQuote = !insideQuote;
        }
        else if (part.back() == '"') {
            part.erase(part.size() - 1);
            quotedString += part;
            result.push_back(quotedString);
            quotedString = "";
            insideQuote = !insideQuote;
        }
        else if (insideQuote) {
            quotedString += part + " ";
        }
        else {
            result.push_back(part);
        }
    }
    return result;
}

bool CommandHandler::wasCommandTypedLongAgo(const std::time_t& t) {
    // Return true if the command happened in the last 30 s.
    // This method is needed to avoid executing old commands
    // when reparsing a log.
    return t < myTime.currentTime() - 30;
}
