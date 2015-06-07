#ifndef LINE_H
#define LINE_H

#include <string>
#include <vector>

struct Line {
	std::string dealer = "";
	std::string receiver = "";
	unsigned int damage = 0;
	std::string type = "";
};

#endif