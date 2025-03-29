#ifndef ISING_H // Check if ISING_H is not defined
#define ISING_H // Avoiding duplicate definitions
#include <vector> // For vector manipulation

class IsingModel {
    public: // Accessible by external code
        std::vector<std::vector<int>> spins; // 2D/1D array of spins
        
    
        IsingModel(int R, int C, double J); // Constructor matching ising.cpp
    
        double computeRowEnergy(); // Computes energy contribution from rows
        double computeColumnEnergy(); // Computes energy contribution from columns
        double computeTotalEnergy(); // Computes total energy
    
        int computeMagnetization(); // Computes magnetization
        void printLattice(); // Prints the spin configuration

        friend class Montecarlo; // Only gives special permission to montecarlo class to access private member

    private: // Hidden from external access
        int R, C; // Number of rows and columns
        double J; // Interaction strength
    };

#endif
