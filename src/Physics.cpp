#include "Physics.hpp"


//physics (not static just to test)

Force Physics::ComputeForce( 
          Mass onMass, 
          Mass becauseOfMass,
          Position3D onPosition, 
          Position3D becauseOfPosition) 
{
  Scalar delta_x = becauseOfPosition._x - onPosition._x;
  Scalar delta_y = becauseOfPosition._y - onPosition._y;
  Scalar delta_z = becauseOfPosition._z - onPosition._z;
  Scalar distance = sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);

  if( distance == 0 ) 
  {
    return 0;
  }

  Force result = G * (onMass * becauseOfMass) /  (distance * distance);
  return result;
};

Acceleration Physics::computeAcceleration( Mass mass, Force force ) 
{
  if( force == 0 ) 
  {
    return 0;
  }

  Scalar result = force / mass;
  return result;
}

Velocity Physics::computeVelocity(Acceleration current, Velocity previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

Position Physics::computePosition(Velocity current, Position previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

Acceleration3D Physics::computeAcceleration3D(Mass mass, const Force3D &force) 
{
  Acceleration3D anAccelVector = Vec3(0, 1., 1.);
  anAccelVector._x = Physics::computeAcceleration(mass, force._x);
  anAccelVector._y = Physics::computeAcceleration(mass, force._y);
  anAccelVector._z = Physics::computeAcceleration(mass, force._z);
  return anAccelVector;
}

Velocity3D Physics::computeVelocity3D(Acceleration3D &accel, Velocity3D &prevVelo, Time deltaT) 
{
  Velocity3D aVelocityVector = Vec3(0, 1., 1.);
  aVelocityVector._x = Physics::computeVelocity( accel._x, prevVelo._x, deltaT );
  aVelocityVector._y = Physics::computeVelocity( accel._y, prevVelo._y, deltaT );
  aVelocityVector._z = Physics::computeVelocity( accel._z, prevVelo._z, deltaT );
  return aVelocityVector;
}

Position3D Physics::computePosition3D(Velocity3D &velo, Position3D &prevPos, Time deltaT) 
{
  Position3D anPositionVector = Vec3(0, 0, 0);
  anPositionVector._x = Physics::computePosition(velo._x, prevPos._x, deltaT);
  anPositionVector._y = Physics::computePosition(velo._y, prevPos._y, deltaT);
  anPositionVector._z = Physics::computePosition(velo._z, prevPos._z, deltaT);
  return anPositionVector;

}