#include "Vec3.hpp"


Vec3::Vec3(spec_t x, spec_t y, spec_t z): _x(x), _y(y), _z(z)
{
}

spec_t Vec3::norm2() const
{
    return _x*_x + _y*_y + _z*_z;
}

Vec3 Vec3::operator+(const Vec3& v)
{
    return {v._x + this->_x, v._y + this->_y, v._z + this->_z};
}

Vec3 Vec3::operator-(const Vec3& v)
{
    return {v._x - this->_x, v._y - this->_y, v._z - this->_z};
}

void Vec3::isString()
{
    std::cout<<"vector: _x:"<< _x <<" ,_y "<< _y <<" ,_z "<<_z<< std::endl;
}

Vec3& Vec3::operator =(const Vec3& v)
{
    _x = v._x;
    _y = v._y;
    _z = v._z;
    return *this;
}

bool Vec3::operator ==(const Vec3 &v) const
{
    return (this->_x == v._x) && (this->_y == v._y) && (this->_z == v._z) ;
}


std::ostream & operator <<(std::ostream &str, const Vec3 &v)
{
    return str << "Vector 3D _x: " << v._x << " _y: " << v._y;
}

void Vec3::normalize() 
{
  Scalar length = this->norm2();
  this->_x /= length;
  this->_y /= length;
  this->_z /= length;
}

void Vec3::invert() 
{
  this->_x *= -1.0;
  this->_y *= -1.0;
  this->_z *= -1.0;
}

void direction(
        const Vec3 &fromVector,
        const Vec3 &toVector,
        Vec3 &resultVector) 
{
  resultVector._x = toVector._x - fromVector._x;
  resultVector._y = toVector._y - fromVector._y;
  resultVector._z = toVector._z - fromVector._z;
  resultVector.normalize();
}

// Physics operations

Force forceNewtonianGravit_y3D( 
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
}

Acceleration computeAccel( Mass mass, Force force ) 
{
  if( force == 0 ) 
  {
    return 0;
  }

  Scalar result = force / mass;
  return result;
}

Velocity computeVelo(Acceleration current, Velocity previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

Position computePos(Velocity current, Position previous, Time deltaT) 
{
  return previous + (current * deltaT);
}

inline Acceleration3D computeAccel3D(Mass mass, const Force3D &force) 
{
  Acceleration3D anAccelVector = {0, 0, 0};
  anAccelVector._x = computeAccel(mass, force._x);
  anAccelVector._y = computeAccel(mass, force._y);
  anAccelVector._z = computeAccel(mass, force._z);
  return anAccelVector;
}

inline Velocit_y3D computeVelo3D(Acceleration3D &accel, Velocit_y3D &prevVelo, Time deltaT) 
{
  Velocit_y3D aVelocit_yVector = {0, 0, 0};
  aVelocit_yVector._x = computeVelo( accel._x, prevVelo._x, deltaT );
  aVelocit_yVector._y = computeVelo( accel._y, prevVelo._y, deltaT );
  aVelocit_yVector._z = computeVelo( accel._z, prevVelo._z, deltaT );
  return aVelocit_yVector;
}

inline Position3D computePos3D(Velocit_y3D &velo, Position3D &prevPos, Time deltaT) 
{
  Position3D anPositionVector = {0, 0, 0};
  anPositionVector._x = computePos(velo._x, prevPos._x, deltaT);
  anPositionVector._y = computePos(velo._y, prevPos._y, deltaT);
  anPositionVector._z = computePos(velo._z, prevPos._z, deltaT);
  return anPositionVector;
}

