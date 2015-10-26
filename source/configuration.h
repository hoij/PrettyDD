#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <fstream>
#include <string>


class Configuration {
public:
    bool read();

    std::string& getPlayerRunningProgram() {return playerRunningProgram;}
    std::string& getLogFilePath() {return logFilePath;}
    std::string& getScriptsPath() {return scriptsPath;}

    bool shouldParseFromEnd() {return parseFromEnd;}
    bool shouldWriteReadable() {return readable;}

private:
    bool getAndConvertToBool(std::ifstream& settings);

    bool verifyParameters();

    std::string playerRunningProgram;
    std::string logFilePath;
    std::string scriptsPath;
    bool parseFromEnd = false;
    bool readable = false;
};


#endif  // CONFIGURATION_H
