#include "Physics.hpp"
#include "random_init.hpp"

class NBody 
{
    private:
        std::vector<Body*> _bodies;
        int _N;
        int _timeSteps;
    public:
        NBody(int N, int t);
        ~NBody() 
        { 
            for(int i=0; _bodies.size(); i++)
            {
                delete _bodies[i];
            }
        }

        void print_cycle(float);
        void print_sim();

        //Physics
        void resolveCollisions();
        void computeAccelerations();
        void computeVelocities();
        void computePositions();
        void getTimeSteps();
        void simulate();

        //getters and setters
        int get_N() const {return _N;}
        int getTimeSteps() const {return _timeSteps;}; 
};
    