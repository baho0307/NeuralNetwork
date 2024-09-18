#include "Population.h"


Population::Population(int count, int lifeTime, std::vector<int> layers, Screen *scr) //create snake population
{
	std::vector<Snake> newSnakes;
	x = scr->get_x();
	y = scr->get_y();
	foods = Food(x, y);

	for (int i = 0; i < count; i++)
	{
		newSnakes.push_back(Snake(layers, &foods, lifeTime, x, y)); //scr size must be variable
	}
	pop = newSnakes;
	addr = scr->generate();
}

void Population::calcGen() //calculate the best snake and replace it to the first indice
{
	double maxScore = pop[0].getFitness();
	int max_indice = 0;
	Snake temp = pop[0];

	for (int i = 0; i < pop.size(); i++)
	{
		if (maxScore < pop[i].getFitness())
		{
			maxScore = pop[i].getFitness();
			max_indice = i;
		}
	}
	
	pop[0] = pop[max_indice];
	pop[max_indice] = temp;
}

void Population::newGeneration()
{
	std::vector<Snake> newSnakes;

	calcGen();
	newSnakes.push_back(Snake(pop[0]));
	for (int i = 1; i < pop.size(); i++)
	{
		newSnakes.push_back(pop[0].crossover(pop[i])); // crossing over all of the snakes with the best gen(best snake)
	}
	pop = newSnakes;
	foods.generate();
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
	{
		newGeneration();
		dead = false;
	}
}
