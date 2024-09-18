#pragma once

#include "../Snake/Snake.h"
#include "../Screen/Screen.h"

class Population
{
private:
	std::vector<Snake>	pop;
	std::string*		addr;

	void newGeneration();
	void calcGen();
	bool dead = false;

	Food foods;

	int x;
	int y;

public:
	Population(int count, int lifeTime, std::vector<int> layers, Screen *str);

	void Run();
};

