#include "random_init.hpp"

#include <random>

const float PI = 3.14159265358979323846;

using namespace std;


Vec3 randomParticlePos()
{
    /* initialize random seed: */
    srand (time(NULL));

	// Random position on a 'thick disk'
	Vec3 pos;
	float t = (rand() / float(RAND_MAX)) *2*PI;
	debug(t)
	float s = rand()*100. / float(RAND_MAX);
	debug(s)
	pos._x = cos(t)*s;
	pos._y = sin(t)*s;
	pos._z = (rand()*4.) / float(RAND_MAX);

	return pos;
}

Vec3 randomParticleVel()
{
    /* initialize random seed: */
    srand (time(NULL));

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(rand() * 20. / RAND_MAX, rand() * 20. / RAND_MAX, 0.);
	return vel;
}


