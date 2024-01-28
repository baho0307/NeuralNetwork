#pragma once

#include "../Snake/Snake.h"

class Population
{
private:
	std::vector<Snake>	pop;
	std::string*		addr;

	void newGeneration();
	bool dead = false;

public:
	Population(int count, int lifeTime, std::vector<int> layers, std::string *str);

	void Run();
};

