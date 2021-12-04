#include "Physics.hpp"


double Physics::Distance(Body a, Body b) {
  return sqrt(
      ((a.position_._x - b.position_._x) * (a.position_._x - b.position_._x)) +
      ((a.position_._y - b.position_._y) * (a.position_._y - b.position_._y)) +
      ((a.position_._z - b.position_._z) * (a.position_._z - b.position_._z)));
}

Vec3 Physics::VelocityInteraction(Body a, Body b, double time_step) 
{
  double distance = Distance(a, b);
  double g = (b.mass_ * G / (distance * distance * distance)) * time_step;
  return Vec3(g * (a.position_._x - b.position_._x),
                g * (a.position_._y - b.position_._y),
                g * (a.position_._z - b.position_._z));
}

double Physics::ConserveMomentum(double m1, double m2, double v1,
                                     double v2) 
{
  return ((m1 * v1) + (m2 * v2)) / (m1 + m2);
}

Body Physics::Collision(Body a, Body b) {
  Vec3 velocity =
      Vec3(
          ConserveMomentum(a.mass_, b.mass_, a.velocity_._x, b.velocity_._x),
          ConserveMomentum(a.mass_, b.mass_, a.velocity_._y, b.velocity_._y),
          ConserveMomentum(a.mass_, b.mass_, a.velocity_._z, b.velocity_._z)
          );
  return Body(a.position_, velocity, a.mass_ + b.mass_,
              std::max(a.radius_, b.radius_));
}

bool Physics::SamePosition(Body a, Body b) {
  return a.radius_ + b.radius_ > Distance(a, b);
}