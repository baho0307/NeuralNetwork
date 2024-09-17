#pragma once 

#define DEBUG true //if you are using this library for your project please be sure to set the variable to false

#if DEBUG
	#include "../Screen/Screen.h"
#endif

#include "../../Eigen/Dense"
#include "../Random/Random.h"
#include <vector>
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class Network
{
public:
	Network();
	Network(std::vector<MatrixXd> weights, std::vector<VectorXd> biases, VectorXd out);
	Network(std::vector<int> layers);
	Network operator=(const Network& net);

	Network crossover(const Network& net, double mutationRate, double mutationStrength);
	void calc(VectorXd in);
	VectorXd getOut();

#if DEBUG
	Network(std::vector<int> layers, std::string *scr);
	void debug(int x);
	std::string* _scr;
#endif

private:
	std::vector<MatrixXd>	weights;
	std::vector<VectorXd>	biases;
	VectorXd				out;
	Random					rand;

};