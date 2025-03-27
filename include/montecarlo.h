#ifndef MONTECARLO_H  // Prevent multiple inclusion of this header file
#define MONTECARLO_H
#include "ising.h" // Include header file

class Montecarlo {
public:
    Montecarlo(double beta);  // Constructor
    void step(IsingModel &model);  // Performs a Monte Carlo step

private:
    double beta;  // Temperature 
};

#endif
