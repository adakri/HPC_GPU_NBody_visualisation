#include "Body.hpp"

void Body::Draw() const {

}

void Body::UpdatePosition(double time_step) {
  position_._x += (velocity_._x * time_step);
  position_._y += (velocity_._y * time_step);
  position_._z += (velocity_._z * time_step);
}