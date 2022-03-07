#ifndef PHYSICS_H
#define PHYSICS_H

#include "random_init.hpp"
#include "Body.hpp"

/**
 * @brief Implements all the physical laws that we approximate to have a viable animation.
 *
 */

class Physics
{
public:
    /*
    The class is purely for an SE perspective, all the constructor/destructor/copy constructor are default and all the methods are static.
    */
    Physics() = default;
    ~Physics() = default;
    Physics(Physics &) = default;

    // Static functions for physics
    /**
     * @brief Computing the force between two bodies
     *
     * @return Force
     */
    static Force ComputeForce(Mass, Mass, Position3D, Position3D, Velocity3D &);

    /*
    Computing the scalar fields as in the project report.
    */
    static Acceleration computeAcceleration(Mass, Force);
    static Velocity computeVelocity(Acceleration, Velocity, Time);
    static Position computePosition(Velocity, Position, Time);

    /*
    Adding direction and creating a 3D physical field.
    */
    static Acceleration3D computeAcceleration3D(Mass, const Force3D &);
    static Velocity3D computeVelocity3D(Acceleration3D &, Velocity3D &, Time);
    static Position3D computePosition3D(Velocity3D &, Position3D &, Time);
};

#endif()