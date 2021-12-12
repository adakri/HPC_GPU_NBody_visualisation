#include "Physics.hpp"


double Distance(Body a, Body b) {
  return sqrt(
      ((a._position._x - b._position._x) * (a._position._x - b._position._x)) +
      ((a._position._y - b._position._y) * (a._position._y - b._position._y)) +
      ((a._position._z - b._position._z) * (a._position._z - b._position._z)));
}

Vec3 VelocityInteraction(Body a, Body b, double time_step) 
{
  double distance = Distance(a, b);
  double g = (b._mass * G / (distance * distance * distance)) * time_step;
  return Vec3(g * (a._position._x - b._position._x),
                g * (a._position._y - b._position._y),
                g * (a._position._z - b._position._z));
}

double ConserveMomentum(double m1, double m2, double v1,
                                     double v2) 
{
  return ((m1 * v1) + (m2 * v2)) / (m1 + m2);
}

Body Collision(Body a, Body b) {
  Vec3 velocity =
      Vec3(
          ConserveMomentum(a._mass, b._mass, a._velocity._x, b._velocity._x),
          ConserveMomentum(a._mass, b._mass, a._velocity._y, b._velocity._y),
          ConserveMomentum(a._mass, b._mass, a._velocity._z, b._velocity._z)
          );
          //resolve acceleration
  return Body(a._position, velocity, velocity, a._mass + b._mass, 
              std::max(a._radius, b._radius));
}

bool SamePosition(Body a, Body b) {
  return a._radius + b._radius > Distance(a, b);
}



// Kinematics

Velocity computeVelo( Acceleration current,
                             Velocity previous,
                             Time deltaT ) 
{
   return previous + ( current * deltaT );
}

Position computePos( Velocity current,
                            Position previous,
                            Time deltaT ) 
{
   return previous + ( current * deltaT );
}

Acceleration computeAccel( Mass mass, Force force ) 
{
   if( force == 0 ) {
      return 0;
   }

   Scalar result = force / mass;
   return result;
}




inline Acceleration3D computeAccel3D( Mass mass, const Force3D &force ) 
{
   Acceleration3D anAccelVector = {0, 0, 0};
   anAccelVector._x = computeAccel( mass, force._x );
   anAccelVector._y = computeAccel( mass, force._y );
   anAccelVector._z = computeAccel( mass, force._z );
   return anAccelVector;
}




Velocity3D computeVelo3D( Acceleration3D &accel,
                                 Velocity3D &prevVelo,
                                 Time deltaT ) {
   Velocity3D aVelocityVector = {0, 0, 0};
   aVelocityVector._x = computeVelo( accel._x, prevVelo._x, deltaT );
   aVelocityVector._y = computeVelo( accel._y, prevVelo._y, deltaT );
   aVelocityVector._z = computeVelo( accel._z, prevVelo._z, deltaT );
   return aVelocityVector;
}

Position3D computePos3D( Velocity3D &velo,
                                Position3D &prevPos,
                                Time deltaT ) {
   Position3D anPositionVector = {0, 0, 0};
   anPositionVector._x = computePos( velo._x, prevPos._x, deltaT );
   anPositionVector._y = computePos( velo._y, prevPos._y,  deltaT );
   anPositionVector._z = computePos( velo._z, prevPos._z,  deltaT );
   return anPositionVector;
}
