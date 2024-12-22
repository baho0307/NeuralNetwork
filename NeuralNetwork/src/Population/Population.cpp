#include "Population.h"


Population::Population(int count, int lifeTime, std::vector<int> layers, Screen *scr) //create snake population
{
	std::vector<Snake> newSnakes(count);
	x = scr->get_x();
	y = scr->get_y();
    // Create threads to manage different groups of snakes
    const int batchSize = groupSize;
    int numGroups = (count) / batchSize + ((count) % batchSize != 0 ? 1 : 0);

    // Launch threads to create each batch
    std::vector<std::thread> threads;
    for (int g = 0; g < numGroups; ++g) {
        int startIdx =  g * batchSize; 
        int endIdx = min(startIdx + batchSize, static_cast<int>(count));
        threads.push_back(std::thread(&Population::createSnakeSubset, this, startIdx, endIdx, layers, lifeTime, std::ref(newSnakes)));
    }
    // Wait for all threads to finish
    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }
    threads.clear();
	pop = newSnakes;
	addr = scr->generate();
    this->scr = scr;
}

void Population::calcGen() //calculate the best snake and replace it to the first indice
{
	int maxIndice = 0;
	for (int i = 0; i < pop.size(); i++)
	{
		if (pop[i].getFitness() > pop[maxIndice].getFitness())
			maxIndice = i;
        if (pop[i].getScore() > max_popscore)
            max_popscore = pop[i].getScore();
        if (pop[i].getScore() == (x - 2) * (y - 2) - 1 && pop.size() > best_pop.size())
            best_pop.push_back(Snake(pop[i]));
	}
    std::swap(pop[0], pop[maxIndice]);
}

void Population::generateSnakeSubset(int startIdx, int endIdx, std::vector<Snake>& newSnakes) {
    for (int i = startIdx; i < endIdx; ++i) 
    {
        newSnakes[i] = pop[0].crossover(pop[i], max_popscore > 2 * (x + y) ? mutationRate : mutationRate * 3, 0.5); // crossing over with the best snake
    }
}

void Population::createSnakeSubset(int startIdx, int endIdx, std::vector<int> layers, int lifeTime, std::vector<Snake>& newSnakes) {
    for (int i = startIdx; i < endIdx; ++i) 
    {
        newSnakes[i] = Snake(layers, lifeTime, x, y);
    }
}

void Population::newGeneration() 
{
    if (best_pop.size() == pop.size())
	{
		pop = best_pop;
		best_pop.clear();
		return;
	}
    calcGen(); // Ensure the best snake is first in the population

    // Create a new vector for the new generation of snakes
    std::vector<Snake> newSnakes(pop.size());
    newSnakes[0] = Snake(pop[0]); // Keep the best snake

    // Define the fixed batch size and calculate the number of groups
    const int batchSize = groupSize;
    int numGroups = (pop.size() - 1) / batchSize + ((pop.size() - 1) % batchSize != 0 ? 1 : 0);

    // Launch threads to perform crossover for each batch
    std::vector<std::thread> threads;
    for (int g = 0; g < numGroups; ++g) {
        int startIdx = 1 + g * batchSize; // Skip the first snake
        int endIdx = min(startIdx + batchSize, static_cast<int>(pop.size()));
        threads.push_back(std::thread(&Population::generateSnakeSubset, this, startIdx, endIdx, std::ref(newSnakes)));
    }
    // Wait for all threads to finish
    for (auto& t : threads) {
        if (t.joinable())
            t.join();
    }
    threads.clear();

    pop = std::move(newSnakes); // Replace the old population with the new one
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
                pop[0].Draw(addr, max_popscore);
            }
        }
    } while (!dead_g);

 }  

void Population::Run(bool draw)
{
	if (!dead) {
        // Draw the first snake (the best one)
        pop[0].Draw(addr, max_popscore);
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
            if (t.joinable()) 
                t.join();
        }
        threads.clear();

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
