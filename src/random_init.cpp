#include "random_init.hpp"

#include <random>

const float PI = 3.14159265358979323846;

using namespace std;

Vec3 randomParticlePosition()
{

	// std::cout<<( (double)rand() / RAND_MAX )<<std::endl;

	// Random position on a 'thick disk'
	Vec3 pos = Vec3(
		(((float)rand()) / (float)RAND_MAX) * 1820. - 540.,
		(((float)rand()) / (float)RAND_MAX) * 1860. - 1000.,
		(((float)rand()) / (float)RAND_MAX) * -2500. - 800);

	return pos;
}

Vec3 randomParticleVelocity()
{

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(
		(((float)rand()) / (float)RAND_MAX) * 60. - 30.,
		(((float)rand()) / (float)RAND_MAX) * 60. - 30.,
		(((float)rand()) / (float)RAND_MAX) * 60. - 30.);
	return vel;
}

Vec3 randomParticleacceleration()
{

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(0., 0., 0.);
	return vel;
}

float random_mass()
{
	return ((float)rand()) / (float)RAND_MAX * (5. - 1. + 1) + 1.;
}

float random_radius()
{
	return ((float)rand()) / (float)RAND_MAX * (kMaxRadius - kMinRadius + 1) + kMinRadius;
}
