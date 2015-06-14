#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H


#include <vector>
#include "player.h"

class Player_Manager {
public:
    Player_Manager();
    ~Player_Manager();

    void add_player();
    void remove_player();
    Player& find_player() const;

private:
    std::vector<Player> players;

};


#endif // PLAYER_MANAGER_H
