#ifndef ISING_H // Check if ISING_H is not defined
#define ISING_H // Avoiding duplicate definitions

#include <vector>

class IsingModel {
    public:
        int R, C; // Rows and Columns for 2D Ising model
        std::vector<std::vector<int>> spins; // 2D array of spins
        double J; // Interaction strength
    
        IsingModel(int R, int C, double J); // Constructor matching ising.cpp
    
        double computeRowEnergy(); // Computes energy contribution from rows
        double computeColumnEnergy(); // Computes energy contribution from columns
        double computeTotalEnergy(); // Computes total energy
    
        int computeMagnetization(); // Computes magnetization
        void printLattice(); // Prints the spin configuration
    };

#endif
