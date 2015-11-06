#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H


#include "my_time_interface.h"
#include "player_factory_interface.h"

#include <ctime>
#include <string>


class PlayerFactory : public PlayerFactoryInterface {
public:
    virtual PlayerInterface* createPlayer(std::string name) {
        MyTime* myTime = new MyTime();
        return new Player(name, myTime);
    }
};


#endif  //PLAYER_FACTORY_H