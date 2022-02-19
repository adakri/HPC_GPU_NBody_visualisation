#include "NBody.hpp"

#include <cuda.h>



// In the same fashion as static function, Let's test device functions (only used and declared exclusively on the device GPU)

// I am trying this in order to wrap a cuda device proc in a c++ class
// https://gist.github.com/lebedov/bca3c70e664f54cdf8c3cd0c28c11a0f


__device__
void d_updateAcceleration(int, Position3D *,Acceleration3D *,Mass *, int);

__device__
void d_updateVelocity(int , float , Acceleration3D *, Velocity3D *); 


__device__
void d_updatePosition(int, float, Velocity3D *,Position3D *); 


__global__
void d_updatePhysics(int, float, Position3D *, Velocity3D *,Acceleration3D *,Mass *, int);

// Can not use a class declaration inside a device function
typedef struct vector 
{
  spec_t _x;
  spec_t _y;
  spec_t _z;
};



// demo (from this astrophysics github page)
# define N 18
static Position3D nBodyPosition[N] = 
{
  { 0.0f, 0.0f, -1000.0f },
  { 0.0f, 200.0f, -1000.0f },
  { -200.0f, 0.0f, -1000.0f },
  { 0.0f, 0.0f, -800.0f },
  { -100.0f, 0.0f, -2000.0f },
  { 100.0f, 0.0f, -500.0f },
  { -50.0f, 10.0f, 0.0f },
  { 150.0f, 0.0f, -100.0f },
  { 800.0f, 0.0f, -20.0f },
  { 100.0f, 0.0f, -100.0f },
  { 100.0f, 0.0f, -500.0f },
  { -50.0f, 10.0f, 0.0f },
  { 150.0f, 0.0f, -100.0f },
  { 800.0f, 0.0f, -20.0f },
  { 100.0f, 0.0f, -100.0f },
  { 0.0f, 0.0f, -1000.0f },
  { 0.0f, 200.0f, -1000.0f },
  { -200.0f, 0.0f, -1000.0f },
};

static Velocity3D nBodyVelocity[N] = 
{
  { 0.0f, 0.0f, 0.0f },
  { -30.0f, -30.0f, -30.0f },
  { 30.0f, 30.0f, 30.0f },
  { 45.0f, -30.0f, 15.0f },
  { -30.0f, 20.0f, -45.0f},
  { -30.0f, 20.0f, 10.0f},
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { -30.0f, -30.0f, -30.0f },
  { 30.0f, 30.0f, 30.0f }
};

static Acceleration3D nBodyAcceleration[N] = 
{
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f }
};

static Mass nBodyMass[N] = 
{
  1e16f,
  1e1f,
  1e1f,
  2e1f,
  3e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  2e1f,
  1e16f,
  1e1f,
  1e1f
};