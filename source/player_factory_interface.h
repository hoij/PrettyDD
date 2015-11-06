#ifndef PLAYER_FACTORY_INTERFACE_H
#define PLAYER_FACTORY_INTERFACE_H


#include <ctime>
#include <string>


class PlayerFactoryInterface {
public:
    virtual ~PlayerFactoryInterface() {}
    virtual PlayerInterface* createPlayer(std::string name) = 0;
};


#endif  // PLAYER_FACTORY_INTERFACE_H