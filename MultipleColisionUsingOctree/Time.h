//
//  Time.h
//  MultipleColisionUsingOctree
//
//  Created by Todor Ivanov on 3/22/16.
//  Copyright © 2016 Todor Ivanov. All rights reserved.
//

#ifndef Time_h
#define Time_h

//=============================================================================
/// @brief Time class
///        class that will be used to calc dt
//=============================================================================
class Time{
public:
    //=============================================================================
    /// @brief  function that will calculate the time
    ///         between frames
    //=============================================================================
    static float time_lapse();
};

float Time::time_lapse()
{
    static float oldframe = 0;
    float timeCap = 0.25;
    float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    
    float deltaTime = (timeSinceStart - oldframe)/1000;
    std::cout << "delta Time " << deltaTime << std::endl;
    
    if (deltaTime > timeCap)
        deltaTime = timeCap;
    oldframe = timeSinceStart;
    return deltaTime;
}

#endif /* Time_h */
