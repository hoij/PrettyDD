#include "formatted_line.h"
#include "logger.h"
#include "sstream"

FormattedLine::FormattedLine(std::string& line) {
    format(line);
}

bool FormattedLine::format(std::string& line) {
    originalLine = line;
    split(line, ',', formattedLine);
    if (formattedLine.size() == 6) {
        // Some log lines contain a comma, merge those lines back together
        formattedLine[4] = formattedLine[4] + "," + formattedLine[5];
        formattedLine.erase(formattedLine.begin() + 5);
    }
    if (formattedLine.size() >= 3) {  // At least some kind of safety check.
        cleanup(formattedLine);
        formatted = true;
        return true;
    }
    else {
        // TODO: Raise excpetion
        errorLog.write("Could not format the following line: ");
        errorLog.write(line);
        return false;
    }
}

void FormattedLine::cleanup(std::vector<std::string>& formattedLine) {
	// Remove quotation marks etc.
	formattedLine[0].erase(0, 3);
	formattedLine[0].erase(16, 2);
	formattedLine[1].erase(0, 1);
	formattedLine[1].erase(formattedLine[1].length() - 1, formattedLine[1].length());
	formattedLine[2].erase(0, 1);
	formattedLine[2].erase(formattedLine[2].length() - 1, formattedLine[2].length());
}

std::vector<std::string>& FormattedLine::split(std::string& s, char delim, std::vector<std::string>& formattedLine) {
    // This is shit. But it works.
    // First char passed in must be a comma ","
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

const std::string& FormattedLine::getSender() const {
    // This string only contains something if the line is a chat line.
    // Otherwise it is empty.
    return formattedLine[2];
}

const std::string& FormattedLine::getDescription() const {
    if (formattedLine[0] == "00000003000011fc") {
        // Special case when an org chat message is parsed.
        // [1] contains the org name which is of no use when we only want to
        // know if it is an org message.
        return formattedLine[0];
    }
    else {
        return formattedLine[1];
    }
}
