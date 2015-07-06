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

void LogLine::setUnfitForPlayerAddition() {
    fitForPlayerAddition = false;
}

void LogLine::setCommand(std::string cmd) {
    command = cmd;
    isLineACommand = true;
}
