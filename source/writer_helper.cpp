#include "writer_helper.h"

#include <iomanip>


WriterHelper::WriterHelper(Configuration& config,
                           std::ofstream& file) :
                           config(config),
                           file(file),
                           nl(config) {}

bool WriterHelper::openFile() {
    file.open(config.getScriptsPath() + "pdd");
    if (!file.is_open()) {
        errorLog.write("Error: Could not open/create \"pdd"
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
            title << "</font><br>";
    if (config.shouldWriteReadable()) {
        file << std::endl;
    }
}

void WriterHelper::writeEndOfLink(std::string title) {
    file << "\">" + title << std::endl;
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
                                         unsigned int windowNr,
                                         unsigned int nanosPerWindow) {
    std::string interval = std::to_string(windowNr * nanosPerWindow + 1) +
        "-" + std::to_string((windowNr + 1) * nanosPerWindow);
    return titleBase + " " + interval;
}

void WriterHelper::createNotFoundMessage(std::string title,
                                         std::string message,
                                         std::ostream& os) {

    os << "<a href=\"text://" + message + "<br>" <<
          "\">" + title + "</a>";
}

unsigned int WriterHelper::calcNrOFWindows(unsigned int amount,
                                           unsigned int amountPerWindow) {
    return amount / amountPerWindow + (amount % amountPerWindow != 0);
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
