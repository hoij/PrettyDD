#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "logger.h"

#include <fstream>
#include <string>
#include <sstream>


class Configuration {
public:
    bool read() {
        std::ifstream settings("settings.txt");
        if (settings.is_open()) {
            getline(settings, playerRunningProgram);
            getline(settings, logFilePath);
            getline(settings, scriptsPath);

            // Decide wether to read the log form the start or not
            // (read from the end).
            // Read the text "true" or "false and store it as a bool.
            parseFromEnd = getAndConvertToBool(settings);
            // Decide wether to write the stats to a file that's
            // human readable or not (write it in AO's format of
            // one long line).
            readable = getAndConvertToBool(settings);

            return verifyParameters();
        }
        else {
            errorLog.write("Could not open \"settings.txt\".");
            return false;
        }
    }

    std::string& getPlayerRunningProgram() {return playerRunningProgram;}
    std::string& getLogFilePath() {return logFilePath;}
    std::string& getScriptsPath() {return scriptsPath;}
    bool shouldParseFromEnd() {return parseFromEnd;}
    bool shouldWriteReadable() {return readable;}

private:
    bool getAndConvertToBool(std::ifstream& settings) {
        std::string s;
        bool b;
        getline(settings, s);
        std::istringstream(s) >> std::boolalpha >> b;
        return b;
    }
    bool verifyParameters() {
        if (playerRunningProgram.size() < 4 ||
            playerRunningProgram.size() > 12) {
            errorLog.write("Error: Invalid player name:");
            errorLog.write("Error: " + playerRunningProgram);
            return false;
        }
        if (scriptsPath.size() != 0 && scriptsPath.back() != '\\') {
            scriptsPath += '\\';
        }
        return true;
    }

    std::string playerRunningProgram;
    std::string logFilePath;
    std::string scriptsPath;
    bool parseFromEnd = false;
    bool readable = false;
};


#endif  // CONFIGURATION_H
