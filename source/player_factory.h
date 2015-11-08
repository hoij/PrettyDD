#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H


#include "affected_player_factory.h"
#include "affected_player_vector.h"
#include "my_time.h"
#include "player.h"
#include "player_factory_interface.h"

#include <ctime>
#include <memory>
#include <string>


class PlayerFactory : public PlayerFactoryInterface {
public:
    virtual PlayerInterface* createPlayer(std::string name) {
        AffectedPlayerFactoryInterface* affectedPlayerFactory = new AffectedPlayerFactory();
        return new Player(
            name,
            std::make_shared<AffectedPlayerVector>(affectedPlayerFactory),
            std::make_shared<MyTime>());
    }
};


#endif  //PLAYER_FACTORY_H