#include "random_init.hpp"
#include "Body.hpp"



class Physics
{
    public:
        Physics()=default;
        ~Physics()=default;
        Physics(Physics&)=default;

        // Static functions for physics
        static Force ComputeForce(Mass, Mass, Position3D, Position3D);

        static Acceleration computeAcceleration( Mass, Force);
        static Velocity computeVelocity(Acceleration, Velocity, Time);
        static Position computePosition(Velocity, Position, Time);
        static Acceleration3D computeAcceleration3D(Mass, const Force3D &);
        static Velocity3D computeVelocity3D(Acceleration3D &, Velocity3D &, Time);
        static Position3D computePosition3D(Velocity3D &, Position3D &, Time);





};