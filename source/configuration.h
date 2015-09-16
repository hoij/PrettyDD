#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include "logger.h"

#include <fstream>
#include <string>


class Configuration {
public:
    bool read() {
        std::ifstream settings("settings.txt");
        if (settings.is_open()) {
            getline(settings, playerRunningProgram);
            getline(settings, logFilePath);
            return verifyParameters();
        }
        else {
            errorLog.write("Could not open \"settings.txt\".");
            return false;
        }
    }

    std::string& getplayerRunningProgram() {return playerRunningProgram;}
    std::string& getLogFilePath() {return logFilePath;}

private:
    bool verifyParameters() {
        if (playerRunningProgram.size() < 4 ||
            playerRunningProgram.size() > 12) {
            errorLog.write("Error: Invalid player name:");
            errorLog.write("Error: " + playerRunningProgram);
            return false;
        }
        return true;
    }

    std::string playerRunningProgram;
    std::string logFilePath;
};


#endif  // CONFIGURATION_H
