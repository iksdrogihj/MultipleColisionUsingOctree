//
//  ball.hpp
//  MultipleColisionUsingOctree
//
//  Created by Todor Ivanov on 3/23/16.
//  Copyright © 2016 Todor Ivanov. All rights reserved.
//

#ifndef ball_hpp
#define ball_hpp

#include <stdio.h>
#include "Vec3f.hpp"

//=============================================================================
/// @brief Ball structure
/// Structure containing general information about a single ball
//=============================================================================
struct Ball
{
    Vec3f velocity;
    Vec3f pos;
    Vec3f color;
    float r;
};

//=============================================================================
/// @brief BallPair structure
/// This structure contains pointers to balls that
/// could potentially collide
//=============================================================================
struct BallPair
{
    Ball* ball1;
    Ball* ball2;
};
#endif /* ball_hpp */
