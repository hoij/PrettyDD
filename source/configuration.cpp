#include "configuration.h"
#include "logger.h"

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
    std::string key;
    std::string s;
    bool b;
    getline(settings, key, '=');  // Read the key value
    getline(settings, s);  // read what should be a bool string
    if (s != "true" && s != "false") {
        errorLog.write("Error: Expected \"true\" or \"false\" when reading "
                       "the settings key \"" + key + "\". Instead got: " + s);
    }
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
