#ifndef ISING_H // Check if ISING_H is not defined
#define ISING_H // Avoiding duplicate definitions

#include <vector>

class IsingModel {
public:
    int L;                        // Number of spins in the system
    std::vector<int> spins;       // Array of spins (+1 or -1)
    double J;                     // Interaction strength

    // Constructor
    IsingModel(int L, double J);

    // Compute total energy of the system
    double computeEnergy();

    // Display spins
    void printLattice();
};

#endif
