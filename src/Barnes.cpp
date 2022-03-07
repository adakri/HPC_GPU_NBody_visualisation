#include "Barnes.hpp"

void Barnes::createOctree(std::vector<Body *> bodies)
{
    for (int i = 0; i < bodies.size(); i++)
    {
        insertInOctree(bodies[i], _octree);
    }
}

int Barnes::determine(Body *body, Node octree)
{
    double x = body->get_position().getX();
    double y = body->get_position().getY();
    double z = body->get_position().getZ();

    double x2 = octree.coord.getX();
    double y2 = octree.coord.getY();
    double z2 = octree.coord.getZ();

    for (int i = 0; i < 8; i++)
    {
        if (x < x2 && y > y2 && z < z2)
            return 0;
        if (x < x2 && y > y2 && z > z2)
            return 1;
        if (x > x2 && y > y2 && z < z2)
            return 2;
        if (x > x2 && y > y2 && z > z2)
            return 3;
        if (x < x2 && y < y2 && z < z2)
            return 4;
        if (x < x2 && y < y2 && z > z2)
            return 5;
        if (x > x2 && y < y2 && z < z2)
            return 6;
        if (x > x2 && y < y2 && z > z2)
            return 7;
    }
}

std::vector<Node> Barnes::subdivise(Node octree)
{
    Vec3 center = octree.coord;
    int width = octree.width;
    int newWidth = width / 2;
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

    hbg.coord = Vec3(x - newWidth / 2, y + newWidth / 2, z - newWidth / 2);
    hhg.coord = Vec3(x - newWidth / 2, y + newWidth / 2, z + newWidth / 2);
    hhd.coord = Vec3(x + newWidth / 2, y + newWidth / 2, z + newWidth / 2);
    hbd.coord = Vec3(x + newWidth / 2, y + newWidth / 2, z - newWidth / 2);
    bbg.coord = Vec3(x - newWidth / 2, y + newWidth / 2, z - newWidth / 2);
    bhg.coord = Vec3(x - newWidth / 2, y + newWidth / 2, z + newWidth / 2);
    bhd.coord = Vec3(x + newWidth / 2, y + newWidth / 2, z + newWidth / 2);
    bbd.coord = Vec3(x + newWidth / 2, y + newWidth / 2, z - newWidth / 2);

    children = {hbg, hhg, hhd, hbd, bbg, bhg, bhd, bbd};
    return children;
}

void Barnes::insertInOctree(Body *body, Node octree)
{
    if (octree.numberOfParticle > 1)
    {
        int choice = determine(body, octree);
        insertInOctree(body, octree.children[choice]);
    }
    else if (octree.numberOfParticle == 1)
    {
        // diviser l'octree
        octree.children = subdivise(octree);

        int choiceExistingParticule = determine(octree.particule, octree);
        insertInOctree(octree.particule, octree.children[choiceExistingParticule]);
        octree.particule = NULL;

        int choice = determine(body, octree);
        insertInOctree(body, octree.children[choice]);
        // determiner pour la particule deja presente
        // determiner pour la nouvelle particule
    }
    else if (octree.numberOfParticle == 0)
    {
        octree.numberOfParticle += 1;
        octree.particule = body;
        octree.totalMass += body->get_mass();
    }
}

propNode Barnes::updateMass(Node octree)
{
    if (octree.numberOfParticle == 1)
    {
        octree.totalMass = octree.particule->get_mass();
        octree.centerMass = octree.particule->get_position();
        return {octree.totalMass, octree.centerMass};
    }
    else if (octree.numberOfParticle == 0)
    {
        octree.totalMass = 0;
        octree.centerMass == octree.coord;
        return {octree.totalMass, octree.centerMass};
    }
    else
    {
        std::vector<propNode> propsNode;
        for (int i = 0; i < 8; i++)
        {
            propsNode.push_back(updateMass(octree.children[i]));
        }
        octree.totalMass = propsNode[0].totalMass + propsNode[1].totalMass + propsNode[2].totalMass + propsNode[3].totalMass + propsNode[4].totalMass + propsNode[5].totalMass + propsNode[6].totalMass + propsNode[7].totalMass;
        for (int i = 0; i < 8; i++)
        {
            octree.centerMass = octree.centerMass + propsNode[i].centerMass * propsNode[i].totalMass;
        }
        octree.centerMass = octree.centerMass * (1. / octree.totalMass);
        return {octree.totalMass, octree.centerMass};
    }
}

void Barnes::updateForces(std::vector<Body *> bodies)
{
    for (int i = 0; i < bodies.size(); i++)
    {
        updateForce(bodies[i], _octree);
    }
}

Vec3 Barnes::updateForce(Body *body, Node octree)
{
    Vec3 force;
    if (octree.numberOfParticle == 1)
    {
        Force scalarForce;
        scalarForce = Physics::ComputeForce(body->get_mass(), octree.particule->get_mass(), body->get_position(), octree.particule->get_position(), body->_velocity);

        direction(body->get_position(), octree.particule->get_position(), force);
        force = force * scalarForce;
        body->force = force;
        return force;
    }
    else if (octree.numberOfParticle == 0)
    {
        return Vec3(0, 0, 0);
    }
    else
    {
        Scalar delta_x = octree.centerMass._x - body->get_position()._x;
        Scalar delta_y = octree.centerMass._y - body->get_position()._y;
        Scalar delta_z = octree.centerMass._z - body->get_position()._z;
        Scalar r = sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z); // distance particule i et centre de masse du noeud;
        double D = octree.width;
        if (r / D < 2.)
        {
            Force scalarForce;

            scalarForce = Physics::ComputeForce(body->get_mass(), octree.totalMass, body->get_position(), octree.centerMass, body->_velocity);
            force = force * scalarForce;
            body->force = force;
            return force;
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                force = force + updateForce(body, octree.children[i]);
            }
            body->force = force;
            return force;
        }
    }
}
