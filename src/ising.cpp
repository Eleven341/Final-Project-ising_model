#include "../include/ising.h" // include header file
#include <iostream>
#include <cstdlib> // For rand()
#include <cmath>   // For fabs()

// Constructor: Initializes a 2D/1D spin lattice of size R x C
IsingModel::IsingModel(int R, int C, double J) : R(R), C(C), J(J) {
    
    // Resize for both low dimension array (vector) or higher dimension array(matrix)
    spins.resize(R, std::vector<int>(C));

    // Construct lattice randomly with +1 or -1
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // Randomly assign +1 or -1
        }
    }
}

// Computes row-wise interaction energy
// If 1D then this function could just compute the total energy of the vector
// If 2D then this function sweep through every row of the matrix and compute enery for each row
double IsingModel::computeRowEnergy() {
    double rowEnergy = 0.0;

    // This condition check if we have 1D or 2D configuration
    if (R == 1) { 
        for (int j = 0; j < C - 1; j++) {
            int S = spins[0][j];
            int right = spins[0][j + 1];
            rowEnergy += -J * S * right;
    
        }
    }
    

    // Here consider 2D model
    else {

        int i = 0; // Row iterator
        do {
            for (int j = 0; j < C; j++) {
                int S = spins[i][j];
                int right = spins[i][(j + 1) % C]; // Right neighbor
                rowEnergy += -J * S * right;
            }
            i++;
        } while (i < R); // Continue for all rows (Also prevent out of bound of the matrix)
    }

    return rowEnergy;
}

// Compute column wise interaction energy
// If 1D then this function return 0 value
// If 2D then this function sweep through every column of the matrix and compute enery for each column
double IsingModel::computeColumnEnergy() {
    double columnEnergy = 0.0;
    
    if (R == 1) {

        // No vertical neighbors in 1D row case
        return 0.0;
    }
    // Consider case when we have several rows and just 1 column
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
       
        int j = 0; // Column iterator
        do {
            for (int i = 0; i < R; i++) {
                int S = spins[i][j];
                int down = spins[(i + 1) % R][j]; // Bottom neighbor (wrap around in 2D)
                columnEnergy += -J * S * down;
            }
            j++;
        } while (j < C); // Continue for all columns (Prevent out of bound of the matrix)
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

// Print the spin configuration
void IsingModel::printLattice() {
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            std::cout << (spins[i][j] == 1 ? "+ " : "- "); // Print + for spin up (+1), - for spin down (-1)
        }
        std::cout << std::endl;
    }
}
