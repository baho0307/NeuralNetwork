#pragma once

#include<thread>
#include "../Snake/Snake.h"
#include "../Screen/Screen.h"

class Population
{
private:
	std::vector<std::thread> threads;
	std::vector<Snake>	pop;
	std::vector<Snake>	best_pop;
	Screen* scr;
	std::vector<std::string>*		addr;
	Random random;

	void newGeneration();
	void calcGen();
	void generateSnakeSubset(int startIdx, int endIdx, std::vector<Snake>& newSnakes);
	void createSnakeSubset(int startIdx, int endIdx, std::vector<int> layers, int lifetime, std::vector<Snake>& newSnakes);
	void processGroup(int startIdx, int endIdx);
	bool dead = false;

	const int groupSize = 200;

	int x;
	int y;
	int max_popscore = 0;
	double max_fitness = 0;
	double mutationRate = 0.05;

public:
	Population(int count, int lifeTime, std::vector<int> layers, Screen *str);

	void Run(bool draw = true);
};
