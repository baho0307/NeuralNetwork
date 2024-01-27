#pragma once

#include "../Snake/Snake.h"

class Population
{
private:
	std::vector<Snake> pop;


public:
	Population(int count, int lifeTime, std::vector<int> layers);

};

