#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glColor3f(1, 1, 1); glVertex2f(-0.5f, -0.5f);
        glColor3f(0.5, 0, 1); glVertex2f(0.5f, -0.5f);
        glColor3f(1, 1, 1); glVertex2f(0.5f, 0.5f);
        glColor3f(1, 1, 1); glVertex2f(-0.5f, 0.5f);
    glEnd();
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
