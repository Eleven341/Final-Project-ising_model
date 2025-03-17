#include "ising.h"
#include "metropolis.h"
#include <iostream>
#include <vector>
#include <cmath> // For fabs()

int main()
{
    int L = 100;        // Number of spins
    double J = 1.0;     // Interaction strength
    int steps = 100000; // Increased steps to fully observe behavior
    double beta = 5.0;
    IsingModel model(L, J);  // Create an Ising Model object with L spins
    Metropolis mc(beta); // Default beta value for stabilization testing

    // Display the initial spin configuration
    std::cout << "Initial Spin Configuration:\n";
    model.printLattice();
    std::cout << "Initial Energy: " << model.computeEnergy() << std::endl;

    // Vectors to track energy and magnetization over time
    std::vector<double> energy_history;
    std::vector<int> magnetization_history;
    bool stabilized = false;

    // Monte Carlo Simulation for stabilization check
    for (int i = 0; i < steps; i++)
    {
        mc.step(model);  // Perform a single Monte Carlo step


        // Record energy and magnetization every 1000 steps
        if (i % 1000 == 0)
        {
            double energy = model.computeEnergy(); // Compute current total energy

            int magnetization = model.computeMagnetization(); // Compute total magnetization

            energy_history.push_back(energy);  // Store energy value

            magnetization_history.push_back(magnetization); // Store magnetization value

            // Print energy and magnetization
            std::cout << "Step " << i << " - Energy: " << energy
                      << ", Magnetization: " << magnetization << std::endl;

            // Check for stabilization (only perform check after we have more than 500 records)
            if (energy_history.size() > 500 && !stabilized)
            {
                double avg_energy = 0, avg_mag = 0, max_energy_change = 0;


                // Calculate the average energy and magnetization over the last 500 recorded steps
                for (int j = energy_history.size() - 500; j < energy_history.size(); j++)
                {
                    avg_energy += energy_history[j];
                    avg_mag += magnetization_history[j];

                    // Compute the maximum energy change between consecutive steps
                    if (j > energy_history.size() - 500 + 1)
                    {
                        double change = fabs(energy_history[j] - energy_history[j - 1]);
                        if (change > max_energy_change)
                        {
                            max_energy_change = change;
                        }
                    }
                }
                // compute energy and magnetization mean
                avg_energy /= 500;
                avg_mag /= 500;


                if (max_energy_change < 0.01 && fabs(energy - avg_energy) < 0.001 && fabs(magnetization - avg_mag) < 0.1)
                {
                    std::cout << "System stabilized at step " << i << "!\n";
                    stabilized = true; // Mark the system as stabilized
                }
            }
        }
    }

    // Print the final spin configuration and energy after all steps
    std::cout << "Final Spin Configuration:\n";
    model.printLattice();
    std::cout << "Final Energy: " << model.computeEnergy() << std::endl;

    // Running simulations for different β values
    std::vector<double> beta_values = {0.1, 0.5, 1.0, 2.5, 5.0, 10.0};
    std::cout << "\nRunning Monte Carlo Simulations for different β values...\n\n";

    for (double beta : beta_values)
    {
        std::cout << "-----------------------------\n";
        std::cout << "beta = " << beta << std::endl;

        IsingModel model(L, J);
        Metropolis mc(beta);

        // Loop over different beta values to analyze their impact on the system
        for (int step = 0; step < 50000; step++)
        {
            mc.step(model);
        }

        double final_energy = model.computeEnergy();
        int final_magnetization = model.computeMagnetization();

        std::cout << "Final Energy: " << final_energy << std::endl;
        std::cout << "Final Magnetization: " << final_magnetization << std::endl;

        // Print the final spin configuration to observe the effect of temperature
        model.printLattice();
        std::cout << "-----------------------------\n";
    }

    return 0;
}
