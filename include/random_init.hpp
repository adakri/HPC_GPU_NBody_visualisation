#include <glm/glm.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Vec3.hpp"

/**
 * Generates a random particle position
 * @return 3D position 
 */
Vec3 randomParticlePos();

/**
 * Generates a random particle velocity
 * @param pos the same particle's position
 * @return 3D velocity 
 */
Vec3 randomParticleVel();

//same
Vec3 randomParticleacceleration();

//same 
float random_mass();

//same
float random_radius();

