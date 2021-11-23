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


class Vec2 {

    private:
      
    public:
      spec_t _x;
      spec_t _y;
      spec_t _z;
      //Première fois que j'avais besoin d'implémenter un constructeur par défault et que je ne l'ai pas implementé
      Vec2()=default;
      Vec2(spec_t x, spec_t y, spec_t z);
      ~Vec2(){};

      //implemented methods
      spec_t norm2() const;

      Vec2 operator+(const Vec2& v);
      Vec2 operator-(const Vec2& v);
      bool operator==(const Vec2& v) const;
      Vec2& operator=(const Vec2& v);
      friend std::ostream &operator <<(std::ostream &str, const Vec2 &v);
      void isString();

      // getter/setter
      spec_t getX() const { return _x;};
      spec_t getY() const { return _y;};
      spec_t getZ() const { return _z;};
      void setX(spec_t x) {this->_x = x;};
      void setY(spec_t y) {this->_y = y;};
      void setY(spec_t z) {this->_z = z;};
};




#endif