#include <glm/glm.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Vec3.hpp"

/*
Gives random particle physical quantities in the constant range given in the Constants.hpp file.
*/
Vec3 randomParticlePosition();

Vec3 randomParticleVelocity();

Vec3 randomParticleacceleration();

float random_mass();

float random_radius();
