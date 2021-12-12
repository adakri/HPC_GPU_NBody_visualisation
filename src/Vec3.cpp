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

Vec3 Vec3::operator*(double s) const 
{
    return Vec3(_x*s, _y*s, _z*s);
}

std::istream& operator>>(std::istream& in, Vec3& v) {
    return in >> v._x >> v._y >> v._z;
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




