#include "writer_helper.h"

#include <iomanip>


WriterHelper::WriterHelper(Configuration& config,
                           std::ofstream& file) :
                           config(config),
                           file(file),
                           nl(config) {}

bool WriterHelper::openFile(std::string fileName) {
    file.open(config.getScriptsPath() + fileName);
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create: \"" + fileName +
                       "\" for writing. Tried writing it to:");
        errorLog.write("Error:\t" + config.getScriptsPath());
        return false;
    }
    return true;
}

void WriterHelper::closeFile() {
    file.close();
}

void WriterHelper::writeStartOfLink(std::string title) {
    file << "<a href=\"text://<font color = " + yellow + ">" <<
            title << "</font><br>" << nl;
}

void WriterHelper::writeEndOfLink(std::string title) {
    file << "\">" << title << "</a>" << std::endl;
}

void WriterHelper::writePlace(int place) {
    file << std::right << std::setw(3) << std::setfill(fillChar) <<
        " " + std::to_string(place) <<
        std::setfill(' ');
}

void WriterHelper::writeName(std::string name) {
    /* Write the name or %m which is the name of the player executing
    the script. */
    file << ((name == "You") ? " %m" : " " + name);
}

std::string WriterHelper::checkIfSelf(std::string name) {
    std::string playerRunningProgram = config.getplayerRunningProgram();
    return (name == playerRunningProgram ? "You" : name);
}

std::string WriterHelper::appendInterval(std::string titleBase,
                                         int startOffset,
                                         int stopOffset) {
    std::string interval = std::to_string(1 + startOffset) +
        "-" + std::to_string(stopOffset);
    return titleBase + " " + interval + ".";
}

void WriterHelper::createNotFoundMessage(std::string title,
                                         std::string message) {
    if (!openFile("pdd1")) {
        return;
    }
    file << "<a href=\"text://" + message + "<br>" <<
            "\">" + title + "</a>";
    closeFile();
}

unsigned int WriterHelper::calcNrOfWindows(unsigned int amount,
                                           unsigned int amountPerWindow) {
    return amount / amountPerWindow + (amount % amountPerWindow != 0);
}

unsigned int WriterHelper::calcNrOfFiles(unsigned int nrOfWindows,
                                         unsigned int windowsPerFile) {
    return nrOfWindows / windowsPerFile + (nrOfWindows % windowsPerFile != 0);
}

std::string WriterHelper::percentage(int total, int part) {
    if (part == 0) {
        return "0.0";
    }
    else {
        return dblToString((double)part * 100 / total);
    }
}

std::string WriterHelper::dblToString(const double d) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(1) << d;
    return out.str();
}
