#include "NBody.hpp"



NBody::NBody(int N, int t): _N(N), _timeSteps(t) 
{
    for(int i=0; i<_N; i++)
    {
        _bodies[i]->_position = randomParticlePos();
        _bodies[i]->_velocity = randomParticleVel();
        _bodies[i]->_acceleration = Vec3(0.,0.,0.);
    }
        
}

void NBody::print_cycle(float t)
{
    printf("Cycle %2.2f :  \n");
    for (int i = 0; i < _N; i++)
    {
        printf(
        " Body %d : Mass %3.3f and Radius %3.3f - Position ( %8.4f , %8.4f )- Velocity ( %8.4f , %8.4f ) - Acceleration ( %8.4f , %8.4f ) \n)" 
        , _bodies[i]->_mass, _bodies[i]->_radius, 
        _bodies[i]->_position._x, _bodies[i]->_position._y, 
        _bodies[i]->_velocity._x, _bodies[i]->_velocity._y,
        _bodies[i]->_acceleration._x, _bodies[i]->_acceleration._y
        );
    }
    
}

void NBody::resolveCollisions() 
{

}

void NBody::computeAccelerations() 
{
   
}

void NBody::computeVelocities() {
    
}

void NBody::computePositions() {
    
}



void NBody::simulate() {
    computeAccelerations();
    computePositions();
    computeVelocities();
    resolveCollisions();
}



