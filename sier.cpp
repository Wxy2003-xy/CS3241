#include <cmath>
#include <iostream>
#include <time.h>
#include <iostream>
#include <thread> // Required for std::this_thread::sleep_for
#include <chrono> // Required for std::chrono::milliseconds
#include <bits/stdc++.h>
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// --- Sierpiński Tetrahedron (recursive) ---
struct Vec3 { float x,y,z; };
static inline Vec3 mid(const Vec3& a, const Vec3& b){ return {(a.x+b.x)*0.5f,(a.y+b.y)*0.5f,(a.z+b.z)*0.5f}; }
static inline Vec3 sub(const Vec3& a, const Vec3& b){ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
static inline Vec3 cross(const Vec3& a, const Vec3& b){ return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x}; }
float alpha = 0.0, k=1;
float delta = 0.0, epsilon = 0.0;
static void drawFace(const Vec3& a, const Vec3& b, const Vec3& c){
    Vec3 n = cross(sub(b,a), sub(c,a)); // flat normal
    glNormal3f(n.x, n.y, n.z);
    glBegin(GL_TRIANGLES);
        {
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;
        glColor3f(r,g,b);  glVertex3f(a.x,a.y,a.z);
        }
                {
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b1 = static_cast<float>(rand()) / RAND_MAX;
        glColor3f(r,g,b1);  glVertex3f(b.x,b.y,b.z);
        }
                {
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;
        glColor3f(r,g,b);  glVertex3f(c.x,c.y,c.z);
        }
       
        
        
    glEnd();
}

static void drawTetraSolid(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D){
    srand(static_cast<unsigned>(time(nullptr)));
    // CCW winding as seen from outside (choose consistently)
    // glColor3f(0.4f,0.9f,0.4f); 
    drawFace(A,C,D);
    // glColor3f(0.4f,0.4f,0.9f); 
    drawFace(A,D,B);
    // glColor3f(0.9f,0.9f,0.4f); 
    drawFace(B,D,C);
}

void sierpinskiTetra(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D, int depth){
    if (depth == 0){ drawTetraSolid(A,B,C,D); return; }
    Vec3 AB=mid(A,B), AC=mid(A,C), AD=mid(A,D), BC=mid(B,C), BD=mid(B,D), CD=mid(C,D);
    // Recurse into 4 corner tetrahedra (the central void is removed)
    sierpinskiTetra(A,  AB, AC, AD, depth-1);
    sierpinskiTetra(AB, B,  BC, BD, depth-1);
    sierpinskiTetra(AC, BC, C,  CD, depth-1);
    sierpinskiTetra(AD, BD, CD, D,  depth-1);
}

void display() {
    // In init():


    // In display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // camera
    gluLookAt(2.2,1.8,2.6,  0,0,0,  0,1,0);

    // a unit-ish tetrahedron
    Vec3 A{ 0, 1, 0};
    Vec3 B{-0.9428f,-0.3333f, 0};
    Vec3 C{ 0.4714f,-0.3333f, 0.8165f};
    Vec3 D{ 0.4714f,-0.3333f,-0.8165f};

    glScalef(0.9f,0.9f,0.9f);
    glPushMatrix();
		glRotatef(alpha, 0,1,0);
		glRotatef(delta, 1,0,0);
		glRotatef(epsilon, 0,0,1);
    sierpinskiTetra(A,B,C,D, /*depth=*/4);
    glPopMatrix();
    glutSwapBuffers();

}
void reshape(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (h == 0) ? 1.0f : (float)w / (float)h;
    GLfloat P[16] = {
        3,0,0,0,
        0,3,0,0,
        0,0,-(100.0f+0.1f)/(100.0f-0.1f),-1,
        0,0,(-2*100.0f*0.1f)/(100.0f-0.1f),0
    };
    glMultMatrixf(P);
}
void init() {
    // glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    GLfloat light_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat light_col[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_col);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_col);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // glShadeModel(GL_SMOOTH);  // smooth shading shows gradients nicely
}

int main(int argc,char**argv){
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("sier");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutIdleFunc([]{ alpha += 0.0f; delta += 0.0f; epsilon += 0.2f; glutPostRedisplay(); });
    glutMainLoop();
    return 0;
}