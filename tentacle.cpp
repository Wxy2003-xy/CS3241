#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void drawUnitSquare() {
    glBegin(GL_POLYGON);
        glColor3f(1, 0, 0); glVertex2f(-1.0f, -1.0f);
        glColor3f(1, 0, 0); glVertex2f(1.0f, -1.0f);
        glColor3f(1, 0, 0); glVertex2f(1.0f, 1.0f);
        glColor3f(1, 0, 0); glVertex2f(-1.0f, 1.0f);
    glEnd();
    glFlush();
}

void drawTentacle(int n) {
    if (n == 0) return ;
    drawUnitSquare();
    glPushMatrix();
        glTranslatef(0, 2.0f, 0);
        glScalef(0.5f, 0.5f, 1.0f);
        glRotatef(10, 0, 0, 1);
        drawTentacle(n - 1);
    glPopMatrix();
}

void drawWheel(int n, int m) {
    if (n == 0) return ;
    drawTentacle(10);
    glPushMatrix();
        glRotatef(360/m, 0, 0, 1);
        drawWheel(n-1, m);
    glPopMatrix();
}

void display() {    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.1f, 0.1f, 1.0f);
    drawTentacle(100);
    glFlush();
}

void displayWheel() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.1f, 0.1f, 1.0f);
    drawWheel(8, 8);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("tut 1");
    glutDisplayFunc(displayWheel);
    glutMainLoop();
    return 0;
}
