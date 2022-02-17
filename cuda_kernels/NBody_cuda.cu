#include "Physics.cuh"

#include <vector>

class NBody_cuda 
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
        __device__
        void d_updateAcceleration(int, Position3D *,Acceleration3D *,Mass *);

        __device__
        void d_updateVelocity(int , float , Acceleration3D *, Velocity3D *); 


        __device__
        void d_updatePosition(int, float, Velocity3D *,Position3D *); 


        __global__
        void d_updatePhysics(int, float, Position3D *, Velocity3D *,Acceleration3D *,Mass *);

        //getters and setters
        int get_N() const {return _N;}
        int getTimeSteps() const {return _timeSteps;}; 
};
    