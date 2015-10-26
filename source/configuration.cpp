#include "configuration.h"

#include <sstream>


bool Configuration::read() {
    std::ifstream settings("settings.txt");
    if (settings.is_open()) {
        std::string key;
        getline(settings, key, '=');
        getline(settings, playerRunningProgram);
        getline(settings, key, '=');
        getline(settings, logFilePath);
        getline(settings, key, '=');
        getline(settings, scriptsPath);
        getline(settings, key, '=');

        // Decide wether to read the log form the start or from the end
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

bool Configuration::getAndConvertToBool(std::ifstream& settings) {
    std::string s;
    bool b;
    getline(settings, s, '=');
    getline(settings, s);
    std::istringstream(s) >> std::boolalpha >> b;
    return b;
}

bool Configuration::verifyParameters() {
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
