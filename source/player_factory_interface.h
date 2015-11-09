#ifndef PLAYER_FACTORY_INTERFACE_H
#define PLAYER_FACTORY_INTERFACE_H


#include "player_interface.h"

#include <memory>
#include <string>


class PlayerFactoryInterface {
public:
    virtual ~PlayerFactoryInterface() {}
    virtual std::unique_ptr<PlayerInterface> createPlayer(std::string name) = 0;
};


#endif  // PLAYER_FACTORY_INTERFACE_H