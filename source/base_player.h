#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H

#include <string>

class BasePlayer {
public:
    BasePlayer() {}
    BasePlayer(std::string name) {
        this->name = name;
    }
    virtual ~BasePlayer() {}
    std::string getName() const {
        return name;
    }
    void setName(std::string name) {
        this->name = name;
    }

private:
    std::string name;

};


#endif  // BASE_PLAYER_H
