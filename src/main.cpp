#include "ising.h"
#include "montecarlo.h"
#include <iostream>
#include <vector>
#include <cmath> // For fabs()
#include <cfloat> // For double limits

int main()
{
    int R, C;
    double J = 1.0, beta;
    int steps = 100000;

    // Ask user for input grid size
    std::cout << "Enter number of rows (R): ";
    std::cin >> R;
    std::cout << "Enter number of columns (C): ";
    std::cin >> C;
    std::cout << "Enter beta value: ";
    std::cin >> beta;

    // Initialize model with user-specified size
    IsingModel model(R, C, J);
    Montecarlo mc(beta);


    std::cout << "Initial Spin Configuration:\n";
    model.printLattice();
    std::cout << "Initial Energy: " << model.computeTotalEnergy() << std::endl;

    // Vectors to track energy and magnetization over time
    std::vector<double> energy_history;
    std::vector<int> magnetization_history;
    bool stabilized = false;  // Track stabilization

    // Monte Carlo Simulation for stabilization check
    for (int i = 0; i < steps; i++)
    {
        mc.step(model);  // Perform a single Monte Carlo step

        // Record energy and magnetization every 1000 steps
        if (i % 1000 == 0)
        {
            double energy = model.computeTotalEnergy();
            int magnetization = model.computeMagnetization();

            energy_history.push_back(energy);
            magnetization_history.push_back(magnetization);

            std::cout << "Step " << i << " - Energy: " << energy
                      << ", Magnetization: " << magnetization << std::endl;

            // Check for stabilization (after 500 recorded values)
            if (energy_history.size() > 500 && !stabilized)
            {
                double avg_energy = 0, avg_mag = 0, max_energy_change = 0;

                // Compute average energy/magnetization over last 500 steps
                for (int j = energy_history.size() - 500; j < energy_history.size(); j++)
                {
                    avg_energy += energy_history[j];
                    avg_mag += magnetization_history[j];

                    // Compute the max change in energy
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

                // Stabilization criteria
                if (max_energy_change < 0.1 && fabs(energy - avg_energy) < 0.01)
                {
                    std::cout << "System stabilized at step " << i << "!\n";
                    stabilized = true; // Mark the system as stabilized
                }
            }
        }
    }

    // Print final results
    std::cout << "Final Spin Configuration:\n";
    model.printLattice();
    std::cout << "Final Energy: " << model.computeTotalEnergy() << std::endl;

    // Running simulations for different beta values
    std::vector<double> beta_values = {0.1, 0.5, 1.0, 2.5, 5.0, 10.0};
    std::cout << "\nRunning Monte Carlo Simulations for different beta values...\n\n";

    for (double beta : beta_values)
    {
        std::cout << "-----------------------------\n";
        std::cout << "beta = " << beta << std::endl;

        IsingModel model(R, C, J);
        Montecarlo mc(beta);

        bool stabilized = false;
        std::vector<double> energy_history;
        std::vector<int> magnetization_history;

        int stabilize_step = -1;

        // Loop for each beta value
        for (int step = 0; step < 50000; step++)
        {
            mc.step(model);

            // Record every 1000 steps
            if (step % 1000 == 0)
            {
                double energy = model.computeTotalEnergy();
                int magnetization = model.computeMagnetization();

                energy_history.push_back(energy);
                magnetization_history.push_back(magnetization);

                // Stabilization check
                if (energy_history.size() > 500 && !stabilized)
                {
                    double avg_energy = 0, avg_mag = 0, max_energy_change = 0;

                    for (int j = energy_history.size() - 500; j < energy_history.size(); j++)
                    {
                        avg_energy += energy_history[j];
                        avg_mag += magnetization_history[j];

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

                    if (max_energy_change < 0.01 && fabs(energy - avg_energy) < 0.001 && fabs(magnetization - avg_mag) < 0.1)
                    {
                        std::cout << "System stabilized for beta = " << beta << " at step " << step << "!\n";
                        stabilized = true;
                        stabilize_step = step;
                    }
                }
            }
        }

        double final_energy = model.computeTotalEnergy();
        int final_magnetization = model.computeMagnetization();

        std::cout << "Final Energy: " << final_energy << std::endl;
        std::cout << "Final Magnetization: " << final_magnetization << std::endl;
        if (stabilize_step != -1)
            std::cout << "Stabilized at step: " << stabilize_step << std::endl;

        model.printLattice();
        std::cout << "-----------------------------\n";
    }

    return 0;
}
