# NeuralNetwork

A neural network that learns to play Snake on its own. Weights are not trained with backpropagation but evolved with a **genetic algorithm** (selection + crossover + mutation). Thousands of snakes are simulated in parallel across multiple threads each generation, and the best one is shown live in the console.

Written in C++; matrix operations use [Eigen](https://eigen.tuxfamily.org/) (included in the repo, no separate installation needed).

## Features

- Feed-forward neural network written from scratch on top of Eigen
- Training with a genetic algorithm: elitism, crossover with the best individual, adjustable mutation rate/strength
- Parallel simulation of the population in groups (`groupSize = 200`) using threads
- Live rendering in the Windows console (double buffering, only changed parts are redrawn)
- The snake's "vision": distance to food, its own body, and walls in 8 directions

## How it works

### Inputs (24)
The snake casts rays from its head in 8 directions (horizontal, vertical and diagonals, relative to the direction the snake is facing). Each direction produces 3 values:

| Value | Meaning |
|-------|---------|
| 1 | `1 / distance` if food is in this direction, otherwise 0 |
| 2 | `1 / distance` if its own body is in this direction, otherwise 0 |
| 3 | `1 / distance` to the wall |

8 directions × 3 = **24 inputs**.

### Network architecture
The default layer layout in `main` is `{ 24, 32, 16, 8, 3 }`. ReLU activation is used on all layers. There are **3 outputs** (turn left / go straight / turn right); the output with the highest value is chosen.

### Fitness
Lifetime and score (number of food items eaten) are evaluated together; score is rewarded exponentially (`2^score`). Snakes that completely fill the board are additionally collected in the `best_pop` list.

### Evolution loop
1. All snakes play independently until they die or their lifetime runs out (in parallel using threads).
2. The best snake is moved to the front of the population and kept unchanged (elitism).
3. Every other individual is produced by **crossing over** with the best snake and then mutated.
4. While the score is low, the mutation rate is tripled (more exploration); as performance improves it drops back to the normal rate (`0.05`).

## Project structure

```
NeuralNetwork/
├── NeuralNetwork.sln
└── NeuralNetwork/
    ├── NeuralNetwork.cpp        # Entry point, population and screen thread
    ├── Eigen/                   # Eigen library (header-only)
    └── src/
        ├── Network/             # Feed-forward network, crossover and mutation
        ├── Neuron/              # Simple neuron class
        ├── Snake/               # Snake logic, vision, movement, fitness
        ├── Food/                # Food spawning
        ├── Population/          # Generation management, selection, multithreaded simulation
        ├── Screen/              # Windows console rendering
        └── Random/              # Random number helper
```

## Build and run

**Requirements:** Windows and Visual Studio 2022 (platform toolset `v143`, "Desktop development with C++" workload). The project uses `Windows.h`, so it only builds on Windows.

1. Clone the repo:
   ```bash
   git clone https://github.com/baho0307/NeuralNetwork.git
   ```
2. Open `NeuralNetwork.sln` in Visual Studio.
3. Select the **Release** configuration (x64 recommended); training is very slow in Debug mode.
4. Run with `Ctrl + F5`.

## Configuration

In [`NeuralNetwork.cpp`](NeuralNetwork/NeuralNetwork.cpp):

```cpp
Screen scr(10, 10);
Population pop(2000, 100, { 24, 32, 16, 8, 3 }, &scr);
```

| Parameter | Description |
|-----------|-------------|
| `Screen(10, 10)` | Game area size (max about 120×30 for the console) |
| `2000` | Population size |
| `100` | Initial lifetime of a snake (number of moves) |
| `{24, 32, 16, 8, 3}` | Layer sizes (the first 24 and last 3 must stay fixed) |

The mutation rate can be changed via `mutationRate` in `Population.h`, and the thread group size via `groupSize`.

## Notes

- The `DEBUG` macro in `Network.h` should be left `false`; setting it to `true` enables extra debugging output.
- Training runs in an infinite loop; close the console window to stop it.
