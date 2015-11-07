#ifndef AFFECTED_PLAYER_FACTORY_INTERFACE_H
#define AFFECTED_PLAYER_FACTORY_INTERFACE_H


#include "affected_player_interface.h"

#include <string>


class AffectedPlayerFactoryInterface {
public:
    virtual ~AffectedPlayerFactoryInterface() {}
    virtual AffectedPlayerInterface* createPlayer(std::string name) = 0;
};


#endif  // AFFECTED_PLAYER_FACTORY_INTERFACE_H