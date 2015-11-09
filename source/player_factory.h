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
    virtual std::unique_ptr<PlayerInterface> createPlayer(std::string name) {
        std::unique_ptr<AffectedPlayerFactoryInterface>
            affectedPlayerFactory(new AffectedPlayerFactory());
        return std::unique_ptr<PlayerInterface>(new Player(
            name,
            std::make_shared<AffectedPlayerVector>(std::move(affectedPlayerFactory)),
            std::make_shared<MyTime>()));
    }
};


#endif  //PLAYER_FACTORY_H