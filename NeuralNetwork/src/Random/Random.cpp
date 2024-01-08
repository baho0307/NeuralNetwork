#include "Random.h"

Random::Random() : gen(rd())
{
	
}

double Random::getRand(double start, double end)
{
	std::uniform_real_distribution<> dis(start, end);
	return dis(gen);
}
