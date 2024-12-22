#include "Network.h"

Network::Network()
{

}

Network::Network(std::vector<MatrixXd> weights, std::vector<VectorXd> biases, VectorXd out)
{
    input_size = weights[0].cols();
    this->weights = weights;
    this->biases = biases;
    this->out = out;
}

Network::Network(std::vector<int> layers)
{
    input_size = layers[0];
    for (int i = 1; i < layers.size(); i++)
    {
        weights.push_back(MatrixXd(layers[i], layers[i - 1]));
        biases.push_back(VectorXd(layers[i]));
        for (int j = 0; j < layers[i]; j++)
        {
            (biases[i - 1])(j) = rand.getRand(-1, 1);
            for (int k = 0; k < layers[i - 1]; k++)
                (weights[i - 1])(j, k) = rand.getRand(-1, 1);
        }
    }
}

Network Network::operator=(const Network& net)
{
   	weights = net.weights;
	biases = net.biases;
    out = net.out;
    input_size = net.input_size;
    return Network(weights, biases, out);
}

static std::string toString(const Eigen::MatrixXd& mat) {
    std::stringstream ss;
    ss << mat;
    return ss.str();
}

static Eigen::VectorXd sigmoid(const Eigen::VectorXd& input) {
    return (2.0 / (1.0 + (-input.array()).exp())) - 1;
}

static Eigen::VectorXd relu(const Eigen::VectorXd& input) {
    return input.cwiseMax(0.0);
}

#include <random>

Network Network::crossover(const Network& net, double mutationRate, double mutationStrength)
{
    std::vector<MatrixXd> newW;
    std::vector<VectorXd> newB;

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);  // For random selection (crossover and mutation)
    std::normal_distribution<> mutationNoise(0.0, mutationStrength);  // For mutation noise (Gaussian)

    for (int i = 0; i < weights.size(); i++)
    {
        MatrixXd childW = MatrixXd::Zero(weights[i].rows(), weights[i].cols());
        VectorXd childB = VectorXd::Zero(biases[i].size());

        // Randomly select weights from either this network or the other network (net), and mutate some weights
        for (int r = 0; r < weights[i].rows(); r++)
        {
            for (int c = 0; c < weights[i].cols(); c++)
            {
                // Perform crossover
                if (dis(gen) < 0.5)
                {
                    childW(r, c) = weights[i](r, c);  // Take weight from this network
                }
                else
                {
                    childW(r, c) = net.weights[i](r, c);  // Take weight from net
                }

                // Apply mutation with the specified mutation rate
                if (dis(gen) < mutationRate)
                {
                    childW(r, c) += mutationNoise(gen);  // Add small random noise to mutate the weight
                }
            }
        }

        // Randomly select biases from either this network or the other network, and mutate some biases
        for (int j = 0; j < biases[i].size(); j++)
        {
            // Perform crossover
            if (dis(gen) < 0.5)
            {
                childB(j) = biases[i](j);  // Take bias from this network
            }
            else
            {
                childB(j) = net.biases[i](j);  // Take bias from net
            }

            // Apply mutation with the specified mutation rate
            if (dis(gen) < mutationRate)
            {
                childB(j) += mutationNoise(gen);  // Add small random noise to mutate the bias
            }
        }

        newW.push_back(childW);
        newB.push_back(childB);
    }

    return Network(newW, newB, out);
}

void Network::calc(VectorXd in)
{
    VectorXd r = in;
    for (int i = 0; i < weights.size(); i++)
    {
        r = relu((weights[i] * r) + biases[i]); // Relu or Sigmoid
    }
    out = r;
}

VectorXd Network::getOut()
{
    return out;
}

int Network::getInput()
{
    return input_size;
}

#if DEBUG

Network::Network(std::vector<int> layers, std::string* scr) : Network(layers)
{
    _scr = scr;
}

void Network::debug(int x)
{
    if (!_scr)
        std::cout << "PLEASE CREATE THE OBJECT WITH DEBUG CONSTRUCTOR" << std::endl;
    else
    {
        _scr->clear();
        *_scr += toString(weights[x % weights.size()]); //+ ((i + x) % weights.size() < weights.size() - 1 ? " : hl" + (i + x) % weights.size() : " : ol") + " | ";
    }
}

#endif
