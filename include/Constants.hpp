
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>


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

/* Random velocity range for bodies */
const int kMetersInKiloMeters = 1000;
const double kMinVelocity = -0.5 * kMetersInKiloMeters;
const double kMaxVelocity = 0.5 * kMetersInKiloMeters;

/* Random mass range for bodies */
const double kMinMass = 0.025 * kSolarMassScaled;
const double kMaxMass = 100 * kSolarMassScaled;

/* Random radius range for bodies */
const double kMinRadius = 5;
const double kMaxRadius = 12;

#endif 