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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

GLfloat PI = 3.14;
float alpha = 0.0, k=1;
float delta = -67.5, epsilon = 0.0;
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
inline float U(int j) { return j / 360.0f; }                   // s (wraps around)
inline float V(int i) { return (i + 90.0f) / 180.0f; }         // t (south..north)

GLuint texEarth = 0, texCloud = 0, currentTex = 0, texMoon = 0, texBg = 0;
int winW = 600, winH = 600;

GLuint loadTexture2D(const char* path, bool flip=true) {
    int w,h,n;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path, &w,&h,&n, 0);
    if (!data) { fprintf(stderr, "stb_image failed for %s\n", path); return 0; }

    GLenum fmt = (n == 4) ? GL_RGBA : GL_RGB;

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        // wrap horizontally
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // clamp vertically

    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return id;
}

void drawBackground() {
    if (!texBg) return;

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    glOrtho(0, winW, 0, winH, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texBg);
    glColor4f(1,1,1,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,    0);
        glTexCoord2f(1,0); glVertex2f(winW, 0);
        glTexCoord2f(1,1); glVertex2f(winW, winH);
        glTexCoord2f(0,1); glVertex2f(0,    winH);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
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

void drawSpherePolygon(int step, float r, bool filled = false, bool outline = true) {
    for (int i = -90; i < 90; i+=step) {
        float cp = cosf(to_radian(i));
        float sp = sinf(to_radian(i));
        for (int j = 0; j < 360; j+=step) {
            glBegin(GL_TRIANGLES);
            {
                float x = getX(i, j);
                float y = getY(i, j);
                float z = getZ(i, j);
                float u = U(j);
                float v = V(i);
                glTexCoord2f(u, v);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i+step, j);
                float y = getY(i+step, j);
                float z = getZ(i+step, j);
                float u = U(j);
                float v = V(i+step);
                glTexCoord2f(u, v);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i, j+step);
                float y = getY(i, j+step);
                float z = getZ(i, j+step);
                float u = U(j+step);
                float v = V(i);
                glTexCoord2f(u, v);
                glVertex3f(x,y,z);
            }
            glEnd();
            glBegin(GL_TRIANGLES);
            {
                float x = getX(i+step, j);
                float y = getY(i+step, j);
                float z = getZ(i+step, j);
                float u = U(j);
                float v = V(i+step);
                glTexCoord2f(u, v);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i, j+step);
                float y = getY(i, j+step);
                float z = getZ(i, j+step);
                float u = U(j+step);
                float v = V(i);
                glTexCoord2f(u, v);
                glVertex3f(x,y,z);
            }
            {
                float x = getX(i+step, j+step);
                float y = getY(i+step, j+step);
                float z = getZ(i+step, j+step);
                float u = U(j+step);
                float v = V(i+step);
                glTexCoord2f(u, v);
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

void displayPolygon() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    drawBackground();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glPushMatrix();
		glTranslatef(tx, ty, 0.0f);
		glRotatef(alpha, 0,1,0);
		glRotatef(delta, 1,0,0);
		glRotatef(epsilon, 0,0,1);
		glScalef(k,k,k);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			glScalef(1,1,1);
            glColor3f(1.0f, 1.0f, 1.0f);
            glBindTexture(GL_TEXTURE_2D, texEarth);  
            drawSpherePolygon(5, 0.8f);
    	glPopMatrix();
        glDepthMask(GL_FALSE);
        glPushMatrix();
            glScalef(1.02,1.02,1.02);
            glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
            glRotatef(0.8*alpha, 0,0,1);
            glBindTexture(GL_TEXTURE_2D, texCloud);  
            drawSpherePolygon(5, 0.8f);
        glPopMatrix();
        glDepthMask(GL_TRUE);
        glPushMatrix();
            glTranslatef(2.2f, 0, 0);
            glScalef(0.25f,0.25f,0.25f);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            glRotatef(2*alpha, 0,0,1);
            glBindTexture(GL_TEXTURE_2D, texMoon);  
            drawSpherePolygon(5, 0.8f);
        glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

void init(void)
{
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    texBg = loadTexture2D("stars.jpg"); 
    texEarth = loadTexture2D("earth_hires.jpg");
    texMoon = loadTexture2D("moon.jpg");
    texCloud = loadTexture2D("clouds.png");
    currentTex = texEarth; 
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
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(displayPolygon);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc([]{alpha += 0.6f; 
                    // delta += 0.2; 
                    // epsilon += 0.2; 
                    glutPostRedisplay(); });
	glutMainLoop();

	return 0;
}
