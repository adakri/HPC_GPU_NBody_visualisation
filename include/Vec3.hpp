#ifndef VEC3_H
#define VEC3_H


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <stdint.h>
#include <chrono>
#include <ctime> 

#define spec_t double

#include "Constants.hpp"


// Vectors
typedef double Vector;
typedef Vector Force;
typedef Vector Acceleration;
typedef Vector Position;
typedef Vector Velocity;


class Vec3 {

    private:
      
    public:
      spec_t _x;
      spec_t _y;
      spec_t _z;
      //Première fois que j'avais besoin d'implémenter un constructeur par défault et que je ne l'ai pas implementé
      Vec3()=default;
      Vec3(spec_t x, spec_t y, spec_t z);
      ~Vec3(){};

      //implemented methods/ maths and physics
      spec_t norm2() const;
      void normalize();
      void invert();
      Force forceNewtonianGravit_y3D();


      Vec3 operator+(const Vec3& v);
      Vec3 operator-(const Vec3& v);
      Vec3 operator*(double s) const;
      bool operator==(const Vec3& v) const;
      Vec3& operator=(const Vec3& v);
      friend std::ostream &operator <<(std::ostream &str, const Vec3 &v);
      friend std::istream& operator>>(std::istream&, Vec3&);
      void isString();

      // getter/setter
      spec_t getX() const { return _x;};
      spec_t getY() const { return _y;};
      spec_t getZ() const { return _z;};
      void setX(spec_t x) {this->_x = x;};
      void setY(spec_t y) {this->_y = y;};
      void setZ(spec_t z) {this->_z = z;};
};

//3D
typedef Vec3 Force3D;
typedef Vec3 Acceleration3D;
typedef Vec3 Velocity3D;
typedef Vec3 Position3D;

// Scalar
typedef double Scalar;
typedef Scalar Mass;
typedef Scalar Time;



#endif