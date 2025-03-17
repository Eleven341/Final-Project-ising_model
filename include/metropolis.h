#ifndef METROPOLIS_H
#define METROPOLIS_H

#include "ising.h"

class Metropolis {
public:
    double beta;  // Temperature (1/kT)

    Metropolis(double beta);  // Constructor
    void step(IsingModel &model);  // Performs a Monte Carlo step
};

#endif
