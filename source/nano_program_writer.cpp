#include "nano_program_writer.h"

#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string>


void NanoProgramWriter::createCastedDetailedList(
    const NanoPrograms& nanoPrograms) {

    std::string titleBase = "Casted Nano Program Info";

    std::vector<std::string> nanoProgramNames =
        nanoPrograms.getNanoProgramNames();
    sortByExecutes(nanoProgramNames, nanoPrograms);

    const unsigned int nanosPerWindow = 5;
    unsigned int nrOfNanos = (unsigned int)nanoPrograms.getNanoProgramNames().size();
    unsigned int nrOfWindows = calcNrOFWindows(nrOfNanos, nanosPerWindow);

    for (unsigned int windowNr = 0; windowNr != nrOfWindows; windowNr++) {
        std::string title =
            appendInterval(titleBase, windowNr, nanosPerWindow);

        writeStartOfLink(title);

        writeDetailedListHeadings(file);

        file << "<font color = " + lime + ">";
        if (config.shouldWriteReadable()) {
            file << std::endl;
        }

        auto namesStart = nanoProgramNames.begin() + windowNr * nanosPerWindow;
        auto namesStop = getStopIter(nanoProgramNames, windowNr, nanosPerWindow);
        writeDetailedList(namesStart, namesStop, nanoPrograms, file);

        writeEndOfLink(title);
    }
}

std::ostream& NanoProgramWriter::writeDetailedListHeadings(std::ostream& os) {
    int width = 13;
    os << std::setfill(fillChar) << std::right <<
          "<font color = " + lightBlue + ">";
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    os << std::setw(width - 8) << "Casts " <<
          std::setw(width) << " Lands " <<
          std::setw(width) << " Resists " <<
          std::setw(width) << " Aborts " <<
          std::setw(width) << " Counters " <<
          std::setw(width) << " Fumbles " <<
          "</font><br>" << std::setfill(' ');
    if (config.shouldWriteReadable()) {
        os << std::endl;
    }
    return os;
}

std::vector<std::string>::iterator NanoProgramWriter::getStopIter(
        std::vector<std::string>& nanoProgramNames,
        unsigned int windowNr,
        unsigned int nanosPerWindow) {

    std::vector<std::string>::iterator stop;
    // Stop at either the end or the nr of types per file.
    if (nanoProgramNames.size() < (windowNr + 1) * nanosPerWindow) {
        stop = nanoProgramNames.end();
    }
    else {
        stop = nanoProgramNames.begin() + (windowNr + 1) * nanosPerWindow;
    }
    return stop;
}

std::ostream& NanoProgramWriter::writeDetailedList(
    std::vector<std::string>::iterator namesStart,
    std::vector<std::string>::iterator namesStop,
    const NanoPrograms& nanoPrograms,
    std::ostream& os) {

    const int pcWidth = 6;
    const int nrWidth = 3;
    for (auto it = namesStart; it != namesStop; it++) {
        std::string name = *it;

        std::string landPercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getLands(name));
        std::string resistPercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getResists(name));
        std::string abortPercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getAborts(name));
        std::string counterPercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getCounters(name));
        std::string fumblePercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getFumbles(name));

        os << std::setfill(fillChar) <<
              std::setw(nrWidth + 1) << " " +
                                 std::to_string(nanoPrograms.getExecutes(name))
                                 << " " <<
              std::setw(pcWidth) << " " + landPercentage << "% (" <<
              std::setw(nrWidth) << std::to_string(nanoPrograms.getLands(name))
                                 << ") " <<
              std::setw(pcWidth) << " " + resistPercentage << "% (" <<
              std::setw(nrWidth) << std::to_string(nanoPrograms.getResists(name))
                                 << ") " <<
              std::setw(pcWidth) << " " + abortPercentage << "% (" <<
              std::setw(nrWidth) << std::to_string(nanoPrograms.getAborts(name))
                                 << ") " <<
              std::setw(pcWidth) << " " + counterPercentage << "% (" <<
              std::setw(nrWidth) << std::to_string(nanoPrograms.getCounters(name))
                                 << ") " <<
              std::setw(pcWidth) << " " + fumblePercentage << "% (" <<
              std::setw(nrWidth) << std::to_string(nanoPrograms.getFumbles(name))
                                 << ") " <<
              std::setfill(' ');

        writeName(name);
        os << "<br>";
        if (config.shouldWriteReadable()) {
            os << std::endl;
        }

    }

    return os;
}

void NanoProgramWriter::sortByExecutes(std::vector<std::string>& v,
                                       const NanoPrograms& nanoPrograms) {
    std::sort(v.begin(), v.end(),
              [nanoPrograms](std::string name1, std::string name2) {
                     return nanoPrograms.getExecutes(name1) >
                            nanoPrograms.getExecutes(name2);
                 });
}
