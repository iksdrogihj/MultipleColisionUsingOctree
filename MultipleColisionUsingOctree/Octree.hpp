//
//  Octree.hpp
//  MultipleColisionUsingOctree
//
//  Created by Todor Ivanov on 3/21/16.
//  Copyright © 2016 Todor Ivanov. All rights reserved.
//

#ifndef Octree_hpp
#define Octree_hpp

#include <stdio.h>
#include "ball.hpp"
#include <vector>

//=============================================================================
/// @brief  Struct that represents the bounding box of a single Octant
///
//=============================================================================
struct BoundingBox
{
    //=========================================================================
    /// @var  minCorner
    /// @brief A vector that holds the values with the lesser part of the cube
    //         in the 3 dimensions. (i.e. minX to center, minY to center,
    //         minZ to center)
    //=========================================================================
    Vec3f minCorner;
    
    //=========================================================================
    /// @var  maxCorner
    /// @brief A vector that holds the values with the larger part of the cube
    //         in the 3 dimensions. (i.e. center to maxX, center to maxY,
    //         center to maxZ)
    //=========================================================================
    Vec3f maxCorner;
    
    //=========================================================================
    /// @var  center
    /// @brief A vector that stores the middle of the Cube in the 3 dimensions
    //         (i.e (for x(minCorner[0] + maxCorner[0])/2,
    //               for y(minCorner[1] + maxCorner[1])/2,
    //               for z(minCorner[2] + maxCorner[2])/2))
    //=========================================================================
    Vec3f center;
};

class Octree
{
public:
    // ctor
    Octree (Vec3f cornerMin, Vec3f cornerMax, int depth);
    
    //=============================================================================
    /// @var children
    /// @brief The children of this, if this has any.  children[0][*][*] are the
    /// children with x coordinates ranging from minX to centerX.
    /// children[1][*][*] are the children with x coordinates ranging from
    /// centerX to maxX.  Similarly for the other two dimensions of the
    /// children array.
    //=============================================================================
    Octree* children[2][2][2];
    
    //=============================================================================
    /// @var  boundingBox
    /// @brief The coords of the bounding cube of the node
    //=============================================================================
    BoundingBox boundingBox;
    
    //=============================================================================
    /// @var  MAX_OCTREE_DEPTH
    /// @brief A constant value that we will check the depth of a tree MUST NOT
    ///        go over this number as the balls will apear in too many Nodes
    //=============================================================================
    const int MAX_OCTREE_DEPTH = 6;
    
    //=============================================================================
    /// @var  MIN_BALLS_IN_NODE
    /// @brief If the balls in a single node drop bellow this number
    ///        there is no point in subdividing it.
    /// @note The number of balls that we will Brute Force check in a single node
    ///       will be between 3 and 6
    //=============================================================================
    const int MIN_BALLS_IN_NODE = 3;
    
    //=============================================================================
    /// @var  MAX_BALLS_PER_OCTREE
    /// @brief If the balls in a single node go over this number subdivide the node
    //=============================================================================
    const int MAX_BALLS_PER_OCTREE = 6;
    
    //=============================================================================
    /// @var MIN_BOUND_SIZE
    /// @brief The bounding Box cannot go under this constans
    ///        otherwise the ball will apear in too many nodes
    //=============================================================================
    const int MIN_BOUND_SIZE = 1;
    
    //=============================================================================
    /// @var hasChildren
    /// @brief Bool value that will keep state wheter a given node has child nodes
    //=============================================================================
    bool hasChildren;
    
    //=============================================================================
    /// @var numBalls
    /// @brief The number of balls in the current Node
    //=============================================================================
    int numBalls;
    
    //=============================================================================
    /// @var depth
    /// @brief The depth of this in the tree
    //=============================================================================
    int depth;
    
  //  bool addBall;

    //=============================================================================
    /// @var ballsInTree
    /// @brief A Vector that stores all the balls in the current Node
    //=============================================================================
    std::vector <Ball*> ballsInTree;
    
    //=============================================================================
    /// @fn MapBallToBoundingBox
    /// @brief Function that will, after breaking down of a node into 8 octants,
    ///         decide which ball belongs to which node
    /// @param ball - a ball that will be moved in a node
    /// @param pos -
    /// @param addBall - a Bool which will indicate wheter we should add or remove
    ///                  a ball
    //=============================================================================
    void MapBallToBoundingBox(Ball* ball, Vec3f pos, bool addBall);
    
    //=============================================================================
    /// @fn PartitionSpace
    /// @brief Function partitions a space into 8 parts
    //=============================================================================
    void PartitionSpace();
    
    void add(Ball* ball);
    
    void remove(Ball* ball, Vec3f pos);
    
    //Destroys the children of this, and moves all balls in its descendants
    //to the "balls" set
    void destroyChildren();
    //=============================================================================
    /// @fn potentialBallBallCollisions
    /// @brief Gathers all the balls in the node and puts them in the collisions vec
    /// @param collisions - a vector that will contain a pair of Ball objects that
    ///                     could potentially collide
    //=============================================================================
    void potentialBallBallCollisions(std::vector<BallPair> &collisions);
    
};

#endif /* Octree_hpp */
