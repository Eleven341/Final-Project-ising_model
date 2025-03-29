// Include both header files
#include "ising.h" // Include header file
#include "montecarlo.h" // Include header file
#include <iostream> // For std::cout
#include <vector>  // For using std::vector container
#include <cmath>   // Include for fabs()
#include <cfloat>  // For double limits
#include <fstream> // For file operations (CSV files)
#include <ctime>   // For random seed to follow current time (prevent same random stuff everytime)
#include <numeric> // Used to compute sum
#include <sstream> // Used to build filenames like "frame_1.csv" (GIF)
#include <string>  // Used to create output path
#include <filesystem> // For creating directories and managing files


// Compute average
double compute_mean(const std::vector<double> &values)
{
    // Accumulate the sum of all elements in the vector
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size(); // Divide by numbers of elements
}

// Commpute standard deviation
double compute_std(const std::vector<double> &values, double mean)
{
    double sum_sq = 0.0;
    for (double val : values)                  // Loop through vector
        sum_sq += (val - mean) * (val - mean); // Add the squared difference between the value and the mean
    return std::sqrt(sum_sq / values.size());  // Divide by numbers of elements and take square root
}

// Function to validate our computetotalEnergy() function
// For all 8 possible configurations of a 1D Ising model with 3 spins.
// Following the table in the question sheet
void testAllThreeSpinConfigs()
{
    std::cout << "Testing all 3-spin configurations:\n";

    IsingModel model(1, 3, 1.0); // 1 row, 3 columns, J = 1.0

    // Loop over all combinations of -1 and +1 for the 3 spins
    for (int s1 : {-1, 1})
    {
        for (int s2 : {-1, 1})
        {
            for (int s3 : {-1, 1})
            {
                // Set the spins manually
                model.spins[0][0] = s1;
                model.spins[0][1] = s2;
                model.spins[0][2] = s3;

                // Compute energy and magnetization
                double E = model.computeTotalEnergy();
                int M = model.computeMagnetization();

                // Print results
                std::cout << "Spins: [" << s1 << ", " << s2 << ", " << s3 << "]"
                          << " -> Energy: " << E << ", Magnetization: " << M << "\n";
            }
        }
    }
}

int main()
{
    // Validate compute energry function
    testAllThreeSpinConfigs();

    srand(time(0));       // Seed the RNG with the current time
    int R, C;             // User input for row and column
    double J = 1.0, beta; // Parameter that we need
    int steps = 150000;   // Numbers of time of performing Monte Carlo for 1 configuration

    // Set output folder for saving all results
    std::string output_dir = "Results_100x100/";

    // Create frame and PNG folders (for GIF) if they don't exist
    std::string frame_folder = output_dir + "frames_100x100";
    std::string png_folder = output_dir + "frames_png_100x100";
    std::filesystem::create_directories(frame_folder);
    std::filesystem::create_directories(png_folder);

    // Ask user for input grid size
    std::cout << "Enter number of rows (R): ";
    std::cin >> R;
    std::cout << "Enter number of columns (C): ";
    std::cin >> C;
    std::cout << "Enter beta value: ";
    std::cin >> beta;

    // Create necessary folders to store simulation frames and PNG images
    std::filesystem::create_directories(output_dir + "frames_100x100");
    std::filesystem::create_directories(output_dir + "frames_png_100x100");

    // Initialize model with user-specified size and perform Monte Carlo with user-specified beta
    IsingModel model(R, C, J);
    Montecarlo mc(beta);

    std::cout << "Initial Spin Configuration:\n";
    model.printLattice(); // print initial configuration
    std::cout << "Initial Energy: " << model.computeTotalEnergy() << std::endl;

    // Vectors to track energy and magnetization over time
    std::vector<double> energy_history;
    std::vector<int> magnetization_history;
    bool stabilized = false; // Track stabilization

    // Clear all files inside the "frames_RxC" directory before running the simulation
    for (const auto &entry : std::filesystem::directory_iterator(frame_folder)) {
        std::filesystem::remove(entry);
    }

    // Clear all files inside the "frames_png_RxC" directory before running the simulation
    for (const auto &entry : std::filesystem::directory_iterator(png_folder)) {
        std::filesystem::remove(entry);
    }
    // Perform Montel Carlo
    for (int i = 0; i < steps; i++)
    {
        mc.step(model); // Perform a single Monte Carlo step

        // Record energy and magnetization every 2000 steps (could be edit by user)
        if (i % 2000 == 0)
        {
            double energy = model.computeTotalEnergy();
            int magnetization = model.computeMagnetization();

            energy_history.push_back(energy);               // Store computed energy to this vector
            magnetization_history.push_back(magnetization); // Store compute magnetization to this vector

            std::cout << "Step " << i << " - Energy: " << energy
                      << ", Magnetization: " << magnetization << std::endl;

            // Below code chunk is for smoother GIF
            int subframes = 5; // Number of additional spin snapshots per 200000 steps

            for (int k = 0; k < subframes; ++k)
            {
                // Perform mini-steps in between frames (could be edit by user)
                for (int j = 0; j < 2000 / subframes; ++j)
                {
                    mc.step(model); // Keep evolving the system
                }

                // Save subframe
                std::ostringstream filename;
                filename << output_dir << "frames_100x100/frame_" << (i / 2000) * subframes + k << ".csv"; // 2000 here could be edit by user

                // Save the current spin configuration to a CSV file
                std::ofstream frameFile(filename.str());
                for (int r = 0; r < R; ++r) // Loop over rows
                {
                    for (int c = 0; c < C; ++c) // Loop over columns
                    {
                        frameFile << model.spins[r][c]; // Write spin values
                        if (c < C - 1)
                            frameFile << ","; // Add commas for the file to follow CSV file format
                    }
                    frameFile << "\n";
                }
                frameFile.close();
            }

            // Check for stabilization (after 5 recorded values)
            if (energy_history.size() > 5 && !stabilized)
            {
                double avg_energy = 0, avg_mag = 0, max_energy_change = 0;

                // Compute average energy and magnetization over last 5 entries
                for (int j = energy_history.size() - 5; j < energy_history.size(); j++)
                {
                    avg_energy += energy_history[j];
                    avg_mag += magnetization_history[j];

                    // Compute the max change in energy
                    if (j > energy_history.size() - 5 + 1)
                    {
                        double change = fabs(energy_history[j] - energy_history[j - 1]);
                        if (change > max_energy_change)
                        {
                            max_energy_change = change;
                        }
                    }
                }

                avg_energy /= 5;
                avg_mag /= 5;

                // Stabilization criteria, if reached then it is considered as stabilize/converged
                // If the energy change is small or if the energy is close to the average then we say it stabilized
                if (max_energy_change < 5.0 && fabs(energy - avg_energy) < 1.0)
                {
                    std::cout << "System stabilized at step " << i << "!\n";
                    stabilized = true; // Mark the system as stabilized
                }
            }
        }
    }

    // Save results to CSV after full simulation
    std::ofstream outFile(output_dir + "ising_single_run_100x100.csv");
    outFile << "Step,Energy,Magnetization\n";
    for (int i = 0; i < energy_history.size(); ++i)
    {
        outFile << (i * 1000) << "," << energy_history[i] << "," << magnetization_history[i] << "\n";
    }
    outFile.close();
    std::cout << "Results saved to ising_single_run_100x100.csv\n";

    // Print final results
    std::cout << "Final Spin Configuration:\n";
    model.printLattice();
    std::cout << "Final Energy: " << model.computeTotalEnergy() << std::endl;

    // Below we run different times of simulation for different value of beta

    std::vector<double> beta_values = {0.5, 1, 5, 10, 15, 20}; // Different beta that we will test on

    // Number of independent simulation runs for each beta value (could be change by user)
    int num_runs = 100;

    // Open CSV file to save averaged results across beta values
    std::ofstream summaryFile(output_dir + "ising_averaged_results_100x100.csv");
    summaryFile << "Beta,AvgEnergy,StdEnergy,AvgMagnetization,StdMagnetization\n";

    // New file to store all individual results for distribution plots
    std::ofstream allRunsFile(output_dir + "ising_all_runs_100x100.csv");
    allRunsFile << "Beta,Energy,Magnetization\n"; // header row

    // Loop over each beta value
    for (double beta_val : beta_values)
    {
        std::vector<double> energies; // Empty ector to store double value
        std::vector<double> magnetizations; // Empty ector to store double value

        // Repeat simulation for num_runs(100) times for statistical averaging
        for (int run = 0; run < num_runs; ++run)
        {
            IsingModel simModel(R, C, J); // Initialize new random spin configuration
            Montecarlo simMC(beta_val); // Peform Montecarlo

            // Perform Monte Carlo simulation for the given number of steps for each configuration
            for (int step = 0; step < steps; ++step)
            {
                simMC.step(simModel);
            }

            // Record energy and absolute value of magnetization after simulation
            double E = simModel.computeTotalEnergy();
            double M = std::abs(simModel.computeMagnetization());

            energies.push_back(E);
            magnetizations.push_back(M);

            // Save individual data to file
            allRunsFile << beta_val << "," << E << "," << M << "\n";
        }

        // Compute average and standard deviation of energy and magnetization of 1000 different configurations for each beta
        double mean_E = compute_mean(energies);
        double std_E = compute_std(energies, mean_E);
        double mean_M = compute_mean(magnetizations);
        double std_M = compute_std(magnetizations, mean_M);

        // Save results to CSV file
        summaryFile << beta_val << "," << mean_E << "," << std_E
                    << "," << mean_M << "," << std_M << "\n";

        std::cout << "Finished beta = " << beta_val << "\n";
    }

    summaryFile.close();
    std::cout << "Averaged results saved to ising_averaged_results_100x100.csv\n";

    return 0;
}
