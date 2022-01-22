#include "random_init.hpp"

#include <random>

const float PI = 3.14159265358979323846;

using namespace std;


Vec3 randomParticlePos()
{

	//std::cout<<( (double)rand() / RAND_MAX )<<std::endl;

	// Random position on a 'thick disk'
	Vec3 pos;
	float t = ( (double)rand() / RAND_MAX ) *2*PI;
	float s = ( (double)rand() / RAND_MAX )*1.;
	pos._x = cos(t)*s;
	pos._y = sin(t)*s;
	pos._z = (rand()*4.) / float(RAND_MAX);

	//std::cout<<pos._x<<" "<<pos._y<<" "<<pos._z<<std::endl;
	return pos;
}

Vec3 randomParticleVel()
{

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(rand() * 20. / RAND_MAX, rand() * 20. / RAND_MAX, 0.);
	return vel;
}


Vec3 randomParticleacceleration()
{

	// Initial velocity is 'orbital' velocity from position
	Vec3 vel = Vec3(rand() * 2. / RAND_MAX, rand() * 2. / RAND_MAX, 0.);
	return vel;
}


