#pragma once

#include<thread>
#include "../Snake/Snake.h"
#include "../Screen/Screen.h"

class Population
{
private:
	std::vector<std::thread> threads;
	std::vector<Snake>	pop;
	Screen* scr;
	std::string*		addr;

	void newGeneration();
	void calcGen();
	void processGroup(int startIdx, int endIdx);
	bool dead = false;

	const int groupSize = 200;

	int x;
	int y;
	int max_popscore = 0;

public:
	Population(int count, int lifeTime, std::vector<int> layers, Screen *str);

	void Run();
};

