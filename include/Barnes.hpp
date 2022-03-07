#ifndef BARNES_H
#define BARNES_H

#include "NBody.hpp"

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief A class that implements the Barnes-hut approximation and integrates it to the visualisation
 *
 */

/**
 * @brief Node structure to represent Node object in a tree.
 *
 */
struct Node
{
    std::vector<Node> children;
    double totalMass;
    Vec3 centerMass;
    int numberOfParticle;
    Vec3 coord;
    int width;
    Body *particule = NULL;
};

/**
 * @brief Node propereties structure.
 *
 */
struct propNode
{
    double totalMass;
    Vec3 centerMass;
};

/**
 * @brief Class that implements the Barnes hut approximation.
 *
 */
class Barnes
{
private:
    struct Node _octree;

public:
    /**
     * @brief Create a Octree object
     * Based on our custom structure.
     * @param bodies
     */
    void createOctree(std::vector<Body *> bodies);
    /**
     * @brief Insert a leaf in the tree.
     *
     * @param body
     * @param octree
     */
    void insertInOctree(Body *body, struct Node octree);
    /**
     * @brief Determine the level and position of an object in the tree.
     *
     * @param body
     * @param octree
     * @return int
     */
    int determine(Body *body, Node octree);
    /**
     * @brief Divide the tree.
     *
     * @param octree
     * @return std::vector<Node>
     */
    std::vector<Node> subdivise(Node octree);
    /**
     * @brief Update the masses in the tree properties.
     *
     * @param octree
     * @return propNode
     */
    propNode updateMass(Node octree);
    /**
     * @brief Update the forces attribute in a simulation of bodies.
     *
     * @param bodies
     */
    void updateForces(std::vector<Body *> bodies);
    /**
     * @brief Update the forces in the tree properties.
     *
     * @param bodies
     */
    Force updateForce(Body *body, struct Node octree);
};

#endif