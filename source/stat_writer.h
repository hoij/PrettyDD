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
    void createDRTopList();
    void createDDDetailedTopList();
    void createDRDetailedTopList();
    void createDDPerType(std::string playerName);
    void createDRPerType(std::string playerName);
    void createDDPerTypeDetailed(std::string playerName);
    void createDRPerTypeDetailed(std::string playerName);
    void createDDPerOpponent(std::string playerName);
    void createDRPerOpponent(std::string playerName);
    void createDamagePerType(std::string playerName,
                             std::string opponentName);
    void createDamagePerTypeDetailed(std::string playerName,
                                     std::string opponentName);

    void createHealReceivedTopList();
    void createHealReceivedDetailedTopList();
    void createHealDealtPerReceiver();  // Can only be done for "You".

    void createNanoReceivedTopList();
    void createNanoReceivedDetailedTopList();
    void createNanoDealtPerReceiver();  // Can only be done for "You".

    void createNanoProgramsCastedDetailedList();  // Can only be done for "You".

    void createXPInfo();

    void createHelp();

private:
    void createCommands1();
    void createCommands2();
    void createInfo1();
    void createInfo2();
    void writeCommand(std::string command, std::string description = "");

    PlayerVector<Player*>& playerVector;
    NanoProgramWriter& nanoProgramWriter;
    DamageWriter& damageWriter;
    XPWriter& xpWriter;
};


#endif  // STAT_WRITER_H
