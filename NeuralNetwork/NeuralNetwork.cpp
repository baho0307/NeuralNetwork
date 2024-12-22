#include <iostream>
#include "src/Neuron/Neuron.h"
#include "src/Network/Network.h"
#include "src/Snake/Snake.h"
#include "src/Population/Population.h"

using namespace Eigen;

int main()
{
    Screen scr(10, 10); //For some reason the height should be higher than 10. MAx size is (120, 30)
    Population pop(2000, 100, { 24, 32, 16, 8, 3 }, &scr);

    bool dKeyPressed = false; // Current state of the 'D' key
    bool wasDKeyReleased = true; // Previous state of the 'D' key (initially not pressed)

    std::thread scr_thread(&Screen::Show, &scr);

    while (true)
    {
        //// Check the current state of the 'D' key
        //bool isDKeyPressed = (GetAsyncKeyState('D') & 0x8001) != 0; // Check if the 'D' key is pressed

        //// If the key is not pressed and was previously released
        //if (!isDKeyPressed && wasDKeyReleased)
        //{
        //    dKeyPressed = !dKeyPressed; // Toggle the dKeyPressed state (flip-flop)
        //    wasDKeyReleased = false; // Mark as no longer released
        //}
        //else if (isDKeyPressed)
        //{
        //    wasDKeyReleased = true; // Mark as released when the key is pressed
        //}

        //// Call pop.Run(), which will draw based on dKeyPressed
        pop.Run(dKeyPressed);
    }
}
