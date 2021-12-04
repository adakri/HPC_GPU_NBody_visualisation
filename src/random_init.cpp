#include "random_init.hpp"

#include <random>

const float PI = 3.14159265358979323846;

using namespace std;


Vec3 randomParticlePos()
{
    /* initialize random seed: */
    srand (time(NULL));

	// Random position on a 'thick disk'
	Vec3 particle;
	float t = rand()*2*PI / RAND_MAX;
	float s = rand()*100 / RAND_MAX;
	particle._x = cos(t)*s;
	particle._y = sin(t)*s;
	particle._z = rand()*4 / RAND_MAX;

	return particle;
}

Vec3 randomParticleVel()
{
    /* initialize random seed: */
    srand (time(NULL));

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(rand() * 20. / RAND_MAX, rand() * 20. / RAND_MAX, 0.);
	return vel;
}


