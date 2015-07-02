#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H


#include <vector>
#include "player.h"
#include "log_line.h"

class Player;

class PlayerManager {
public:
    PlayerManager() {}
    ~PlayerManager() {}
    PlayerManager& operator=(PlayerManager& pm) {
        for (Player& p : pm.players) {
            players.push_back(p);
        }
        return *this;
    }

    void createPlayer(std::string name, LogLine& logLine);
    Player* getPlayer(std::string name);
    void removePlayer();
    void addToPlayers(LogLine& logLine);

private:
    std::vector<Player> players;
};


#endif // PLAYER_MANAGER_H
