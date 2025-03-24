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

double IsingModel::computeRowEnergy() {
    double rowEnergy = 0.0;

    // Here we consider 2 case here is consider we have a 1D model
    if (R == 1) { 
        for (int j = 0; j < C - 1; j++) {
            int S = spins[0][j];
            int right = spins[0][j + 1];
            rowEnergy += -J * S * right;
    
        }
    }
    

    // Here consider 2D model
    else {
        // Standard 2D case
        int i = 0; // Row iterator
        do {
            for (int j = 0; j < C; j++) {
                int S = spins[i][j];
                int right = spins[i][(j + 1) % C]; // Right neighbor (wrap around in 2D)
                rowEnergy += -J * S * right;
            }
            i++;
        } while (i < R); // Continue for all rows
    }

    return rowEnergy;
}

// Same here we consider 2 cases 1D or 2D input
double IsingModel::computeColumnEnergy() {
    double columnEnergy = 0.0;
    
    if (R == 1) {
        // No vertical neighbors in 1D row case
        return 0.0;
    }
    // 1D case
    if (C == 1) { 
        // If there's only one column (1D case), avoid accessing out-of-bounds column indices
        for (int i = 0; i < R - 1; i++) { // Ensure we don't go out of bounds
            int S = spins[i][0];
            int down = spins[i + 1][0]; // Bottom neighbor
            columnEnergy += -J * S * down;
        }
    } 

    // 2D case
    else {
        // Standard 2D case
        int j = 0; // Column iterator
        do {
            for (int i = 0; i < R; i++) {
                int S = spins[i][j];
                int down = spins[(i + 1) % R][j]; // Bottom neighbor (wrap around in 2D)
                columnEnergy += -J * S * down;
            }
            j++;
        } while (j < C); // Continue for all columns
    }

    return columnEnergy;
}


// Compute total energy by summing row and column energy
double IsingModel::computeTotalEnergy() {
    double rowEnergy = computeRowEnergy();
    double columnEnergy = computeColumnEnergy();
    double totalEnergy = rowEnergy + columnEnergy;

  


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
