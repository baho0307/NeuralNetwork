#include "Population.h"


Population::Population(int count, int lifeTime, std::vector<int> layers, std::string* str)
{
	pop = std::vector<Snake>(count, Snake(layers, lifeTime, 120, 30));
	addr = str;
}

void Population::newGeneration()
{
	std::vector<Snake> newSnakes;

	for (int i = 0; i < pop.size(); i++)
	{
		newSnakes.push_back(pop[0].crossover(pop[i]));
	}
}

void Population::Run()
{
	if (!dead)
	{
		pop[0].Draw(addr);
		dead = pop[0].getDead();
		for (int i = 0; i < pop.size(); i++)
		{
			pop[i].Move();
			dead = dead && pop[i].getDead();
		}
	}
	else
		newGeneration();
}
