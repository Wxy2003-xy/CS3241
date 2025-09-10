

// CS3241 Assignment 1: Doodle
////////////
// Name: Wang Xiyu A0282500R
// Extra functions: glutIdleFunc, glDepthFunc
////////////
#include <cmath>
#include <iostream>
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
float alpha_in = 0.0;
float alpha_min = 0.0;
float alpha_hr = 0.0;

float tx = 0.0, ty=0.0;
inline float to_radian(float deg) {
    return (float)(deg * PI / 180.0f);
}
void drawCircle(float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.f,0.2f,0.1f);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// 360 % interval == 0
// inteval % 2 == 0
// rr < r
void drawBackground(float r, float rr, int interval) {
    for (int i = 0; i < 360; i+=interval) {
        glBegin(GL_TRIANGLES);
        float x = r * cosf(to_radian(i));
        float y = r * sinf(to_radian(i));
        float xx = r * cosf(to_radian(i+interval));
        float yy = r * sinf(to_radian(i+interval));
        float xxx = rr * cosf(to_radian(i+(interval/2)));
        float yyy = rr * sinf(to_radian(i+(interval/2)));
        glColor3f(1.f,0.8f,0.1f);
        glVertex2f(x,y);
        glVertex2f(xx,yy);
        glVertex2f(xxx,yyy);
        glEnd();

        glBegin(GL_TRIANGLES);
        float x_ = rr * cosf(to_radian(i+(interval/2)));
        float y_ = rr * sinf(to_radian(i+(interval/2)));
        float xx_ = r * cosf(to_radian(i+interval));
        float yy_ = r * sinf(to_radian(i+interval));
        float xxx_ = rr * cosf(to_radian(i+(3*interval/2)));
        float yyy_ = rr * sinf(to_radian(i+(3*interval/2)));
        glColor3f(0.8f,0.6f,0.1f);
        glVertex2f(x_,y_);
        glVertex2f(xx_,yy_);
        glVertex2f(xxx_,yyy_);
        glEnd();
    }
}

void drawScale(float r, float rr) {
    for (int i = 0; i < 360; i+=5) {
        if (i % 6 == 0) {
            glBegin(GL_TRIANGLES);
            float rrr = 0.8*rr;
            float x = r * cosf(to_radian(i));
            float y = r * sinf(to_radian(i));
            float xx = r * cosf(to_radian(i+1.8f));
            float yy = r * sinf(to_radian(i+1.8f));
            float xxx = rrr * cosf(to_radian(i+(0.9f)));
            float yyy = rrr * sinf(to_radian(i+(0.9f)));
            glColor3f(0.6f,0.1f,0.3f);
            glVertex2f(x,y);
            glVertex2f(xx,yy);
            glVertex2f(xxx,yyy);
            glEnd();
        } else {
            glBegin(GL_TRIANGLES);
            float x = r * cosf(to_radian(i));
            float y = r * sinf(to_radian(i));
            float xx = r * cosf(to_radian(i+1.2f));
            float yy = r * sinf(to_radian(i+1.2f));
            float xxx = rr * cosf(to_radian(i+(0.6f)));
            float yyy = rr * sinf(to_radian(i+(0.6f)));
            glColor3f(0.1f,0.1f,0.5f);
            glVertex2f(x,y);
            glVertex2f(xx,yy);
            glVertex2f(xxx,yyy);
            glEnd();
        }
    }
}

void drawPin() {
    glBegin(GL_TRIANGLES);
        glColor3f(0,0,0);
        glVertex2f(-0.3, 0);
        glVertex2f(0, 3);
        glVertex2f(0.3, 0);
    glEnd();
}

void drawPin2() {
    glBegin(GL_TRIANGLES);
        glColor3f(0,0,0);
        glVertex2f(-0.2, 0);
        glVertex2f(0, 4);
        glVertex2f(0.2, 0);
    glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
        //controls transformation
        glScalef(k, k, k);	
        glTranslatef(tx, ty, 0);	
        glRotatef(alpha, 0, 0, 1);
        //draw your stuff here (Erase the triangle code)
        drawBackground(6, 4.4, 30);
        glPushMatrix();
            glTranslatef(0,0,-0.2f);
            drawScale(4.3f, 3.6f);
        glPopMatrix();
        glPushMatrix();
            glRotatef(alpha_in, 0, 0, 1);
            drawPin();
        glPopMatrix();
        glPushMatrix();
            glRotatef(alpha_min, 0, 0, 1);
            drawPin2();
        glPopMatrix();
        glPushMatrix(); 
            drawCircle(0.4, 30);
        glPopMatrix();
        glPushMatrix();
            glScalef(0.32f, 0.32f, 0.32f);	
            glTranslatef(0,-7.3f,-0.1f);
            drawBackground(6, 4.4, 30);
            glPushMatrix();
                glTranslatef(0,0,-0.2f);
                drawScale(4.3f, 3.6f);
            glPopMatrix();
            glPushMatrix();
                glRotatef(alpha_in*30, 0, 0, 1);
                drawPin();
            glPopMatrix();
            glPushMatrix();
                glRotatef(alpha_min*6, 0, 0, 1);
                drawPin2();
            glPopMatrix();
            glPushMatrix();
                drawCircle(0.4, 30);
            glPopMatrix();
        glPopMatrix();
	glPopMatrix();
    glutSwapBuffers();
}

// void reshape (int w, int h)
// {
// 	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
// 	glMatrixMode (GL_PROJECTION);
// 	glLoadIdentity();
// 	glOrtho(-10, 10, -10, 10, -10, 10);
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// }

void reshape(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (h == 0) ? 1.0f : (float)w / (float)h;
    float f = 1.0f / tanf(to_radian(60.0f) * 0.5f);
    GLfloat P[9] = {
        f/aspect,0,0,
        0,f,0,
        0,0,(-2*100.0f*0.1f)/(100.0f-0.1f)
    };
    glMultMatrixf(P);
}

void init(void)
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
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
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc([]{ alpha_in -= 0.1f;
                    alpha_min -= 6.f;
                    alpha_hr -= 360.f;
    glutPostRedisplay(); });
	glutMainLoop();

	return 0;
}
