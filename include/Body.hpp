#ifndef BODY_H
#define BODY_H

#include "Constants.hpp"
#include "Vec3.hpp"

#include <math.h>
#include <stdio.h>

/**
 * @brief
 * Class for a body in the simulation (position, velocity, mass,
  and radius). Also eventually the functions for drawing the body in the simulation, as well as updating the body.
 */

class Body
{
public:
  Vec3 _position;
  Vec3 _velocity;
  Vec3 _acceleration;
  Vec3 force;
  double _mass;
  double _radius;

  /**
   * @brief Construct a new Body object
   *
   * @param p
   * @param v
   * @param a
   * @param m
   * @param r
   */
  Body(Vec3 p, Vec3 v, Vec3 a, double m, double r)
  {
    _position = p;
    _velocity = v;
    _acceleration = a;
    _mass = m;
    _radius = r;
  }

  /**
   * @brief Update the position of the body by modeling the body movement with the given time step.
   *
   * @param time_step
   */
  void UpdatePosition(double time_step);
  /**
   * @brief isString method to print a body in a more exhaustive way (instead of overloading the operator).
   *
   */
  void isString();

  // getters and setters
  Vec3 get_position() const { return _position; };
  Vec3 get_velocity() const { return _velocity; };
  Vec3 get_acceleration() const { return _acceleration; };
  double get_mass() const { return _mass; };
  double get_radius() const { return _radius; };

  void set_position(Vec3 p) { _position = p; };
  void set_velocity(Vec3 v) { _velocity = v; };
  void set_acceleration(Vec3 a) { _acceleration = a; };
};

#endif /* body_h */