#include "../include/montecarlo.h"
#include <cmath>
#include <cstdlib>

Montecarlo::Montecarlo(double beta) : beta(beta) {}

void Montecarlo::step(IsingModel &model) {
    int R = model.R;  // Number of rows
    int L = model.L;  // Number of columns
    int r = rand() % R; // Pick a random row
    int c = rand() % L; // Pick a random column

    // Compute energy change ΔE if this spin is flipped
    double dE = 2 * model.J * model.spins[r][c] * (
        model.spins[r][(c - 1 + L) % L] + // Left neighbor
        model.spins[r][(c + 1) % L] +     // Right neighbor
        model.spins[(r - 1 + R) % R][c] + // Top neighbor
        model.spins[(r + 1) % R][c]       // Bottom neighbor
    );

    // Metropolis acceptance rule
    if (dE < 0 || exp(-beta * dE) > ((double)rand() / RAND_MAX)) {
        model.spins[r][c] *= -1;  // Flip the spin
    }
}