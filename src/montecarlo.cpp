#include "../include/montecarlo.h"
#include <cmath> // Include for exp()
#include <cstdlib> // For random number generation (rand())
#include <iostream> // For console output

Montecarlo::Montecarlo(double beta) : beta(beta) {}

void Montecarlo::step(IsingModel &model) {
    int R = model.R;  // Number of rows
    int C = model.C;  // Number of columns
    int r = rand() % R; // Pick a random row
    int c = rand() % C; // Pick a random column
     
    // Compute current  energy 
    double current_energy = model.J * model.spins[r][c] * (
        model.spins[r][(c - 1 + C) % C] + // Left neighbor
        model.spins[r][(c + 1) % C] +     // Right neighbor
        model.spins[(r - 1 + R) % R][c] + // Top neighbor
        model.spins[(r + 1) % R][c]       // Bottom neighbor
    );
    // Compute new energy 
    double new_energy = model.J * model.spins[r][c] * -1 * (
        model.spins[r][(c - 1 + C) % C] + // Left neighbor
        model.spins[r][(c + 1) % C] +     // Right neighbor
        model.spins[(r - 1 + R) % R][c] + // Top neighbor
        model.spins[(r + 1) % R][c]       // Bottom neighbor
    );

    // Compute energy change "dE" if this spin is flipped
    double dE = current_energy - new_energy;

    // Condition for flipping the selected spin
    if (dE < 0 || exp(-beta * dE) > ((double)rand() / RAND_MAX)) {
        model.spins[r][c] *= -1;  // Flip the spin
    }
}