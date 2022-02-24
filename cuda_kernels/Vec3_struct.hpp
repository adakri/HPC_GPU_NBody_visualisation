#include "NBody.hpp"


// Can not use a class declaration inside a device function
typedef struct vector 
{
  spec_t _x;
  spec_t _y;
  spec_t _z;

/*   vector(spec_t x, spec_t y, spec_t z) 
        : _x(x), _y(y), _z(z)
  {
  } */

  vector operator+(const vector& v)
  {
      return {v._x + this->_x, v._y + this->_y, v._z + this->_z};
  }

  vector operator-(const vector& v)
  {
      return {v._x - this->_x, v._y - this->_y, v._z - this->_z};
  }

  void isString()
  {
      std::cout<<"vector: _x:"<< _x <<" ,_y "<< _y <<" ,_z "<<_z<< std::endl;
  }

  __device__
  vector& operator =(const vector& v)
  {
      _x = v._x;
      _y = v._y;
      _z = v._z;
      return *this;
  }

  bool operator ==(const vector &v) const
  {
      return (this->_x == v._x) && (this->_y == v._y) && (this->_z == v._z) ;
  }

  vector operator*(double s) const 
  {
      vector ret;
      ret._x = _x * s;
      ret._y = _y * s;
      ret._z = _z * s; 
      return ret;
  }

  


};

// All the overloaded functions
__device__
spec_t norm2(vector& v)
{
    return v._x*v._x + v._y*v._y + v._z*v._z;
}

__device__
void normalize(vector& v) 
{
    Scalar length = norm2(v);
    if(length==0.)
    {
      v._x = 0.;
      v._y = 0.;
      v._z = 0.;
    }
    else{
      v._x /= length;
      v._y /= length;
      v._z /= length;
    }
}

__device__
void invert(vector& v) 
{
    v._x *= -1.0;
    v._y *= -1.0;
    v._z *= -1.0;
}

__device__
void v_direction(
        const vector &fromVector,
        const vector &toVector,
        vector &resultVector) 
{
  resultVector._x = toVector._x - fromVector._x;
  resultVector._y = toVector._y - fromVector._y;
  resultVector._z = toVector._z - fromVector._z;

  //printf("result %f %f %f \n", resultVector._x, resultVector._y, resultVector._z);

  normalize(resultVector);
  invert(resultVector);
};

__device__
Force ComputeForce( 
          Mass onMass, 
          Mass becauseOfMass,
          vector onPosition, 
          vector becauseOfPosition,
          vector& onVel ) 
{
  Scalar delta_x = becauseOfPosition._x - onPosition._x;
  Scalar delta_y = becauseOfPosition._y - onPosition._y;
  Scalar delta_z = becauseOfPosition._z - onPosition._z;
  Scalar distance = sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
  // add rayon moyen
  if(distance <= 1e2)
  {
    invert(onVel);
  }
    

  //printf("Inside the force comp %f \n", distance);

  if( distance < 1e-5 ) 
  {
    return 0.;
  }

  //printf("debug %f %f %f \n", onMass, becauseOfMass, G * (onMass * becauseOfMass) /  (distance * distance));

  Force result = (float)G * (float)(onMass * becauseOfMass) /  (float)(distance * distance);

  //printf("Inside the force comp G %f \n", result);

  return result;
};

__device__
Acceleration computeAcceleration( Mass mass, Force force ) 
{
  if( force == 0 ) 
  {
    return 0;
  }

  Scalar result = force / mass;
  return result;
};

__device__
Velocity computeVelocity(Acceleration current, Velocity previous, Time deltaT) 
{
  return previous + (current * deltaT);
};

__device__
Position computePosition(Velocity current, Position previous, Time deltaT) 
{
  return previous + (current * deltaT);
};

__device__
vector computeAcceleration3D(Mass mass, const vector &force) 
{
  vector anAccelVector = {0, 1., 1.};
  anAccelVector._x = computeAcceleration(mass, force._x);
  anAccelVector._y = computeAcceleration(mass, force._y);
  anAccelVector._z = computeAcceleration(mass, force._z);
  return anAccelVector;
};

__device__
vector computeVelocity3D(vector &accel, vector &prevVelo, Time deltaT) 
{
  vector aVelocityVector = {0, 1., 1.};
  aVelocityVector._x = computeVelocity( accel._x, prevVelo._x, deltaT );
  aVelocityVector._y = computeVelocity( accel._y, prevVelo._y, deltaT );
  aVelocityVector._z = computeVelocity( accel._z, prevVelo._z, deltaT );
  return aVelocityVector;
};

__device__
vector computePosition3D(vector &velo, vector &prevPos, Time deltaT) 
{
  vector anPositionVector = {0, 0, 0};
  anPositionVector._x = computePosition(velo._x, prevPos._x, deltaT);
  anPositionVector._y = computePosition(velo._y, prevPos._y, deltaT);
  anPositionVector._z = computePosition(velo._z, prevPos._z, deltaT);
  return anPositionVector;

};
