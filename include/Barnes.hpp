#include "NBody.hpp"

#include <iostream>
#include <cstdlib>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct Node {
    std::vector<Node> children;
    double totalMass;
    Vec3 centerMass;
    int numberOfParticle;
    Vec3 coord;
    int width;
    Body* particule = NULL; 
};

struct propNode {
    double totalMass; 
    Vec3 centerMass; 
};

class Barnes {
    private: 
        struct Node _octree; 

    public:
        void createOctree(std::vector<Body*> bodies);
        void insertInOctree(Body* body, struct Node octree);
        int determine(Body* body, Node octree);
        std::vector<Node> subdivise(Node octree);
        propNode updateMass(Node octree);
        void updateForces(std::vector<Body*> bodies);
        Force updateForce(Body* body, struct Node octree);


};