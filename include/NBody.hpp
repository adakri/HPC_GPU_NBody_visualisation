#ifndef NBODY_H
#define NBODY_H

#include "Physics.hpp"

/**
 * @brief Class implementing an aggregate of bodies and their respective animation directives and tools.
 *
 */

class NBody
{
private:
    int _N;
    float _tf;
    int _timeSteps;
    float _deltaT;

public:
    std::vector<Body *> _bodies;

    /**
     * @brief Construct a new NBody object
     *
     */
    NBody(int, float, int);
    /**
     * @brief Destroy the NBody object
     *
     */
    ~NBody()
    {
        for (int i = 0; _bodies.size(); i++)
        {
            delete _bodies[i];
        }
    }
    /**
     * @brief Print a frame of the simulation
     *
     */
    void print_sim();

    // Physics
    /**
     * @brief Display of the bodies
     *
     */
    void display_bodies();
    /**
     * @brief Encapsulates the sequence of update functions
     *
     */
    void compute();

    void updateAcceleration(int);
    void updateAcceleration2(int);
    void updateVelocity(int, float);
    void updatePosition(int, float);
    void updatePhysics(float);
    /**
     * @brief The Barnes_hut update physics function
     *
     */
    void updatePhysics2(float);

    // getters and setters
    int get_N() const { return _N; }
    int getTimeSteps() const { return _timeSteps; };
};

#endif
