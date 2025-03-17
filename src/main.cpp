#include "../include/ising.h"
#include <iostream>

int main() {
    int L = 100;       // Number of spins
    double J = 1.0;   // Interaction strength

    // Create an Ising Model object
    IsingModel model(L, J);

    std::cout << "Initial Spin Configuration:\n";
    model.printLattice();

    std::cout << "Total Energy: " << model.computeEnergy() << std::endl;

    return 0;
}
