#ifndef BODY_H
#define BODY_H

#include "Constants.hpp"
#include "Vec3.hpp"


#include <math.h>
#include <stdio.h>


/*
  Class for a body in the simulation (position, velocity, mass,
  and radius). Also eventually the functions for drawing the body in the simulation, as well as updating the body.
*/
class Body {
  public:
    Vec3 _position;
    Vec3 _velocity;
    Vec3 _acceleration;
    double _mass;
    double _radius;

    Body(Vec3 p, Vec3 v, Vec3 a, double m, double r) {
      _position = p;
      _velocity = v;
      _acceleration = a;
      _mass = m;
      _radius = r;
    }

    /* Will draw the body in the application. */
    void Draw() const;
    /*
      Update the position of the body by modeling the body movement with the given time step.
    */
    void UpdatePosition(double time_step);
    void isString();

    //getters and setters
    Vec3 get_position() const { return _position;};
    Vec3 get_velocity() const { return _velocity;};
    Vec3 get_acceleration() const { return _acceleration;};
    double get_mass() const { return _mass;};
    double get_radius() const { return _radius;};

    void set_position(Vec3 p) {  _position = p;};
    void set_velocity(Vec3 v) {  _velocity = v;};
    void set_acceleration(Vec3 a) {  _acceleration = a;};

    

};

#endif /* body_h */