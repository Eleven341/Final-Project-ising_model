#include "ising.h"
#include "metropolis.h"
#include <iostream>
#include <vector>
#include <cmath> // For fabs() to check stabilization

int main()
{
    int L = 100;       // Number of spins
    double J = 1.0;    // Interaction strength
    double beta = 5; // Inverse temperature (1/kT)
    int steps = 100000; // Increased steps to fully observe the behavior

    IsingModel model(L, J);
    Metropolis mc(beta);

    std::cout << "Initial Spin Configuration:\n";
    model.printLattice();
    std::cout << "Initial Energy: " << model.computeEnergy() << std::endl;

    // Track energy and magnetization over time
    std::vector<double> energy_history;
    std::vector<int> magnetization_history;
    bool stabilized = false; // Flag to mark stabilization

    // Monte Carlo Simulation
    for (int i = 0; i < steps; i++)
    {
        mc.step(model);

        // Record energy every 1000 steps
        if (i % 1000 == 0)
        {
            double energy = model.computeEnergy();
            int magnetization = model.computeMagnetization();
            energy_history.push_back(energy);
            magnetization_history.push_back(magnetization);

            std::cout << "Step " << i << " - Energy: " << energy
                      << ", Magnetization: " << magnetization << std::endl;

            /// Check if energy AND magnetization have stabilized over the last 100 steps
            if (energy_history.size() > 500 && !stabilized)
            {
                double avg_energy = 0, avg_mag = 0, max_energy_change = 0;

                for (int j = energy_history.size() - 500; j < energy_history.size(); j++)
                {
                    avg_energy += energy_history[j];
                    avg_mag += magnetization_history[j];

                    // Track maximum energy change in last 100 steps
                    if (j > energy_history.size() - 500 + 1)
                    {
                        double change = fabs(energy_history[j] - energy_history[j - 1]);
                        if (change > max_energy_change)
                        {
                            max_energy_change = change;
                        }
                    }
                }
                avg_energy /= 500;
                avg_mag /= 500;

                // Ensure fluctuations are minimal
                if (max_energy_change < 0.01 && fabs(energy - avg_energy) < 0.001 && fabs(magnetization - avg_mag) < 0.1)
                {
                    std::cout << "System truly stabilized at step " << i << "!\n";
                    stabilized = true; // Mark stabilization but keep running
                }
            }
        }
    }

    std::cout << "Final Spin Configuration:\n";
    model.printLattice();
    std::cout << "Final Energy: " << model.computeEnergy() << std::endl;

    return 0;
}
