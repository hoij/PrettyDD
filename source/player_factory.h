#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H


#include "my_time_interface.h"
#include "player_factory_interface.h"

#include <ctime>
#include <string>


class PlayerFactory : public PlayerFactoryInterface {
public:
    virtual PlayerInterface* createPlayer(std::string name) {
        return new Player(name, std::make_shared<MyTime>());
    }
};


#endif  //PLAYER_FACTORY_H