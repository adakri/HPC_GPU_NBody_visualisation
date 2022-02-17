#include "NBody.hpp"


// In the same fashion as static function, Let's test device functions (only used and declared exclusively on the device GPU)

// I am trying this in order to wrap a cuda device proc in a c++ class
// https://gist.github.com/lebedov/bca3c70e664f54cdf8c3cd0c28c11a0f


__device__
void d_updateAcceleration(int, Position3D *,Acceleration3D *,Mass *);

__device__
void d_updateVelocity(int , float , Acceleration3D *, Velocity3D *); 


__device__
void d_updatePosition(int, float, Velocity3D *,Position3D *); 


__global__
void d_updatePhysics(int, float, Position3D *, Velocity3D *,Acceleration3D *,Mass *);

