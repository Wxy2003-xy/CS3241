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
int mouseButton = 0;
int moving, startx, starty;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
void drawCube() {
    glBegin(GL_LINE_LOOP);
        glNormal3f(0,0,1);
        glVertex3f(-1,-1, 1);
        glVertex3f( 1,-1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f(-1, 1, 1);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1, 1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1,-1,-1);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glNormal3f(1,0,0);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1,-1, 1);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glNormal3f(-1,0,0);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1,-1, 1);
        glVertex3f(-1, 1, 1);
        glVertex3f(-1, 1,-1);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glNormal3f(0,1,0);
        glVertex3f(-1, 1,-1);
        glVertex3f(-1, 1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1, 1,-1);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glNormal3f(0,-1,0);
        glVertex3f(-1,-1,-1);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1,-1, 1);
        glVertex3f(-1,-1, 1);
    glEnd();
}

// g++ one_rec_pyramid.cpp -std=c++11 -lGL -lGLU -lglut   (Linux/Windows)
// g++ one_rec_pyramid.cpp -std=c++11 -framework OpenGL -framework GLUT   (macOS)

#ifdef _WIN32
  #include <Windows.h>
  #include "GL/glut.h"
  #include "GL/glu.h"
#elif __APPLE__
  #include <GLUT/glut.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/glut.h>
  #include <GL/glu.h>
#endif

// ------- SINGLE function that draws everything and recurses ONCE -------
void drawPyramid(int n, float y = 0.0f) {
    glPushMatrix();
    for (int i = 0; i < n; i++) {
        glPushMatrix();
        for (int j = 0; j < n; j++) {
            glTranslated(0,0,2);
            drawCube();
        }
        glTranslated(2,0,0);
        glPopMatrix();
    }
    glTranslated(1,2,1);
    if (n>=0) {
    drawPyramid(n-1);
    }
    glPopMatrix();
}

// --------- usual GLUT glue (not part of the "one function" constraint) ----------
void reshape(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)w/(h?h:1), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
            glRotatef(angle2, 1.0f, 0.0f, 0.0f);
        glRotatef(angle,  0.0f, 1.0f, 0.0f);
        glScalef(zoom, zoom, zoom);
    gluLookAt(6,6,10,  0,2,0,  0,1,0);
    glRotatef(20, 0,1,0);
    drawPyramid(5);                 // <-- single recursive function
    glutSwapBuffers();
}




void
mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        mouseButton = button;
        moving = 1;
        startx = x;
        starty = y;
    }
    if (state == GLUT_UP) {
        mouseButton = button;
        moving = 0;
    }
}

void motion(int x, int y)
{
    if (moving) {
        if (mouseButton == GLUT_LEFT_BUTTON)
        {
            angle = angle + (x - startx);
            angle2 = angle2 + (y - starty);
        }
        else zoom += ((y - starty) * 0.001);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }

}


int main(int argc,char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutCreateWindow("One-Function One-Recursion Pyramid");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
        glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
