#include "Barnes.hpp"



Barnes::Barnes(){
    
}


void Barnes::createOctree(std::vector<Body*> bodies){
    for(int i=0; i< bodies.size(); i++){
        insertInOctree(bodies[i], _octree);
    }
    deleteEmptyNode(_octree);
}

int Barnes::determine(Body* body, std::vector<Node> children){
    for (int i=0; i<8; i++){

    }
}

void Barnes::insertInOctree(Body* body, Node octree){
    if(octree.numberOfParticle > 1){
        int choice = determine(body, octree.children);
        insertInOctree(body, octree.children[choice]);
    }
    else if(octree.numberOfParticle == 1){
        //diviser l'octree
        // determiner pour la particule deja presente
        
    }
    else if(octree.numberOfParticle == 0){
        octree.numberOfParticle += 1;
        octree.totalMass += body->get_mass();

    }
}
    
void Barnes::deleteEmptyNode(Node octree){
    for (int i=0; i<8; i++){

    }

}


void Barnes::updateMass(){
    s
}