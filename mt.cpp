#include <cmath>
#include <iostream>
#include <time.h>
#include <iostream>
#include <thread> // Required for std::this_thread::sleep_for
#include <chrono> // Required for std::chrono::milliseconds
#include <bits/stdc++.h>
#define GL_SILENCE_DEPRECATION


#ifdef _WIN32
#include <Windows.h>
#include "GL/glut.h"
#define M_PI 3.141592654
#endif
#ifdef __APPLE__
// #include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

using namespace std;

#define numStars 100
#define numPlanets 8

void drawCube2() {
    
}

void drawPyramid() {

}


void display() {    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                 // <-- add this line
    // gluLookAt(2,2,3,  0,0,0,  0,1,0);
    glRotatef(45,1,1,1);
    drawPyramid();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("tut 1");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}