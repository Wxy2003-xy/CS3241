#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float xPos = -0.9f;
float speed = 0.01f;

void display() {
    // 🚫 no glClear(GL_COLOR_BUFFER_BIT) here
    glBegin(GL_POLYGON);
        glColor3f(1, 0, 0);
        glVertex2f(xPos - 0.1f, -0.1f);
        glVertex2f(xPos + 0.1f, -0.1f);
        glVertex2f(xPos + 0.1f,  0.1f);
        glVertex2f(xPos - 0.1f,  0.1f);
    glEnd();
    glFlush();
}

void timer(int value) {
    xPos += speed;
    if (xPos > 1.0f || xPos < -1.0f) speed = -speed;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("No glClear demo");

    // set black background (used if you *do* clear)
    glClearColor(0, 0, 0, 1);

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
