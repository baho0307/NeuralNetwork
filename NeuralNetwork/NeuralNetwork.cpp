#include <iostream>
#include "src/Neuron/Neuron.h"
#include "src/Network/Network.h"
#include "src/Snake/Snake.h"
#include "src/Population/Population.h"

using namespace Eigen;

int main()
{
    std::mutex mutex;
    Screen scr(30, 30, mutex); //For some reason the height should be higher than 10. Max size is (120, 30)
    Population pop(10000, 240, { 26, 32, 16, 8, 3 }, &scr, mutex);

    bool dKeyPressed = false;
    bool wasDKeyReleased = true;

    std::thread scr_thread(&Screen::Show, &scr);

    while (true)
    {
        pop.Run(dKeyPressed);
    }
}
