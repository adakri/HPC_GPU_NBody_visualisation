#include "Body.hpp"

void Body::Draw() const {

}

void Body::isString()
{
  printf("Body with: \n");
  printf("Mass %3.3f and Radius %3.3f ", this->_mass, this->_radius);
  printf("Position ( %8.4f , %8.4f, %8.4f ) \n", this->_position._x, this->_position._y, this->_position._z);
  printf("Velocity ( %8.4f , %8.4f, %8.4f ) \n", this->_velocity._x, this->_velocity._y, this->_velocity._y);
  printf("Acceleration ( %8.4f , %8.4f, %8.4f ) \n", this->_acceleration._x, this->_acceleration._y, this->_acceleration._y);
}

void Body::UpdatePosition(double time_step) {
  _position._x += (_velocity._x * time_step);
  _position._y += (_velocity._y * time_step);
  _position._z += (_velocity._z * time_step);
}