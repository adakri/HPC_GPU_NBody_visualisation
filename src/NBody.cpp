#include "NBody.hpp"


bool DEMO = false;


NBody::NBody(int N , float tf, int timeSteps): _N(N), _tf(tf), _timeSteps(timeSteps) 
{
    _deltaT = _tf / float(_timeSteps);
    _bodies.resize(_N);

    /* initialize random seed: */
    srand (time(0));
    
    //initialising
    std::cout<<"===Initialising the bodies==="<<std::endl;


    if(DEMO)
    {
      const float BodyRadius[_N] = {
        50.0f,
        10.0f,
        15.0f,
        5.0f,
        20.0f,
        13.0f,
      };

      Position3D BodyPosition[_N] = {
        { 0.0f, 0.0f, -1000.0f },
        { 0.0f, 200.0f, -1000.0f },
        { -200.0f, 0.0f, -1000.0f },
        { 0.0f, 0.0f, -800.0f },
        { -100.0f, 0.0f, -2000.0f },
        { 100.0f, 0.0f, -500.0f }
      };
      Velocity3D BodyVelocity[_N] = {
        { 0.0f, 0.0f, 0.0f },
        { -30.0f, -30.0f, -30.0f },
        { 30.0f, 30.0f, 30.0f },
        { 45.0f, -30.0f, 15.0f },
        { -30.0f, 20.0f, -45.0f},
        { -30.0f, 20.0f, 10.0f}
      };
      Acceleration3D BodyAcceleration[_N] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f }
      };
      float BodyMass[_N] = {
        1e12f,
        1e1f,
        1e1f,
        2e1f,
        3e1f,
        2e1f
      };
    
      for(int i=0; i<_N; i++)
      {
          _bodies[i] = new Body(BodyPosition[i], BodyVelocity[i], BodyAcceleration[i], BodyMass[i], BodyRadius[i]);
      }
    }else{
      for(int i=0; i<_N; i++)
      {
          _bodies[i] = new Body(randomParticlePosition(),
                                 randomParticleVelocity(),
                                  randomParticleacceleration(),
                                  random_mass(),
                                  random_radius()
                                  );
      }
    }
    std::cout<<"============Done initialising the bodies============="<<std::endl;

    this->display_bodies();
        
}

void NBody::display_bodies()
{
  for( int i = 0; i < _N; i++ ) 
  {
    printf("\nBody %d:\nMass: %f\nPosition(x ,y, z): %f, %f, %f\nVelocity(x, y, z): %f, %f, %f\nAcceleration(x ,y, z): %f, %f, %f\n\n",
      i + 1, 
      _bodies[i]->_mass,
      _bodies[i]->_position._x, _bodies[i]->_position._y, _bodies[i]->_position._z,

      _bodies[i]->_velocity._x, _bodies[i]->_velocity._y, _bodies[i]->_velocity._z,
      _bodies[i]->_acceleration._x, _bodies[i]->_acceleration._y, _bodies[i]->_acceleration._z);
  }
}

void NBody::compute() 
{
    // display_bodies();
    // time computation
    const clock_t begin_time = clock();

    float t(0.);
    while(t<_tf)
    {
        std::cout<<"======== Updating for t "<<t<<"=============="<<std::endl;
        updatePhysics(t);
        t += _deltaT;
        this->display_bodies();
        
    }
    printf("Time Taken by Serial implementation: %f ms\n", float( clock () - begin_time ) /  CLOCKS_PER_SEC);
}

// Physics

void NBody::updateAcceleration(int bodyIndex ) 
{
   
  Force3D netForce = { 0, 0, 0 };

  for( int i = 0; i < _N; i++ ) 
  {
    if( i == bodyIndex ) 
    {
      continue; // skip
    }

    Force3D vectorForceToOther = {0, 0, 0};

    Force scalarForceBetween = Physics::ComputeForce(
                                  _bodies[bodyIndex]->_mass,
                                  _bodies[i]->_mass,
                                  _bodies[bodyIndex]->_position,
                                  _bodies[i]->_position,
                                  _bodies[i]->_velocity);

    debug(scalarForceBetween)
    
    
    direction( 
              _bodies[bodyIndex]->_position,
              _bodies[i]->_position,
              vectorForceToOther);

    debug(vectorForceToOther._x)
    debug(vectorForceToOther._y)
    debug(vectorForceToOther._z)

    vectorForceToOther._x *= scalarForceBetween;
    vectorForceToOther._y *= scalarForceBetween;
    vectorForceToOther._z *= scalarForceBetween;

    netForce._x += vectorForceToOther._x;
    netForce._y += vectorForceToOther._y;
    netForce._z += vectorForceToOther._z;

    debug(netForce._x)
    debug(netForce._y)
    debug(netForce._z)
  }

    //debug(netForce)

    _bodies[bodyIndex]->_acceleration = Physics::computeAcceleration3D(_bodies[bodyIndex]->_mass, netForce);

    //debug(_bodies[bodyIndex]->_acceleration)
    
}

void NBody::updateVelocity( int bodyIndex, float deltaT ) 
{
    _bodies[bodyIndex]->_velocity = Physics::computeVelocity3D(
                                _bodies[bodyIndex]->_acceleration,
                                _bodies[bodyIndex]->_velocity,
                                deltaT);

    debug(_bodies[bodyIndex]->_velocity._x)
    debug(_bodies[bodyIndex]->_velocity._y)
    debug(_bodies[bodyIndex]->_velocity._z)
}

void NBody::updatePosition( int bodyIndex, float deltaT ) 
{

  _bodies[bodyIndex]->_position = Physics::computePosition3D( 
                                _bodies[bodyIndex]->_velocity,
                                _bodies[bodyIndex]->_position,
                                deltaT);
}

void NBody::updatePhysics(float deltaT)
{
  for( int i = 0; i < _N; i++ ) 
  {
    updateAcceleration(i);
    updateVelocity(i, deltaT );
    updatePosition(i, deltaT );
  }
}



