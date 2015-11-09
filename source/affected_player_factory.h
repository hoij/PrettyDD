#ifndef AFFECTED_PLAYER_FACTORY_H
#define AFFECTED_PLAYER_FACTORY_H


#include "affected_player.h"
#include "my_time.h"
#include "affected_player_factory_interface.h"

#include <ctime>
#include <string>


class AffectedPlayerFactory : public AffectedPlayerFactoryInterface {
public:
    virtual std::shared_ptr<AffectedPlayerInterface> createPlayer(std::string name) {
        return std::make_shared<AffectedPlayer>(name, std::make_shared<MyTime>());
    }
};


#endif  //AFFECTED_PLAYER_FACTORY_H