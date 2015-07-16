#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <fstream>
#include <string>
#include "logger.h"


class Configuration {
public:
    bool read() {
        std::ifstream settings("settings.txt");
        if (settings.is_open()) {
            getline(settings, playerRunningProgram);
            getline(settings, logFilePath);
            return true;
        }
        else {
            errorLog.write("Could not open \"settings.txt\".");
            return false;
        }
    }
    std::string& getplayerRunningProgram() {return playerRunningProgram;}
    std::string& getLogFilePath() {return logFilePath;}
private:
    std::string playerRunningProgram;
    std::string logFilePath;
};


#endif  // CONFIGURATION_H
