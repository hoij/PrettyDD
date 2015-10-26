#ifndef XP_WRITER_H
#define XP_WRITER_H


#include "player_vector.h"
#include "writer_helper.h"
#include "xp.h"

#include <ostream>
#include <string>


class Configuration;

class XPWriter : public WriterHelper {
public:
    XPWriter(PlayerVector<Player*>& playerVector,
             Configuration& config,
             std::ofstream& file);

    // Can only be done for "You".
    void createXPInfo();

private:
    void writeXPHeadingsOverall();
    void writeXPHeadingsDetailed();

    void writeXPStatsOverview(const XP& xp, std::string type);
    void writeXPStatsDetailed(const XP& xp, std::string type);

    PlayerVector<Player*>& playerVector;
};


#endif  //NANO_PROGRAM_WRITER_H
