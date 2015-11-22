#ifndef XP_WRITER_H
#define XP_WRITER_H


#include "definitions.h"
#include "player_vector.h"
#include "writer_helper.h"
#include "xp.h"

#include <ostream>
#include <string>


class Configuration;

class XPWriter : public WriterHelper {
public:
    XPWriter(PlayerVector& playerVector,
             Configuration& config,
             std::ofstream& file);

    // Can only be done for "You".
    void createXPInfo();

private:
    void writeXPHeadingsOverall();
    void writeXPHeadingsDetailed();
    void writeXPTypeName(LineType type);

    void writeXPStatsOverview(const XP& xp, LineType type);
    void writeXPStatsDetailed(const XP& xp, LineType type);

    PlayerVector& playerVector;
};


#endif  //NANO_PROGRAM_WRITER_H
