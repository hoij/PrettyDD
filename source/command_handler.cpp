#include "command_handler.h"
#include "line_info.h"
#include "my_time_interface.h"

#include <sstream>
#include <vector>

CommandHandler::CommandHandler(PlayerVector<Player*>& playerVector,
                               std::ofstream& file,
                               DamageWriter& damageWriter,
                               HelpWriter& helpWriter,
                               MyTimeInterface& myTime,
                               NanoProgramWriter& nanoProgramWriter,
                               WriterHelper& writerHelper,
                               XPWriter& xpWriter) :
    playerVector(playerVector),
    damageWriter(damageWriter),
    myTime(myTime),
    nanoProgramWriter(nanoProgramWriter),
    helpWriter(helpWriter),
    writerHelper(writerHelper),
    xpWriter(xpWriter) {}


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
        damageWriter.createDDDetailedTopList();
    }
    else if (nrOfOptions == 1) {
        // No extra options on top/dtop/types will print damage dealt
        // as it's the most common thing to want.
        if (commandParts[1] == "top") {
            damageWriter.createDDTopList();
        }
        else if (commandParts[1] == "dtop") {
            damageWriter.createDDDetailedTopList();
        }
        else if (commandParts[1] == "types") {
            damageWriter.createDDPerType("You");
        }
        else if (commandParts[1] == "dtypes") {
            damageWriter.createDDPerTypeDetailed("You");
        }
        else if (commandParts[1] == "np") {
            nanoProgramWriter.createCastedDetailedList();
        }
        else if (commandParts[1] == "xp") {
            xpWriter.createXPInfo();
        }
        else if (commandParts[1] == "help") {
            std::string fileName = "pdd";
            helpWriter.createHelp(fileName);
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
                writerHelper.clearFile("pdd");
        }
        else if (commandParts[1] == "quit") {
            shouldContinue = false;
        }
        else {
            damageWriter.createDDPerOpponent(commandParts[1]);
        }
    }
    else if (nrOfOptions == 2) {
        if (commandParts[1] == "opp") {
            damageWriter.createDDPerOpponent(commandParts[2]);
        }
        else if (commandParts[1] == "dtypes") {
            damageWriter.createDDPerTypeDetailed(commandParts[2]);
        }
        else if (commandParts[1] == "dr") {
            if (commandParts[2] == "top") {
                damageWriter.createDRTopList();
            }
            else if (commandParts[2] == "dtop") {
                damageWriter.createDRDetailedTopList();
            }
            else if (commandParts[2] == "types") {
                damageWriter.createDRPerType("You");
            }
            else if (commandParts[2] == "dtypes") {
                damageWriter.createDRPerTypeDetailed("You");
            }
            else {
                damageWriter.createDRPerOpponent(commandParts[2]);
            }
        }
        else if (commandParts[1] == "h") {  // Heal
            if (commandParts[2] == "top") {
                //healWriter.createHealReceivedTopList();
            }
            else if (commandParts[2] == "dtop") {
                //healWriter.createHealReceivedDetailedTopList();
            }
            else if (commandParts[2] == "dealt") {
                //healWriter.createHealDealtPerReceiver();
            }
        }
        else if (commandParts[1] == "n") {  // Nano
            if (commandParts[2] == "top") {
                //nanoWriter.createNanoReceivedTopList();
            }
            else if (commandParts[2] == "dtop") {
                //nanoWriter.createNanoReceivedDetailedTopList();
            }
            else if (commandParts[2] == "dealt") {
                //nanoWriter.createNanoDealtPerReceiver();
            }
        }
        else if (commandParts[1] == "np") {  // Nano Program
            if (commandParts[2] == "casted") {
                nanoProgramWriter.createCastedDetailedList();
            }
            else if (commandParts[2] == "received") {
                //nanoProgramWriter.createNanoProgramsReceived();
            }
        }
        else if (commandParts[1] == "types") {
                damageWriter.createDDPerType(commandParts[2]);
        }
        else {
            damageWriter.createDDPerType(commandParts[1], commandParts[2]);
        }
    }
    else if (nrOfOptions == 3) {
        if (commandParts[1] == "dr") {
            if (commandParts[2] == "types") {
                damageWriter.createDRPerType(commandParts[3]);
            }
            else if (commandParts[2] == "dtypes") {
                damageWriter.createDRPerTypeDetailed(commandParts[3]);
            }
            else if (commandParts[2] == "opp") {
                damageWriter.createDRPerOpponent(commandParts[3]);
            }
            else {
                damageWriter.createDRPerType(commandParts[2], commandParts[3]);
            }
        }
        else if (commandParts[1] == "np") {  // Nano Program
            if (commandParts[2] == "casted" &&
                commandParts[3] == "t") {
                //nanoProgramWriter.createNanoProgramsCastedPerTarget();
            }
            else if (commandParts[2] == "received" &&
                     commandParts[3] == "t") {
                //nanoProgramWriter.createNanoProgramsReceivedPerTarget();
            }
            else if (commandParts[2] == "casted") {
                //nanoProgramWriter.createNanoProgramsCastedOnTarget(commandParts[3]);
            }
            else if (commandParts[2] == "received") {
                //nanoProgramWriter.createNanoProgramsReceivedFromTarget(
                //    commandParts[3]);
            }
        }
        else if (commandParts[1] == "types") {
            damageWriter.createDDPerType(commandParts[2], commandParts[3]);
        }
        else if (commandParts[1] == "dtypes") {
            damageWriter.createDDPerTypeDetailed(commandParts[2],
                                               commandParts[3]);
        }
    }
    else if (nrOfOptions == 4) {
        if (commandParts[1] == "dr") {
            if (commandParts[2] == "types") {
                damageWriter.createDRPerType(commandParts[3], commandParts[4]);
            }
            else if (commandParts[2] == "dtypes") {
                damageWriter.createDRPerTypeDetailed(commandParts[3],
                                                   commandParts[4]);
            }
        }
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
        else if (part.front() == '"' &&
                 part.back() == '"' &&
                 part.size() == 1 &&
                 insideQuote) {
            quotedString += part;
            result.push_back(quotedString);
            insideQuote = !insideQuote;
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
