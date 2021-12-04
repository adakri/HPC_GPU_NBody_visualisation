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
