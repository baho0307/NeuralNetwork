#include "Population.h"


Population::Population(int count, int lifeTime, std::vector<int> layers, Screen *scr) //create snake population
{
	std::vector<Snake> newSnakes;
	x = scr->get_x();
	y = scr->get_y();
	foods = Food(x, y);
    // Create threads to manage different groups of snakes
	for (int i = 0; i < count; i++)
	{
		newSnakes.push_back(Snake(layers, &foods, lifeTime, x, y)); //scr size must be variable
	}
	pop = newSnakes;
	addr = scr->generate();
    this->scr = scr;
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

void Population::processGroup(int startIdx, int endIdx) 
{
    bool dead_g;
    do 
    {
        dead_g = pop[0].getDead();
        for (int i = startIdx; i < endIdx; ++i) 
        {
            dead_g = dead_g && pop[i].getDead();
            pop[i].Move();
            if (i == 0)
            {
                pop[0].Draw(addr);
                scr->Show(); // do the score and food thing
            }
        }
    } while (!dead_g);
}  

void Population::Run()
{
	if (!dead) {
        // Draw the first snake (the best one)
        pop[0].Draw(addr);
        dead = pop[0].getDead();
        // Function to process a subset of snakes

        // Calculate how many threads we need
        int numGroups = pop.size() / groupSize;
        if (pop.size() % groupSize != 0) {
            numGroups++;
        }

        // Launch threads for each group
        for (int g = 0; g < numGroups; ++g) {
            int startIdx = g * groupSize;
            int endIdx = min(startIdx + groupSize, (int)pop.size());
            threads.push_back(std::thread(&Population::processGroup, this, startIdx, endIdx));
        }

        // Wait for all threads to finish
        for (auto& t : threads) {
            if (t.joinable()) {  // Ensure the thread is joinable before calling join
                t.join();
            }
        }

        // Check if all snakes are dead
        for (int i = 0; i < pop.size(); i++) {
            dead = dead && pop[i].getDead();
        }
    }
    else {
        // If all snakes are dead, generate a new population
        newGeneration();
        dead = false;
    }
}
