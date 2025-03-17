#include "../include/ising.h"
#include <iostream>
#include <cstdlib> // Provide rand() funtion

IsingModel::IsingModel(int L, double J) : L(L), J(J) {
    // Initialize spins randomly as either +1 or -1
    spins.resize(L);
    for (int i = 0; i < L; i++) {
        spins[i] = (rand() % 2) * 2 - 1;  // Generates +1 or -1 randomly
    }
}

// Compute the total energy of the system
double IsingModel::computeEnergy() {
    double energy = 0;
    for (int i = 0; i < L - 1; i++) {
        energy += -J * spins[i] * spins[i + 1];  // Neighbor interaction
    }
    return energy;
}

// Print the spin configuration
void IsingModel::printLattice() {
    for (int spin : spins)
        std::cout << (spin == 1 ? "+ " : "- "); // Use + to represent upwards arrow, - for downwards arrow
    std::cout << std::endl;
}
