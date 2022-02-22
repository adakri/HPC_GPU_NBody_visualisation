#include "Barnes.hpp"



Barnes::Barnes(){
    
}


void Barnes::createOctree(std::vector<Body*> bodies){
    for(int i=0; i< bodies.size(); i++){
        insertInOctree(bodies[i], _octree);
    }
   
}

int Barnes::determine(Body* body, std::vector<Node> children){
    for (int i=0; i<8; i++){
        
    }
}

std::vector<Node> Barnes::subdivise(Node octree){
    Vec3 center = octree.coord; 
    int width = octree.width; 
    int newWidth = width/2;
    std::vector<Node> children; 

    double x = center._x;
    double y = center._y; 
    double z = center._z;

    // haut bas gauche 
    Node hbg;
    Node hhg;
    Node hhd; 
    Node hbd;
    Node bbg;
    Node bhg;
    Node bhd; 
    Node bbd;

    hbg.width = newWidth;
    hhg.width = newWidth;
    hhd.width = newWidth;
    hbd.width = newWidth;
    bbg.width = newWidth;
    bhg.width = newWidth;
    bhd.width = newWidth;
    bbd.width = newWidth;

    hbg.coord = Vec3(x-newWidth/2, y+newWidth/2, z-newWidth/2);
    hhg.coord = Vec3(x-newWidth/2, y+newWidth/2, z+newWidth/2);
    hhd.coord = Vec3(x+newWidth/2, y+newWidth/2, z+newWidth/2);
    hbd.coord = Vec3(x+newWidth/2, y+newWidth/2, z-newWidth/2);
    bbg.coord = Vec3(x-newWidth/2, y+newWidth/2, z-newWidth/2);
    bhg.coord = Vec3(x-newWidth/2, y+newWidth/2, z+newWidth/2);
    bhd.coord = Vec3(x+newWidth/2, y+newWidth/2, z+newWidth/2);
    bbd.coord = Vec3(x+newWidth/2, y+newWidth/2, z-newWidth/2);

    children = {hbg, hhg, hhd, hbd, bbg, bhg, bhd, bbd};
    return children;


}

void Barnes::insertInOctree(Body* body, Node octree){
    if(octree.numberOfParticle > 1){
        int choice = determine(body, octree.children);
        insertInOctree(body, octree.children[choice]);
    }
    else if(octree.numberOfParticle == 1){
        //diviser l'octree
        octree.children = subdivise(octree);

        int choiceExistingParticule = determine(octree.particule, octree.children);
        insertInOctree(octree.particule, octree.children[choiceExistingParticule]);
        octree.particule = NULL;

        int choice = determine(body, octree.children);
        insertInOctree(body, octree.children[choice]);
        // determiner pour la particule deja presente
        // determiner pour la nouvelle particule
    }
    else if(octree.numberOfParticle == 0){
        octree.numberOfParticle += 1;
        octree.particule = body; 
        octree.totalMass += body->get_mass();
    }
}
    
void Barnes::deleteEmptyNode(Node octree){
    for (int i=0; i<8; i++){

    }

}



propNode Barnes::updateMass(Node octree){
    if(octree.numberOfParticle == 1){
        octree.totalMass = octree.particule->get_mass();
        octree.centerMass = octree.particule->get_position();
        return {octree.totalMass, octree.centerMass};
    }
    else if (octree.numberOfParticle == 0){
        octree.totalMass = 0;
        octree.centerMass == octree.coord; 
        return {octree.totalMass, octree.centerMass};
    }
    else {
        std::vector<propNode> propsNode;
        for(int i = 0; i<8; i++){
            propsNode.push_back(updateMass(octree.children[i]));
        }
        octree.totalMass = propsNode[0]+propsNode[1]+propsNode[2]+propsNode[3]+propsNode[4]+propsNode[5]+propsNode[6]+propsNode[7];
        octree.centerMass = octree.coord; // A CHANGER !!!!!!!
        return {octree.totalMass, octree.centerMass};
    }
    
}

void Barnes::updateForces(std::vector<Body*> bodies){
    for(int i=0; i<bodies.size(); i++){
        updateForce(bodies[i], _octree);
    }
}

double Barnes::updateForce(Body* body, Node octree){
    double force; 
    if (octree.numberOfParticle==1){
        // compute force avec la formule

        // update force pour le body
        return force;
    } else {
        double r; // distance particule i et centre de masse du noeud;
        double D = octree.width; 
        if (r/D < 2.){
            // compute avec totalMass et centre de masse; 

            // update force pour le body 
            return force;
        } else{
            for(int i = 0; i<8; i++){
                force += updateForce(body, octree.children[i]);
            }

            //update force pour le body
            return force;
        }
        
    }

}