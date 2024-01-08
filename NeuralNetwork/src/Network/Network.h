#pragma once 

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
	Network(std::vector<int> layers);
	void calc(VectorXd in);
	VectorXd getOut();
private:
	std::vector<MatrixXd>	weights;
	std::vector<VectorXd>	biases;
	VectorXd				out;
	Random					rand;

};