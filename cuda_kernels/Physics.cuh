#include "Vec3_struct.hpp"

#include <cuda.h>
#include <math.h>  



// In the same fashion as static function, Let's test device functions (only used and declared exclusively on the device GPU)

// I am trying this in order to wrap a cuda device proc in a c++ class
// https://gist.github.com/lebedov/bca3c70e664f54cdf8c3cd0c28c11a0f


__device__
void d_updateAcceleration(int, vector *,vector *,Mass *, int);

__device__
void d_updateVelocity(int , float , vector *, vector *); 


__device__
void d_updatePosition(int, float, vector *,vector *); 


__global__
void d_updatePhysics(int, float, vector *, vector *,vector *,Mass *, int);




// demo (from this astrophysics github page)
# define N 32
static vector nBodyPosition[N] = 
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
  { 0.0f, 200.0f, -1000.0f }
};

static vector nBodyVelocity[N] = 
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
  { -30.0f, -30.0f, -30.0f }
};

static vector nBodyAcceleration[N] = 
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
  1e14f,
  1e11f,
  2e15f,
  3e11f,
  2e11f,
  2e16f,
  2e11f,
  2e11f,
  2e17f,
  2e11f,
  8e15f,
  2e11f,
  2e11f,
  9e11f,
  1e16f,
  1e11f,
  4.5e16f,
  2e11f,
  3e11f,
  2.3e11f,
  2e11f,
  2e11f,
  2.4e11f,
  2e11f,
  2e11f,
  205e11f,
  2e11f,
  267e11f,
  2e11f,
  122e16f,
  11e11f
};