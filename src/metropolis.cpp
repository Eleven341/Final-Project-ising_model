#include "../include/metropolis.h"
#include <cmath>

Metropolis::Metropolis(double beta) : beta(beta) {}

void Metropolis::step(IsingModel &model) {
    int L = model.L;  
    int i = rand() % L;  // Pick a random spin

    // Compute energy change if this spin is flipped
    double dE = 2 * model.J * model.spins[i] * 
                (model.spins[(i - 1 + L) % L] + model.spins[(i + 1) % L]);

    // Metropolis acceptance rule
    if (dE < 0 || exp(-beta * dE) > ((double)rand() / RAND_MAX)) {
        model.spins[i] *= -1;  // Flip the spin
    }
}

