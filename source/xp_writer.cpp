#include "xp_writer.h"

#include <iomanip>
#include <string>
#include <vector>


XPWriter::XPWriter(PlayerVector<Player*>& playerVector,
                   Configuration& config,
                   std::ofstream& file) :
                   WriterHelper(config, file),
                   playerVector(playerVector) {}

void XPWriter::createXPInfo() {

    if (!openFile("pdd1")) {
        return;
    }

    std::string title = "XP Info";

    Player* pp = playerVector.getPlayer("You");
    if (pp == nullptr) {
        createNotFoundMessage(title, "You not found.");
        return;
    }

    const XP& xp = pp->getXp();

    writeStartOfLink(title);

    writeXPHeadingsOverall();

    file << "<font color = " + lime + ">" << nl;
    std::vector<std::string> types = xp.getTypes();
    for (const auto& type : types) {
        writeXPStatsOverview(xp, type);
    }
    file << "</font>";

    writeXPHeadingsDetailed();

    file << "<font color = " + lime + ">" << nl;
    for (const auto& type : types) {
        writeXPStatsDetailed(xp, type);
    }
    file << "</font>";

    writeEndOfLink(title);

    closeFile();
}

/************/
/* Headings */
/************/

void XPWriter::writeXPHeadingsOverall() {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
        "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width+3) << " Total " <<
            std::setw(width) << " XPH " <<
            "</font><br>" << std::setfill(' ') << nl;
}

void XPWriter::writeXPHeadingsDetailed() {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
        "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width + 3) << " Total " <<
            std::setw(width) << " XPH " <<
            std::setw(width + 1) << " Max " <<
            std::setw(width + 1) << " Min " <<
            "</font><br>" << std::setfill(' ') << nl;
}

/***************/
/* Stat writes */
/***************/

void XPWriter::writeXPStatsOverview(const XP& xp, std::string type) {
    const int width = 9;
    file << std::setfill(fillChar) <<
            std::setw(width + 1) << " " + std::to_string(xp.getTotal(type)) <<
                                    " " <<
            std::setw(width) << " " + std::to_string(xp.getXPH(type)) << " " <<
            std::setfill(' ');
    writeName(type);
    file << "<br>" << nl;
}

void XPWriter::writeXPStatsDetailed(const XP& xp, std::string type) {

    int maxGained = (xp.getMaxGained(type) == -1) ?
                     0 : xp.getMaxGained(type);
    int minGained = (xp.getMinGained(type) ==
                     std::numeric_limits<int>::max()) ?
                         0 : xp.getMinGained(type);
    int maxLost = (xp.getMaxLost(type) == -1) ?
                   0 : xp.getMaxLost(type);
    int minLost = (xp.getMinLost(type) ==
                   std::numeric_limits<int>::max()) ?
                       0 : xp.getMinLost(type);

    const int width = 9;
    file << std::setfill(fillChar) <<
            std::setw(width + 1) << " " + std::to_string(xp.getTotalGained(type))
                             << " " <<
            std::setw(width) << " " + std::to_string(xp.getXPHGained(type))
                             << " " <<
            std::setw(width) << " " + std::to_string(maxGained) << " " <<
            std::setw(width) << " " + std::to_string(minGained) << " " <<
            std::setfill(' ');
    writeName(type + " gained");
    file << "<br>" << nl;

    file << std::setfill(fillChar) <<
            std::setw(width + 1) << " " + std::to_string(xp.getTotalLost(type))
                             << " " <<
            std::setw(width) << "" << " " <<
            std::setw(width) << " " + std::to_string(maxLost) << " " <<
            std::setw(width) << " " + std::to_string(minLost) << " " <<
            std::setfill(' ');
    writeName(type + " lost");
    file << "<br>" << nl;
}
