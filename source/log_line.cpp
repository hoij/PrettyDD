#include "log_line.h"
#include "logger.h"
#include "sstream"

LogLine::LogLine(std::string& line) {
    format(line);
}

bool LogLine::format(std::string& line) {
    originalLine = line;
    split(line, ',', splitLine);
    if (splitLine.size() >= 3) {  // At least some kind of safety check.
        cleanup(splitLine);
        formatted = true;
        return true;
    }
    else {
        errorLog.write("Could not format the following line: ");
        errorLog.write(line);
        return false;
    }
}

void LogLine::cleanup(std::vector<std::string>& splitLine) {
	// Remove quotation marks etc.
	splitLine[0].erase(0, 3);
	splitLine[0].erase(16, 2);
	splitLine[1].erase(0, 1);
	splitLine[1].erase(splitLine[1].length() - 1, splitLine[1].length());
	splitLine[2].erase(0, 1);
	splitLine[2].erase(splitLine[2].length() - 1, splitLine[2].length());
}

std::vector<std::string>& LogLine::split(std::string& s, char delim, std::vector<std::string>& splitLine) {
    // This is shit. But it works.
	std::stringstream ss(s);
	std::string word;
	while (getline(ss, word, delim)) {
		if (!word.empty()) {
			if (delim != ']')
				split(word, ']', splitLine);
			else
				splitLine.push_back(word);
		}
	}
	return splitLine;
}

const std::string& LogLine::getDescription() {
    if (splitLine[0] == "00000003000011fc") {
        // Special case when an org chat message is parsed.
        // [1] contains the org name which is of no use when we only want to
        // know if it is an org message.
        return splitLine[0];
    }
    else {
        return splitLine[1];
    }
}

void LogLine::setHasPlayerStats(bool hasStats) {
    info.hasStats = hasStats;
}
