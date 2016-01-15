#include "formatted_line.h"
#include "logger.h"

#include <regex>
#include <stdexcept>


using std::regex;
using std::regex_search;


bool FormattedLine::format(std::string line) {
    // Splits the log line into five parts and stores the original line.
    originalLine = line;
    try {
        // Each find function has to be called in this exact order as they
        // rely on the previous function removing it's match plus garbage from
        // the string. Doing it this way simplifies the regexs'.
        //
        // Example lines:
        // ["#00000003000011fc#","Pantheon [YOLO]","Sgtcuddle",1436182498]test
        // ["Team","Team","Sgtcuddle",1436182391]test
        formattedLine.push_back(findDescriptionCode(line));
        formattedLine.push_back(findDescription(line));
        formattedLine.push_back(findSender(line));
        formattedLine.push_back(findTime(line));
        formattedLine.push_back(line);  // The remaining string contains the message
    }
    catch (const std::invalid_argument& e) {
        // Only one catch statement because if any of the calls above fail
        // there is no point in running the rest.
        formatted = false;
        errorLog.write("Error: " + *e.what());
        errorLog.write("Error: Full line: " + line);
    }
    return formatted;
}

std::string FormattedLine::findDescriptionCode(std::string& s) {
    // Needs to be the first function to call when extracting info.
    std::smatch m;
    std::string descriptionCode;
    // Match everything between ["# and #"
    // Org names can contain both [] and a comma.
    // But they can't contain " and # afaik (or maybe they just aren't used).
    if (regex_search(s, m, regex("(?:^\\[\"#)(.*?)(?=#\")"))) {
        descriptionCode = m[1];
        // Erase the match plus the surrounding chars ["# and #","
        s.erase(0, m[1].length() + 3 + 4);
    }
    // Match everything between [" and "
    else if (regex_search(s, m, regex("(?:^\\[\")(.*?)(?=\")"))) {
        descriptionCode = m[1];
        // Erase the match plus the surrounding chars [" and ","
        s.erase(0, m[1].length() + 2 + 3);
    }
    else {
        throw std::invalid_argument("Description code not found in: " + s);
    }
    return descriptionCode;
}

std::string FormattedLine::findDescription(std::string& s) {
    // The string should at this stage look like:
    // Pantheon [YOLO]","Sgtcuddle",1436182498]test
    std::smatch m;
    std::string description;
    // Match everything from the start of the string until ","
    // Assuming here that no org or character have a name containing ","
    // (which might be untrue)
    if (regex_search(s, m, regex("(^.*?)(?=\",\")"))) {
        description = m[1];
         // Erase the match plus the next three chars (",").
        s.erase(0, m[0].length() + 3);
    }
    else {
        throw std::invalid_argument("Description not found in: " + s);
    }
    return description;
}

std::string FormattedLine::findSender(std::string& s) {
    // Sender is only present in chat messages so it's perfectly fine for it
    // to not be found.
    // The string should at this stage look like:
    // Sgtcuddle",1436182498]test
    std::smatch m;
    std::string sender;
    // Match everything from the start of the string until ",
    if (regex_search(s, m, regex("(^.*?)(?=\",)"))) {
        sender = m[1];
         // Erase the match plus the next two chars (",).
        s.erase(0, m[0].length() + 2);
    }
    return sender;
}

std::string FormattedLine::findTime(std::string& s) {
    // The string should at this stage look like:
    // 1436182498]test
    std::smatch m;
    std::string time;
    // Match everything from the start of the string until ]
    if (regex_search(s, m, regex("(^\\d*?)(?=\\])"))) {
        time = m[1];
         // Erase the match plus ]
        s.erase(0, m[0].length() + 1);
    }
    else {
        // This should not happen unless the line is empty or wrong.
        throw std::invalid_argument("Time not found in: " + s);
    }
    return time;
}
