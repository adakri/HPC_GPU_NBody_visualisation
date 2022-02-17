#include "Physics.hpp"

class NBody 
{
    private:
        int _N;
        float _tf;
        int _timeSteps;
        float _deltaT;
    public:
        std::vector<Body*> _bodies;
        
        NBody(int, float, int);
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
        void display_bodies();
        void compute();
        void updateAcceleration(int);
        void updateVelocity(int, float);
        void updatePosition(int, float);
        void updatePhysics(float);

        //drawing with SFML (or SDL)


        //getters and setters
        int get_N() const {return _N;}
        int getTimeSteps() const {return _timeSteps;}; 
};
    