#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <bits/stdc++.h>

void draw3DCubeWireFrame() {
    glColor3f(255,255,255);
    glPushMatrix();

    // 4 side faces
    for(int i=0;i<4;i++) {
        glBegin(GL_LINE_LOOP);
            glVertex3f(0.5, 0.5, 0.5);
            glVertex3f(-0.5, 0.5, 0.5);
            glVertex3f(-0.5,-0.5, 0.5);
            glVertex3f(0.5,-0.5, 0.5);
        glEnd();
        glRotatef(90,0,1,0);
    }

    // top face
    glBegin(GL_LINE_LOOP);
        glVertex3f( 0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5,-0.5);
        glVertex3f( 0.5, 0.5,-0.5);
    glEnd();

    // bottom face
    glBegin(GL_LINE_LOOP);
        glVertex3f( 0.5,-0.5, 0.5);
        glVertex3f(-0.5,-0.5, 0.5);
        glVertex3f(-0.5,-0.5,-0.5);
        glVertex3f( 0.5,-0.5,-0.5);
    glEnd();

    glPopMatrix();
}


void display() {    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                 // <-- add this line
    // gluLookAt(2,2,3,  0,0,0,  0,1,0);
    glRotatef(45,1,1,1);
    draw3DCubeWireFrame();
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