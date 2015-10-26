#include "configuration.h"
#include "xp_writer.h"

#include <iomanip>
#include <vector>


XPWriter::XPWriter(PlayerVector<Player*>& playerVector,
                   Configuration& config,
                   std::ofstream& file) :
                   WriterHelper(config, file),
                   playerVector(playerVector) {}

void XPWriter::createXPInfo() {

    if (!openFile("pdd")) {
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

    file << std::setw(width+4) << " Total " <<
            std::setw(width) << " XPH " <<
            "</font><br>" << std::setfill(' ') << nl;
}

void XPWriter::writeXPHeadingsDetailed() {
    int width = 9;
    file << std::setfill(fillChar) << std::right <<
        "<font color = " + lightBlue + ">" << nl;

    file << std::setw(width + 4) << " Total " <<
            std::setw(width + 1) << " XPH " <<
            std::setw(width) << " Max " <<
            std::setw(width + 1) << " Min " <<
            "</font><br>" << std::setfill(' ') << nl;
}

/***************/
/* Stat writes */
/***************/

void XPWriter::writeXPStatsOverview(const XP& xp, std::string type) {
    const int width = 9;
    file << std::setfill(fillChar) <<
            std::setw(width + 2) << " " + std::to_string(xp.getTotal(type)) <<
                                    " " <<
            std::setw(width) << " " + std::to_string(xp.getXPH(type)) << " " <<
            std::setfill(' ');
    writeName(type);
    file << "<br>" << nl;
}

void XPWriter::writeXPStatsDetailed(const XP& xp, std::string type) {

    int totalGained = xp.getTotalGained(type);
    std::string maxGained = determineMax(xp.getMaxGained(type));
    std::string minGained = determineMin(xp.getMinGained(type));
    const int width = 9;
    if (xp.getCountGained(type) > 0) {
        file << std::setfill(fillChar) <<
                std::setw(width + 2) << " " + std::to_string(totalGained)
                                 << " " <<
                std::setw(width) << " " + std::to_string(xp.getXPHGained(type))
                                 << " " <<
                std::setw(width) << " " + maxGained << " " <<
                std::setw(width) << " " + minGained << " " <<
                std::setfill(' ');
        writeName(type + " gained");
        file << "<br>" << nl;
    }

    int totalLost = xp.getTotalLost(type);
    std::string maxLost = determineMax(xp.getMaxLost(type));
    std::string minLost = determineMin(xp.getMinLost(type));
    if (xp.getCountLost(type) > 0) {
        file << std::setfill(fillChar) <<
                std::setw(width + 2) << " " + std::to_string(totalLost)
                                 << " " <<
                std::setw(width) << "" << " " <<
                std::setw(width) << " " + maxLost << " " <<
                std::setw(width) << " " + minLost << " " <<
                std::setfill(' ');
        writeName(type + " lost");
        file << "<br>" << nl;
    }
}
