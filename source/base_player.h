#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H


#include "line_info.h"

#include <string>


class BasePlayer {
public:
    BasePlayer(std::string name) {this->name = name;}
    virtual ~BasePlayer() {}
    std::string getName() const {return name;}
    void setName(std::string name) {this->name = name;}
    virtual void add(LineInfo& lineInfo) {(void)lineInfo;}

private:
    std::string name;
};


#endif  // BASE_PLAYER_H
