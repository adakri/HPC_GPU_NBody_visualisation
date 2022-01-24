#include "NBody.hpp"



NBody::NBody(int N , float tf, int timeSteps): _N(N), _tf(tf), _timeSteps(timeSteps) 
{
    _deltaT = _tf / float(_timeSteps);
    _bodies.resize(_N);

    /* initialize random seed: */
    srand (time(0));
    
    //initialising
    std::cout<<"===Initialising the bodies==="<<std::endl;
    for(int i=0; i<_N; i++)
    {
        _bodies[i] = new Body(randomParticlePos(), randomParticleVel(), Vec3(0.,0.,0.), random_mass(), random_radius());

        _bodies[i] -> isString();
    }
    std::cout<<"========================="<<std::endl;
        
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
    Force scalarForceBetween = forceNewtonianGravity3D(
                                  _bodies[bodyIndex]->_mass,
                                  _bodies[i]->_mass,
                                  _bodies[bodyIndex]->_position,
                                  _bodies[i]->_position);

    //debug(scalarForceBetween)
    direction( 
      _bodies[bodyIndex]->_position,
      _bodies[i]->_position,
      vectorForceToOther);

    vectorForceToOther._x *= scalarForceBetween;
    vectorForceToOther._y *= scalarForceBetween;
    vectorForceToOther._z *= scalarForceBetween;
    netForce._x += vectorForceToOther._x;
    netForce._y += vectorForceToOther._y;
    netForce._z += vectorForceToOther._z;
  }

    //debug(netForce)

    _bodies[bodyIndex]->_acceleration = computeAccel3D(_bodies[bodyIndex]->_mass, netForce);
    //debug(_bodies[bodyIndex]->_acceleration);
}

void NBody::updateVelocity( int bodyIndex, float deltaT ) 
{
    _bodies[bodyIndex]->_velocity = computeVelo3D(
                                _bodies[bodyIndex]->_acceleration,
                                _bodies[bodyIndex]->_velocity,
                                deltaT);
    //debug(_bodies[bodyIndex]->_velocity)
}

void NBody::updatePosition( int bodyIndex, float deltaT ) 
{

  _bodies[bodyIndex]->_position = computePos3D( 
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



