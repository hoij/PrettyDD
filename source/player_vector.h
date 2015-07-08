#ifndef PLAYER_VECTOR_H
#define PLAYER_VECTOR_H


#include <vector>
#include "player.h"
#include "log_line.h"

class Player;

class PlayerVector {
public:
    PlayerVector() {}
    ~PlayerVector() {}
    PlayerVector& operator=(PlayerVector& pv) {
        if (this != &pv) {
            for (Player& p : pv.players) {
                players.push_back(p);
            }
            return *this;
        }
        return *this;
    }

    void createPlayer(std::string name, LogLine& logLine);
    Player* getPlayer(std::string name);
    std::vector<Player>& getPlayers();
    void removePlayer();
    void addToPlayers(LogLine& logLine);

private:
    std::vector<Player> players;
};


#endif // PLAYER_VECTOR_H
