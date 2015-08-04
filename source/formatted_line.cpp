#include "formatted_line.h"
#include "logger.h"
#include <sstream>

FormattedLine::FormattedLine(std::string& line) {
    format(line);
}

bool FormattedLine::format(std::string& line) {
    originalLine = line;
    split(line, ',', formattedLine);
    if (formattedLine.size() == 6) {
        // Some log lines contain a comma, merge those lines back together
        // TODO: Make it possible to have a line with any number of commas.
        formattedLine[4] = formattedLine[4] + "," + formattedLine[5];
        formattedLine.erase(formattedLine.begin() + 5);
    }
    if (formattedLine.size() >= 3) {  // At least some kind of safety check.
        cleanup(formattedLine);
        formatted = true;
        return true;
    }
    else {
        // TODO: Raise exception
        errorLog.write("Could not format the following line: ");
        errorLog.write(line);
        return false;
    }
}

void FormattedLine::cleanup(std::vector<std::string>& formattedLine) {
	// Remove quotation and hashtag marks.
	if (formattedLine[0].length() == 21) {
        formattedLine[0].erase(0, 3);
        formattedLine[0].erase(16, 2);
    }
    else {
        // Only quotation marks exists. Happens with "Team" chat messages
        formattedLine[0].erase(0, 2);
        formattedLine[0].erase(formattedLine[0].length() - 1, formattedLine[0].length());
    }
	formattedLine[1].erase(0, 1);
	formattedLine[1].erase(formattedLine[1].length() - 1, formattedLine[1].length());
	formattedLine[2].erase(0, 1);
	formattedLine[2].erase(formattedLine[2].length() - 1, formattedLine[2].length());
}

std::vector<std::string>& FormattedLine::split(std::string& s, char delim, std::vector<std::string>& formattedLine) {
    // First char passed in must be a comma ","
    // This is shit. But it works.
    // TODO: Make it less shitty.
	std::stringstream ss(s);
	std::string word;
	while (getline(ss, word, delim)) {
		if (!word.empty()) {
			if (delim != ']')
				split(word, ']', formattedLine);
			else
				formattedLine.push_back(word);
		}
	}
	return formattedLine;
}
