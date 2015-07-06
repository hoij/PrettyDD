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
            getline(settings, programOwnerName);
            getline(settings, logFilePath);
            return true;
        }
        else {
            errorLog.write("Could not open \"settings.txt\".");
            return false;
        }
    }
    std::string& getProgramOwnerName() {return programOwnerName;}
    std::string& getLogFilePath() {return logFilePath;}
private:
    std::string programOwnerName;
    std::string logFilePath;
};


extern Configuration config;


#endif  // CONFIGURATION_H
