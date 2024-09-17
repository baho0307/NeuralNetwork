#pragma once

#include "../Snake/Snake.h"

class Population
{
private:
	std::vector<Snake>	pop;
	std::string*		addr;

	void newGeneration();
	void calcGen();
	bool dead = false;

	Food foods;

public:
	Population(int count, int lifeTime, std::vector<int> layers, std::string *str);

	void Run();
};

