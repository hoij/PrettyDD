#ifndef PLAYER_H
#define PLAYER_H


#include <string>
#include <vector>
#include <map>
#include "line_info.h"
#include "player_vector.h"
#include "stats.h"


class Player {
public:
	Player();
	Player(std::string name);  // Is this used? Remove otherwise
	Player(std::string name, LogLine& logLine);
	~Player();
	Player(const Player& p);
	Player& operator=(Player& p);
	//Player& operator+=(const Player& p);

    std::string& get_name();
    void set_name(std::string name);
    void add(LogLine& logLine);

    Stats stats;

private:
    std::string name;
};


#endif
