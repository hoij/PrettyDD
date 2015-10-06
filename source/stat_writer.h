#ifndef STAT_WRITER_H
#define STAT_WRITER_H


#include "configuration.h"
#include "damage_writer.h"
#include "nano_program_writer.h"
#include "player_vector.h"
#include "writer_helper.h"
#include "xp_writer.h"

#include <ostream>
#include <string>


class Damage;
class Player;

class StatWriter : public WriterHelper {
// TODO: Split into smaller classes
public:
    StatWriter(PlayerVector<Player*>& playerVector,
               Configuration& config,
               NanoProgramWriter& nanoProgramWriter,
               DamageWriter& damagWriter,
               XPWriter& xpWriter,
               std::ofstream& file);
    StatWriter& operator=(StatWriter rhs) = delete;

    void createDDTopList();
    void createDDDetailedTopList();
    void createDDPerDamageType(std::string playerName);
    void createDDPerOpponent(std::string playerName);  // Lists all opponents for playerName
    void createDDOnSpecificOpponent(std::string playerName,
                                    std::string opponentName);
    void createDDOnSpecificOpponentDetailed(std::string playerName,
                                            std::string opponentName);

    void createDRTopList();
    void createDRDetailedTopList();
    void createDRPerDamageType(std::string playerName);
    void createDRPerOpponent(std::string playerName);  // Lists all opponents for playerName

    void createHealReceivedTopList();
    void createHealReceivedDetailedTopList();
    void createHealDealtPerReceiver();  // Can only be done for "You".

    void createNanoReceivedTopList();
    void createNanoReceivedDetailedTopList();
    void createNanoDealtPerReceiver();  // Can only be done for "You".

    void createNanoProgramsCastedDetailedList();  // Can only be done for "You".

    void createXPInfo();

    void createHelp();

    // Remove when done:
    void writeDDOld(const Damage& d, std::ostream& os);

private:
    PlayerVector<Player*>& playerVector;
    Configuration& config;
    NanoProgramWriter& nanoProgramWriter;
    DamageWriter& damageWriter;
    XPWriter& xpWriter;
};


#endif  // STAT_WRITER_H
