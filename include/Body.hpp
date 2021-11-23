#ifndef BODY_H
#define BODY_H

#include "constants.h"
#include "Vec3.hpp"


#include <math.h>
#include <stdio.h>


/*
  Class for a body in the simulation (position, velocity, mass,
  and radius). Also eventually the functions for drawing the body in the
  simulation, as well as updating the body.
*/
class Body {
 public:
  Vec3 position_;
  Vec3 velocity_;
  double mass_;
  double radius_;

  Body(Vec3 p, Vec3 v, double m, double r) {
    position_ = p;
    velocity_ = v;
    mass_ = m;
    radius_ = r;
  }

  /* Will draw the body in the OpenFrameworks application. */
  void Draw() const;

  /*
    Update the position of the body by modeling the body movement with the
    given time step.
   */
  void UpdatePosition(double time_step);
};

#endif /* body_h */