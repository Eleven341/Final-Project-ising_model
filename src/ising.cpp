#include "../include/ising.h"
#include <iostream>
#include <cstdlib> // For rand()
#include <cmath>   // For fabs()

// Constructor: Initializes a 2D spin lattice of size R x C
IsingModel::IsingModel(int R, int C, double J) : R(R), C(C), J(J) {
    
    // Resize for Higher dimension array (matrix)
    spins.resize(R, std::vector<int>(C));

    // Populate the lattice randomly with +1 or -1
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // Randomly assign +1 or -1
        }
    }
}

// Compute energy due to row interactions
double IsingModel::computeRowEnergy() {
    double rowEnergy = 0.0;
    int i = 0; // Row iterator

    do {
        for (int j = 0; j < C; j++) {
            int S = spins[i][j];
            int right = spins[i][(j + 1) % C]; // Right neighbor
            rowEnergy += -J * S * right;
        }
        i++;
    } while (i < R); // Continue for all rows

    return rowEnergy;
}

// Compute energy due to column interactions
double IsingModel::computeColumnEnergy() {
    double columnEnergy = 0.0;
    int j = 0; // Column iterator 

    do {
        for (int i = 0; i < R; i++) {
            int S = spins[i][j];
            int down = spins[(i + 1) % R][j]; // Bottom neighbor 
            columnEnergy += -J * S * down;
        }
        j++;
    } while (j < C); // Continue for all columns

    return columnEnergy;
}

// Compute total energy by summing row and column energy
double IsingModel::computeTotalEnergy() {
    double rowEnergy = computeRowEnergy();
    double columnEnergy = computeColumnEnergy();
    double totalEnergy = rowEnergy + columnEnergy;

    // DEBUG: Print energy values to compare with 1D version
    std::cout << "[DEBUG] Total Energy: " << totalEnergy 
              << " (Row Energy: " << rowEnergy 
              << ", Column Energy: " << columnEnergy << ")\n";


    return totalEnergy;
}

// Compute total magnetization
int IsingModel::computeMagnetization() {
    int magnetization = 0;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            magnetization += spins[i][j];
        }
    }
    return magnetization;
}

// Print the 2D lattice
void IsingModel::printLattice() {
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            std::cout << (spins[i][j] == 1 ? "+ " : "- ");
        }
        std::cout << std::endl;
    }
}
