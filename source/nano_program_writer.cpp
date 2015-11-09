#include "configuration.h"
#include "nano_programs.h"
#include "nano_program_writer.h"
#include "player_interface.h"

#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string>


NanoProgramWriter::NanoProgramWriter(PlayerVector& playerVector,
                                     Configuration& config,
                                     std::ofstream& file) :
                                     WriterHelper(config, file),
                                     playerVector(playerVector) {}

void NanoProgramWriter::createCastedDetailedList() {

    std::string titleBase = "Casted Nano Program Info";

    const PlayerInterface* pp = playerVector.getPlayer("You");
    if (pp == nullptr) {
        createNotFoundMessage("No data logged for \"You\"",
                              "Tried to display " + titleBase + " but " +
                              "could not find \\\"You\\\".");
        return;
    }

    const NanoPrograms& nanoPrograms = pp->getNanoPrograms();
    if (nanoPrograms.empty()) {
        createNotFoundMessage("No nano programs logged",
            "Tried to display " + titleBase + " but " +
            "could not find any nano programs for "
            "\\\"You\\\".");
        return;
    }
    std::vector<std::string> v =
        nanoPrograms.getNanoProgramNames();
    sortByExecutes(v, nanoPrograms);

    const unsigned int nanosPerWindow = 5;
    unsigned int nrOfNanos = (unsigned int)nanoPrograms.getNanoProgramNames().size();
    unsigned int nrOfWindows = calcNrOfWindows(nrOfNanos, nanosPerWindow);


    int place = 1;
    const unsigned int windowsPerFile = 4;
    unsigned int nrOfFiles = calcNrOfFiles(nrOfWindows, windowsPerFile);
    for (unsigned int fileNr = 1; fileNr <= nrOfFiles; fileNr++) {

        std::string fileName = "pdd";
        if (fileNr > 1) {
            fileName += std::to_string(fileNr);
        }

        if (!openFile(fileName)) {
            return;
        }

        /* Sets the link name number and calls the write function for
        each link needed. */
        for (unsigned int windowNr = 0; windowNr != windowsPerFile &&
                                        windowNr != nrOfWindows &&
                                        place <= (int)v.size(); windowNr++) {

            int startOffset = windowNr * nanosPerWindow +
                              (fileNr - 1) * nanosPerWindow * windowsPerFile;
            auto start = v.begin() + startOffset;
            auto stop = v.begin();  // Just to set the right type on stop.
            // Stop at either the end or the nr of nanos per file.
            int stopOffset = startOffset + nanosPerWindow;
            if ((int)v.size() < stopOffset) {
                stop = v.end();
            }
            else {
                stop = v.begin() + stopOffset;
            }

            std::string title =
                appendInterval(titleBase, startOffset, stopOffset);

            writeStartOfLink(title);

            writeDetailedListHeadings();

            file << "<font color = " + lime + ">" << nl;
            writeDetailedList(start, stop, nanoPrograms);
            file << "</font>";

            writeEndOfLink(title);

            // To prevent AO's flood protection from kicking in
            file << "/delay 1000" << std::endl;

            place += nanosPerWindow;
        }

        // Write the command to execute the next file
        // (if it exists) at the end of the current file.
        if (fileNr < nrOfFiles) {
            file << "/delay 3000" << std::endl;
            file << "/pdd" << std::to_string(fileNr + 1) << std::endl;
        }

        closeFile();
    }
}

void NanoProgramWriter::writeDetailedListHeadings() {
    int width = 13;
    file << std::setfill(fillChar) << std::right <<
            "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width - 8) << "Casts " <<
            std::setw(width) << " Lands " <<
            std::setw(width+1) << " Resists " <<
            std::setw(width+1) << " Aborts " <<
            std::setw(width+1) << " Counters " <<
            std::setw(width+1) << " Fumbles " <<
            std::setw(width+1) << " FullNCUs " <<
            "</font><br>" << std::setfill(' ') << nl;
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

void NanoProgramWriter::writeDetailedList(
    std::vector<std::string>::iterator namesStart,
    std::vector<std::string>::iterator namesStop,
    const NanoPrograms& nanoPrograms) {

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
        std::string fullNCUPercentage = percentage(nanoPrograms.getExecutes(name),
                                                nanoPrograms.getFullNCUs(name));

        file << std::setfill(fillChar) <<
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
                std::setw(pcWidth) << " " + fullNCUPercentage << "% (" <<
                std::setw(nrWidth) << std::to_string(nanoPrograms.getFullNCUs(name))
                                   << ") " <<
                std::setfill(' ');

        writeName(name);
        file << "<br>" << nl;
    }
}

void NanoProgramWriter::sortByExecutes(std::vector<std::string>& v,
                                       const NanoPrograms& nanoPrograms) {
    std::sort(v.begin(), v.end(),
              [nanoPrograms](std::string name1, std::string name2) {
                     return nanoPrograms.getExecutes(name1) >
                            nanoPrograms.getExecutes(name2);
                 });
}
