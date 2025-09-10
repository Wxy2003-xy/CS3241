#include <bits/stdc++.h>

// CS3241 Assignment 1: Doodle
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

GLfloat PI = 3.14;
float alpha = 0.0, k=1;
float delta = 0.0, epsilon = 0.0;
float tx = 0.0, ty=0.0;
void drawAxes(float L = 1.0f) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // X (red)
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(L,0,0);
        // Y (green)
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,L,0);
        // Z (blue)
        glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,L);
    glEnd();
    glLineWidth(1.0f);

    // Optional axis labels (bitmap text)
    void *font = GLUT_BITMAP_HELVETICA_18;
    glColor3f(0,0,0);
    glRasterPos3f(L+0.05f, 0, 0); glutBitmapCharacter(font, 'X');
    glRasterPos3f(0, L+0.05f, 0); glutBitmapCharacter(font, 'Y');
    glRasterPos3f(0, 0, L+0.05f); glutBitmapCharacter(font, 'Z');
}

float to_radian(float deg) {
    return (float)(deg * M_PI / 180.0f);
}
inline float getX(float i, float j) {
    return cosf(to_radian(i)) * cosf(to_radian(j));
}
inline float getY(float i, float j) {
    return cosf(to_radian(i)) * sinf(to_radian(j));
}
inline float getZ(float i, float j) {
    return sinf(to_radian(i));
}
void drawSpherePlot(int res, float r) {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = -90; i < 90; i+=2) {
        float cp = cosf(to_radian(i));
        float sp = sinf(to_radian(i));
        for (int j = 0; j < 360; j+=2) {
            float x = r * cosf(to_radian(j)) * cp;
            float y = r * sp;
            float z = r * sinf(to_radian(j)) * cp;
            glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
            glVertex3f(x,y,z);
        }
    }
    glEnd();
}
void drawSpherePolygon(int step, float r, bool filled = false, bool outline = true) {
    for (int i = -90; i < 90; i+=step) {
        float cp = cosf(to_radian(i));
        float sp = sinf(to_radian(i));
        for (int j = 0; j < 360; j+=step) {
            glBegin(GL_LINE_LOOP);
            {
                float x = getX(i, j);
                float y = getY(i, j);
                float z = getZ(i, j);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i+step, j);
                float y = getY(i+step, j);
                float z = getZ(i+step, j);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i, j+step);
                float y = getY(i, j+step);
                float z = getZ(i, j+step);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            glEnd();
            glBegin(GL_LINE_LOOP);
            {
                float x = getX(i+step, j);
                float y = getY(i+step, j);
                float z = getZ(i+step, j);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i, j+step);
                float y = getY(i, j+step);
                float z = getZ(i, j+step);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i+step, j+step);
                float y = getY(i+step, j+step);
                float z = getZ(i+step, j+step);
                glColor3f(fabsf(x)/r, fabsf(y)/r, fabsf(z)/r);
                glVertex3f(x,y,z);
            }
            glEnd();
        }
    }
}

void reshape(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (h == 0) ? 1.0f : (float)w / (float)h;
    float f = 1.0f / tanf(to_radian(60.0f) * 0.5f);
    GLfloat P[16] = {
        f/aspect,0,0,0,
        0,f,0,0,
        0,0,-(100.0f+0.1f)/(100.0f-0.1f),-1,
        0,0,(-2*100.0f*0.1f)/(100.0f-0.1f),0
    };
    glMultMatrixf(P);
}
void displayPlot() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Camera only
    glTranslatef(0.0f, 0.0f, -3.0f);
    // Draw axes at world origin (not affected by your object transforms)
    drawAxes(1.0f);
    // Now apply your object transforms
    glPushMatrix();
		// after your object transforms, before drawing the sphere:
		glTranslatef(tx, ty, 0.0f);
		glRotatef(alpha, 0,1,0);
		glRotatef(delta, 1,0,0);
		glRotatef(epsilon, 0,0,1);
		glScalef(k,k,k);
		drawAxes(1.0f);
		glPushMatrix();
			glScalef(1,1,1);
			drawSpherePlot(10,0.8f);
    	glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void displayPolygon() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Camera only
    glTranslatef(0.0f, 0.0f, -3.0f);
    // Draw axes at world origin (not affected by your object transforms)
    drawAxes(1.0f);
    // Now apply your object transforms
    glPushMatrix();
		// after your object transforms, before drawing the sphere:
		glTranslatef(tx, ty, 0.0f);
		glRotatef(alpha, 0,1,0);
		glRotatef(delta, 1,0,0);
		glRotatef(epsilon, 0,0,1);
		glScalef(k,k,k);
		drawAxes(1.0f);          // <- follows the object
		glPushMatrix();
			glScalef(1,1,1);
			drawSpherePolygon(10,0.8f);
    	glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}



void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;

		case 'j':
			delta += 10;
			glutPostRedisplay();
		break;

		case 'l':
			delta -= 10;
			glutPostRedisplay();
		break;
		case 'i':
			epsilon += 10;
			glutPostRedisplay();
		break;		
		case 'm':
			epsilon -= 10;
			glutPostRedisplay();
		break;
        case 27:
            exit(0);

		default:
		break;
	}
}


int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 1\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(displayPolygon);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc([]{ alpha += 0.2f; delta += 0.2; epsilon += 0.2; glutPostRedisplay(); });
	glutMainLoop();

	return 0;
}
