#include "NBody.hpp"



NBody::NBody(int N , float tf, int timeSteps): _N(N), _tf(tf), _timeSteps(timeSteps) 
{
    _deltaT = _tf / float(_timeSteps);
    _bodies.resize(_N);
    debug(_deltaT)
    //initialising
    for(int i=0; i<_N; i++)
    {
        _bodies[i] = new Body(randomParticlePos(), randomParticleVel(), Vec3(1.,1.,0.), 2., kMinRadius);

        _bodies[i] -> isString();
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
    for (int i = 0; i < _bodies.size(); ++i) 
    {
        for (int j = i + 1; j < _bodies.size(); ++j) 
        {
            if (_bodies[i]->_position == _bodies[i]->_position) {
                std::swap(_bodies[i]->_velocity, _bodies[j]->_velocity);
            }
        }
    }

}

void NBody::computeAccelerations() 
{
    for (int i = 0; i < _bodies.size(); ++i) 
    {
        _bodies[i]->_acceleration = Vec3(0.,0.,0.);
        for (int j = 0; j < _bodies.size(); ++j) {
            if (i != j) {
                double temp = G * _bodies[j]->_mass / pow((_bodies[i]->_position - _bodies[j]->_position).norm2(), 3);
                
                _bodies[i]->_acceleration = _bodies[i]->_acceleration + (_bodies[i]->_position - _bodies[j]->_position) * temp;
            }
        }
        }
   
}

void NBody::computeVelocities(float deltaT) {
    for (int i = 0; i < _bodies.size(); ++i) 
    {
        _bodies[i]->_velocity = computeVelo3D(
                                    _bodies[i]->_acceleration,
                                    _bodies[i]->_velocity,
                                    deltaT );
    }
}

void NBody::computePositions(float deltaT) {
    for (int i = 0; i < _bodies.size(); ++i) 
    {
        _bodies[i]->_velocity = computePos3D(
                                    _bodies[i]->_velocity,
                                    _bodies[i]->_position,
                                    deltaT );
    }
}



void NBody::cycle(float deltaT) {
    computeAccelerations();
    computePositions(deltaT);
    computeVelocities(deltaT);
    resolveCollisions();
}

void NBody::simulate()
{
    float t(0.);
    for(int i=0; i<_timeSteps; i++)
    {
        this->cycle(t);
        this->print_cycle(t);
        t += _deltaT;
    }
}



