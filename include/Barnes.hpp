#include "Physics.hpp"
#include "NBody.hpp"

#include <iostream>
#include <cstdlib>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct Node {
    std::vector<Node> children;
    float totalMass;
    Vec3 centerMass;
    int numberOfParticle;
    Vec3 coord;
    int width;
};

class Barnes {
    private: 
        struct Node _octree; 

    public:
        void createOctree(std::vector<Body*> bodies);
        void insertInOctree(Body* body, struct Node octree);
        int determine(Body* body, std::vector<Node>);
        void deleteEmptyNode(struct Node octree);
        void updateMass();
        void updateForces(std::vector<Body*> bodies);
        void updateForce(Body* body, struct Node octree);


};