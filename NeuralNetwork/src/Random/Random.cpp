#include "Random.h"

double Random::getRand(double start, double end)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(start, end);
	return dis(gen);
}
