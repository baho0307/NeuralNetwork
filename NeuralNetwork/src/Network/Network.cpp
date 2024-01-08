#include "Network.h"

Network::Network(std::vector<int> layers)
{
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

void Network::calc(VectorXd in)
{
    VectorXd r = in;
    for (int i = 0; i < weights.size(); i++)
    {
        r = (weights[i] * r);
    }
    out = r;
}

VectorXd Network::getOut()
{
    std::cout << out << std::endl;
    return out;
}
