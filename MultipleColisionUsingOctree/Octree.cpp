//
//  Octree.cpp
//  MultipleColisionUsingOctree
//
//  Created by Todor Ivanov on 3/21/16.
//  Copyright © 2016 Todor Ivanov. All rights reserved.
//

#include "Octree.hpp"

Octree::Octree(Vec3f c1, Vec3f c2, int d) {
    boundingBox.minCorner = c1;
    boundingBox.maxCorner = c2;
    boundingBox.center = (c1 + c2) /2;
    depth = d;
    numBalls = 0;
    hasChildren = false;
}

void Octree::MapBallToBoundingBox(Ball *ball, Vec3f pos, bool addBall)
{
    //Figure out in which child(ren) the ball belongs
    for(int x = 0; x < 2; ++x)
    {
        if (x == 0)
        {
            if (pos[0] - ball->r > boundingBox.center[0])
            {
                continue;
            }
        }
        else if (pos[0] + ball->r < boundingBox.center[0])
        {
            continue;
        }
        
        for(int y = 0; y < 2; ++y)
        {
            if (y == 0)
            {
                if (pos[1] - ball->r > boundingBox.center[1])
                {
                    continue;
                }
            }
            else if (pos[1] + ball->r < boundingBox.center[1])
            {
                continue;
            }
            
            for(int z = 0; z < 2; ++z)
            {
                if (z == 0)
                {
                    if (pos[2] - ball->r > boundingBox.center[2])
                    {
                        continue;
                    }
                }
                else if (pos[2] + ball->r < boundingBox.center[2])
                {
                    continue;
                }
                
                //Add or remove the ball
                if (addBall)
                {
                    children[x][y][z]->add(ball);
                }
                else
                {
                    children[x][y][z]->remove(ball, pos);
                }
            }
        }
    }
}


void Octree::PartitionSpace()
{
    for(int x = 0; x < 2; ++x)
    {
        float minX;
        float maxX;
        if (x == 0)
        {
            minX = boundingBox.minCorner[0];
            maxX = boundingBox.center[0];
        }
        else
        {
            minX = boundingBox.center[0];
            maxX = boundingBox.maxCorner[0];
        }
        
        for(int y = 0; y < 2; ++y)
        {
            float minY;
            float maxY;
            if (y == 0)
            {
                minY = boundingBox.minCorner[1];
                maxY = boundingBox.center[1];
            }
            else
            {
                minY = boundingBox.center[1];
                maxY = boundingBox.maxCorner[1];
            }
            
            for(int z = 0; z < 2; ++z)
            {
                float minZ;
                float maxZ;
                if (z == 0)
                {
                    minZ = boundingBox.minCorner[2];
                    maxZ = boundingBox.center[2];
                }
                else
                {
                    minZ = minZ = boundingBox.center[2];
                    maxZ = minZ = boundingBox.maxCorner[2];
                }
                
                children[x][y][z] = new Octree(Vec3f(minX, minY, minZ),
                                               Vec3f(maxX, maxY, maxZ),
                                               depth + 1);
            }
        }
    }
    
    //Remove all balls from "ballsInTree" and add them to the new children
    for(int i = 0; i < ballsInTree.size(); ++i)
    {
        MapBallToBoundingBox(ballsInTree.at(i), ballsInTree.at(i)->pos, true);
    }
    ballsInTree.clear();
    hasChildren = true;
}

//Adds a ball to this
void Octree::add(Ball* ball)
{
    numBalls++;
    if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
        numBalls > MAX_BALLS_PER_OCTREE) {
        PartitionSpace();
    }
    
    if (hasChildren) {
        MapBallToBoundingBox(ball, ball->pos, true);
    }
    else {
        ballsInTree.push_back(ball);
    }
}

//Removes the specified ball at the indicated position
void Octree::remove(Ball* ball, Vec3f pos)
{
    numBalls--;
    
    if (hasChildren && numBalls < MIN_BALLS_IN_NODE) {
        destroyChildren();
    }
    
    if (hasChildren) {
        MapBallToBoundingBox(ball, pos, false);
    }
    else {
        ballsInTree.pop_back();
    }
}

//Destroys the children of this, and moves all balls in its descendants
//to the "balls" set
void Octree::destroyChildren() {
    //Move all balls in descendants of this to the "balls" set
    // TODO collectBalls(balls);
    
    for(int x = 0; x < 2; ++x) {
        for(int y = 0; y < 2; ++y) {
            for(int z = 0; z < 2; ++z) {
                delete children[x][y][z];
            }
        }
    }
    
    hasChildren = false;
}
//Adds potential ball-ball collisions to the specified set
void Octree::potentialBallBallCollisions(std::vector<BallPair> &collisions) {
    if (hasChildren) {
        for(int x = 0; x < 2; ++x) {
            for(int y = 0; y < 2; ++y) {
                for(int z = 0; z < 2; ++z) {
                    children[x][y][z]->
                    potentialBallBallCollisions(collisions);
                }
            }
        }
    }
    else {
        //Add all pairs (ball1, ball2) from balls
        for(int i = 0; i < ballsInTree.size(); ++i)
        {
            for(int j = 0; j < ballsInTree.size(); ++j) {
                
                //This test makes sure that we only add each pair once
                if (ballsInTree.at(i) < ballsInTree.at(j)) {
                    BallPair bp;
                    bp.ball1 = ballsInTree.at(i);
                    bp.ball2 = ballsInTree.at(j);
                    collisions.push_back(bp);
                }
            }
        }
    }
}
