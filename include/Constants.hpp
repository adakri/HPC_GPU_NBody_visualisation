
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

/**
 * @brief All the constants used to approach the physical reality of an N-Body simulation.
 * 
 */


/**
 * @brief Macro for debugging, displays variable name and value for natively typed variables.
 * 
 */
#define debug(a) std::cout<<"*** DEBUG ***: "<<#a<<" "<<a<<std::endl;

/* Universal Gravitational Constant */
const double G = 6.67E-11;

/* Mass constants*/
const double kMassScale = 1E-15;
const double kSolarMass = 1.989E30;
const double kSolarMassScaled = kMassScale * kSolarMass;

/* Time constants - scaled due to mass scaling */
const double kTimeScale = 1E-10;
const double kSecondsInYear = 3.154E7;

/* Velocity range for bodies */
const int kMetersInKiloMeters = 1000;
const double kMinVelocity = -0.5 * kMetersInKiloMeters;
const double kMaxVelocity = 0.5 * kMetersInKiloMeters;

/* Mass range for bodies */
const double kMinMass = 0.025 * kSolarMassScaled;
const double kMaxMass = 100 * kSolarMassScaled;

/* Radius range for bodies */
const double kMinRadius = 15;
const double kMaxRadius = 22;

#endif 