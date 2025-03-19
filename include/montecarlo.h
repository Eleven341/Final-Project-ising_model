#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "ising.h"

class Montecarlo {
public:
    double beta;  // Temperature (1/kT)

    Montecarlo(double beta);  // Constructor
    void step(IsingModel &model);  // Performs a Monte Carlo step
};

#endif
