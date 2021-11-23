#include "Vec2.hpp"


Vec2::Vec2(spec_t _x, spec_t _y): _x(_x), _y(_y), _z(z)
{
}

spec_t Vec2::norm2() const
{
    return _x*_x + _y*_y + _z*_z;
}

Vec2 Vec2::operator+(const Vec2& v)
{
    return {v._x + this->_x, v._y + this->_y, v._z + this->_z};
}

Vec2 Vec2::operator-(const Vec2& v)
{
    return {v._x - this->_x, v._y - this->_y, v._z - this->_z};
}

void Vec2::isString()
{
    std::cout<<"vector: _x:"<< _x <<" ,_y "<< _y <<" ,_z "<<_z<< std::endl;
}

//maybe copy swap ?
Vec2& Vec2::operator =(const Vec2& v)
{
    _x = v._x;
    _y = v._y;
    _z = v._z;
    return *this;
}

bool Vec2::operator ==(const Vec2 &v) const
{
    return (this->_x == v._x) && (this->_y == v._y) && (this->_z == v._z) ;
}

//Là ça commence à devenir obscure, des friedns dans des struces c'est nouveau pour moi
// La chance a fait que ça a fonctionné ou que c'est possible.
std::ostream & operator <<(std::ostream &str, const Vec2 &v)
{
    return str << "Vec2D x: " << v._x << " y: " << v._y;
}
