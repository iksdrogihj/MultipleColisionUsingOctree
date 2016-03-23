//
//  main.cpp
//  MultipleColisionUsingOctree
//
//  Created by Todor Ivanov on 3/21/16.
//  Copyright © 2016 Todor Ivanov. All rights reserved.
//


#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "ball.hpp"
#include "Time.h"
#include "Octree.hpp"
using namespace std;

vector<Ball*> ballsInScene;

float randNumber()
{
    return (float)rand() / ((float)RAND_MAX + 1);
}

void generate20Balls()
{
    for (int i = 0; i < 5000; ++i)
    {
        Ball* ball = new Ball();
        ball->color = Vec3f(0.6 * randNumber() , 0.6 * randNumber(), 0.6 * randNumber());
        ball->velocity = Vec3f(8 * randNumber() - 4, 18 * randNumber()-4, 8 * randNumber()-4);
        ball->pos    = Vec3f ( 18 * randNumber(), 18 * randNumber(), 8 * randNumber());
        ballsInScene.push_back(ball);

    }
}

void moveBalls(vector <Ball*> ballsInScene, float dt)
{
    for( int i = 0; i < ballsInScene.size(); ++i)
    {
        ballsInScene.at(i)->pos += ballsInScene.at(i)->velocity * dt;
    }
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    GLfloat mat_specular[] = { 0.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, -1.0 };
  
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

float angle = 0;
void rotateCamera(float dt)
{

    angle+= 1 + dt;
    if (angle > 360) {
        angle -= 360;
    }
    
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 60.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(angle, 0, 1, 0);
    
    // Draws Balls
    for (int i = 0; i < ballsInScene.size(); ++i)
    {
        glPushMatrix();
        glTranslatef(ballsInScene.at(i)->pos[0], ballsInScene.at(i)->pos[1], ballsInScene.at(i)->pos[2]);
        glColor3f(ballsInScene.at(i)->color[0], ballsInScene.at(i)->color[1], ballsInScene.at(i)->color[2]);
        glutSolidSphere(1, 12, 12);
        glPopMatrix();
    }
    moveBalls(ballsInScene, Time::time_lapse());
    rotateCamera(Time::time_lapse());
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x,  int y){
   
    switch (key) {
        case 27:
            exit(0);
            break;
        case ' ':
            generate20Balls();
            cout << "Keybord " << endl;
            break;
        default:
            break;
    }
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90, w/h, 1, 1000);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Collision using Octree");
    srand(time(0));
    init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
   // glutTimerFunc(0.02, moveBalls(ballsInScene), Time::time_lapse()), 0);
    glutMainLoop();
    return 0;
}
