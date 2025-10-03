// CS3241 Assignment 3: Let there be light
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
// To silence deprecation warnings in XCode
// Uncomment this line as necessary
// #define GL_SILENCE_DEPRECATION

#ifdef _WIN32
#include <Windows.h>
#include "GL/glut.h"
#define M_PI 3.141592654
#elif __APPLE__
// #include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

using namespace std;

// global variable

bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;
static inline void setMaterial(bool highlight) {
    GLfloat matAmbient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat matDiffuse[]  = {0.7f, 0.7f, 0.75f, 1.0f};
    GLfloat matSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat shininess     = 10.0f;

    if (highlight) {
        matSpecular[0] = matSpecular[1] = matSpecular[2] = 1.0f;
        shininess = 100.0f;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
void setupLighting()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Lights, material properties
    GLfloat    ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat    diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat    specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { -100.0f,100.0f,100.0f,1.0f };

    glClearDepth(1.0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

    // Default : lighting
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

}

void drawAxes(float L = 1.0f) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(L,0,0);
        glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,L,0);
        glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,L);
    glEnd();
    glLineWidth(1.0f);
    void *font = GLUT_BITMAP_HELVETICA_18;
    glColor3f(0,0,0);
    glRasterPos3f(L+0.05f, 0, 0); glutBitmapCharacter(font, 'X');
    glRasterPos3f(0, L+0.05f, 0); glutBitmapCharacter(font, 'Y');
    glRasterPos3f(0, 0, L+0.05f); glutBitmapCharacter(font, 'Z');
}
void drawCylinder(double r, double h) {
    const int n = 64;
    const double half = h * 0.5;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= n; ++i) {
        double t  = (double)i / n;
        double th = t * 2.0 * M_PI;
        double x = r * cos(th);
        double y = r * sin(th);
        glNormal3d(cos(th), sin(th), 0.0);
        glVertex3d(x, y,  half);
        glNormal3d(cos(th), sin(th), 0.0);
        glVertex3d(x, y, -half);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,1);
    glVertex3d(0,0, half);
    for (int i = 0; i <= n; ++i) {
        double th = (double)i / n * 2.0 * M_PI;
        glVertex3d(r*cos(th), r*sin(th), half);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,-1);
    glVertex3d(0,0,-half);
    for (int i = 0; i <= n; ++i) {
        double th = (double)(n - i) / n * 2.0 * M_PI;
        glVertex3d(r*cos(th), r*sin(th), -half);
    }
    glEnd();
}
void drawSphere(double r) {
    const int stacks = 10;
    const int slices = 10;

    for (int i = 0; i < stacks; ++i) {
        double theta1 = (double)i     * M_PI / stacks;
        double theta2 = (double)(i+1) * M_PI / stacks;

        for (int j = 0; j < slices; ++j) {
            double phi1 = (double)j     * 2.0 * M_PI / slices;
            double phi2 = (double)(j+1) * 2.0 * M_PI / slices;
            double x1 = r * sin(theta1) * cos(phi1);
            double y1 = r * sin(theta1) * sin(phi1);
            double z1 = r * cos(theta1);

            double x2 = r * sin(theta2) * cos(phi1);
            double y2 = r * sin(theta2) * sin(phi1);
            double z2 = r * cos(theta2);

            double x3 = r * sin(theta2) * cos(phi2);
            double y3 = r * sin(theta2) * sin(phi2);
            double z3 = r * cos(theta2);

            double x4 = r * sin(theta1) * cos(phi2);
            double y4 = r * sin(theta1) * sin(phi2);
            double z4 = r * cos(theta1);

            glBegin(GL_QUADS);
                glNormal3d(x1/r, y1/r, z1/r); glVertex3d(x1,y1,z1);
                glNormal3d(x2/r, y2/r, z2/r); glVertex3d(x2,y2,z2);
                glNormal3d(x3/r, y3/r, z3/r); glVertex3d(x3,y3,z3);
                glNormal3d(x4/r, y4/r, z4/r); glVertex3d(x4,y4,z4);
            glEnd();
        }
    }
}

void drawCone(double r, double h) {
    const int n = 20;
    const double hz = h;
    const double bz = 0.0;
    const double len = sqrt(h*h + r*r);
    const double nxz = h / len;
    const double nz  = r / len;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= n; ++i) {
        double t  = (double)i / n;
        double th = t * 2.0 * M_PI;
        double cx = cos(th), cy = sin(th);
        glNormal3d(cx*nxz, cy*nxz, nz);
        glVertex3d(r*cx, r*cy, bz);
        glNormal3d(cx*nxz, cy*nxz, nz);
        glVertex3d(0.0, 0.0, hz);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glNormal3d(0,0,-1);
    glVertex3d(0,0,bz);
    for (int i = 0; i <= n; ++i) {
        double th = (double)(n - i) / n * 2.0 * M_PI;
        glVertex3d(r*std::cos(th), r*std::sin(th), bz);
    }
    glEnd();
}

void drawUnitCube() {
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glVertex3f(-1,-1, 1);
        glVertex3f( 1,-1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f(-1, 1, 1);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1, 1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1,-1,-1);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1, 1,-1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1,-1, 1);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(-1,0,0);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1,-1, 1);
        glVertex3f(-1, 1, 1);
        glVertex3f(-1, 1,-1);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(-1, 1,-1);
        glVertex3f(-1, 1, 1);
        glVertex3f( 1, 1, 1);
        glVertex3f( 1, 1,-1);
    glEnd();
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f(-1,-1,-1);
        glVertex3f( 1,-1,-1);
        glVertex3f( 1,-1, 1);
        glVertex3f(-1,-1, 1);
    glEnd();
}

void drawCube(double w, double h) {
    glPushMatrix();
    glScalef(w*0.5f, w*0.5f, h*0.5f);
    drawUnitCube();
    glPopMatrix();
}
void drawRegCon(double r, double h) {
    glPushMatrix();
        glPushMatrix();
            glTranslated(0, 0, h*0.425); 
            glRotated(90, 0, 1, 0);
            glScaled(1, 1, 1);
            drawCube(0.5, 2.0);
        glPopMatrix();
        drawCone(r, h);
    glPopMatrix();
}


struct FCCVec { double x,y,z; };
static inline FCCVec fccAdd(FCCVec a, FCCVec b){
    return {a.x+b.x,a.y+b.y,a.z+b.z}; 
}
static inline FCCVec fccSub(FCCVec a, FCCVec b){
    return {a.x-b.x,a.y-b.y,a.z-b.z}; 
}
static inline FCCVec fccMul(FCCVec a, double s){
    return {a.x*s,a.y*s,a.z*s}; 
}
static inline double fccDot(FCCVec a, FCCVec b){
    return a.x*b.x+a.y*b.y+a.z*b.z; 
}
static inline FCCVec fccCross(FCCVec a, FCCVec b){
    return {a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x};
}
static inline double fccLen(FCCVec a){
    return std::sqrt(fccDot(a,a)); 
}
static inline FCCVec fccNorm(FCCVec a){ 
    double L=fccLen(a); 
    return (L>0)? fccMul(a,1.0/L) : FCCVec{0,0,0}; 
}

static void drawCylinderZ(double r, double h, int slices=10) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i) {
        double th = (double)i / slices * 2.0 * M_PI;
        double cx = cos(th), cy = sin(th);
        glNormal3d(cx, cy, 0.0);
        glVertex3d(r*cx, r*cy, 0.0);
        glVertex3d(r*cx, r*cy, h);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,0,-1); glVertex3d(0,0,0);
        for (int i = 0; i <= slices; ++i) {
            double th = (double)(slices - i) / slices * 2.0 * M_PI;
            glVertex3d(r*cos(th), r*sin(th), 0.0);
        }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,0,1); glVertex3d(0,0,h);
        for (int i = 0; i <= slices; ++i) {
            double th = (double)i / slices * 2.0 * M_PI;
            glVertex3d(r*cos(th), r*sin(th), h);
        }
    glEnd();
}

static void drawBond(FCCVec p, FCCVec q, double r) {
    FCCVec d = fccSub(q, p);
    double L = fccLen(d);
    if (L < 1e-9) return;
    FCCVec u = fccNorm(d);
    FCCVec z = {0,0,1};
    FCCVec axis = fccCross(z, u);
    double c = clamp(fccDot(z,u), -1.0, 1.0);
    double ang = acos(c) * 180.0 / M_PI;

    glPushMatrix();
        glTranslated(p.x, p.y, p.z);
        if (fccLen(axis) < 1e-9) {
            if (c < 0) glRotated(180.0, 1,0,0);
        } else {
            glRotated(ang, axis.x, axis.y, axis.z);
        }
        drawCylinderZ(r, L);
    glPopMatrix();
}

static void drawAtom(FCCVec p, double R) {
    glPushMatrix();
        glTranslated(p.x, p.y, p.z);
        drawSphere(R);
    glPopMatrix();
}
void drawFCCUnitCell(double a=1.0, double atomR=0.12, double bondR=0.04, bool drawFaceBonds=false) {
    FCCVec C[8] = {
        {0,0,0},{a,0,0},{0,a,0},{a,a,0},
        {0,0,a},{a,0,a},{0,a,a},{a,a,a}
    };
    FCCVec F[6] = {
        {a*0.5, a*0.5, 0   }, {a*0.5, a*0.5, a   },
        {a*0.5, 0,    a*0.5}, {a*0.5, a,    a*0.5},
        {0,    a*0.5, a*0.5}, {a,    a*0.5, a*0.5}
    };

    int E[12][2] = {
        {0,1},{1,3},{2,3},{0,2},
        {4,5},{5,7},{6,7},{4,6},
        {0,4},{1,5},{2,6},{3,7}
    };

    glColor3f(0.2f, 0.2f, 0.2f);
    for (auto &e : E) drawBond(C[e[0]], C[e[1]], bondR);
    if (drawFaceBonds) {
        int face2corners[6][4] = {
            {0,1,3,2},
            {4,5,7,6},
            {0,1,5,4},
            {2,3,7,6},
            {0,2,6,4},
            {1,3,7,5}
        };
        for (int fi=0; fi<6; ++fi) {
            for (int k=0;k<4;++k) drawBond(F[fi], C[face2corners[fi][k]], bondR*0.9);
        }
    }
    glColor3f(0.05f, 0.2f, 0.7f);
    for (auto &p : C) drawAtom(p, atomR);
    glColor3f(0.85f, 0.85f, 0.85f);
    for (auto &p : F) drawAtom(p, atomR);
}
static inline FCCVec fccMake(double x, double y, double z) { FCCVec v{ x, y, z }; return v; }

void drawFCCMesh(int nx, int ny, int nz,
                 double a = 1.0, double atomR = 0.12, double bondR = 0.04,
                 bool drawFaceBonds = true)
{
    if (nx <= 0 || ny <= 0 || nz <= 0) return;
    glColor3f(0.2f, 0.2f, 0.2f);
    for (int j = 0; j <= ny; ++j)
    for (int k = 0; k <= nz; ++k)
    for (int i = 0; i < nx; ++i) {
        FCCVec p = fccMake(i*a,     j*a, k*a);
        FCCVec q = fccMake((i+1)*a, j*a, k*a);
        drawBond(p, q, bondR);
    }
    for (int i = 0; i <= nx; ++i)
    for (int k = 0; k <= nz; ++k)
    for (int j = 0; j < ny; ++j) {
        FCCVec p = fccMake(i*a, j*a,     k*a);
        FCCVec q = fccMake(i*a, (j+1)*a, k*a);
        drawBond(p, q, bondR);
    }
    for (int i = 0; i <= nx; ++i)
    for (int j = 0; j <= ny; ++j)
    for (int k = 0; k < nz; ++k) {
        FCCVec p = fccMake(i*a, j*a, k*a);
        FCCVec q = fccMake(i*a, j*a, (k+1)*a);
        drawBond(p, q, bondR);
    }
    if (drawFaceBonds) {
        for (int k = 0; k <= nz; ++k)
        for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j) {
            FCCVec fc = fccMake((i+0.5)*a, (j+0.5)*a, k*a);
            FCCVec c00 = fccMake(i*a,     j*a,     k*a);
            FCCVec c10 = fccMake((i+1)*a, j*a,     k*a);
            FCCVec c11 = fccMake((i+1)*a, (j+1)*a, k*a);
            FCCVec c01 = fccMake(i*a,     (j+1)*a, k*a);
            drawBond(fc, c00, bondR*0.9);
            drawBond(fc, c10, bondR*0.9);
            drawBond(fc, c11, bondR*0.9);
            drawBond(fc, c01, bondR*0.9);
        }
        for (int j = 0; j <= ny; ++j)
        for (int i = 0; i < nx; ++i)
        for (int k = 0; k < nz; ++k) {
            FCCVec fc = fccMake((i+0.5)*a, j*a, (k+0.5)*a);
            FCCVec c00 = fccMake(i*a,     j*a, k*a);
            FCCVec c10 = fccMake((i+1)*a, j*a, k*a);
            FCCVec c11 = fccMake((i+1)*a, j*a, (k+1)*a);
            FCCVec c01 = fccMake(i*a,     j*a, (k+1)*a);
            drawBond(fc, c00, bondR*0.9);
            drawBond(fc, c10, bondR*0.9);
            drawBond(fc, c11, bondR*0.9);
            drawBond(fc, c01, bondR*0.9);
        }
        for (int i = 0; i <= nx; ++i)
        for (int j = 0; j < ny; ++j)
        for (int k = 0; k < nz; ++k) {
            FCCVec fc = fccMake(i*a, (j+0.5)*a, (k+0.5)*a);
            FCCVec c00 = fccMake(i*a, j*a,     k*a);
            FCCVec c10 = fccMake(i*a, (j+1)*a, k*a);
            FCCVec c11 = fccMake(i*a, (j+1)*a, (k+1)*a);
            FCCVec c01 = fccMake(i*a, j*a,     (k+1)*a);
            drawBond(fc, c00, bondR*0.9);
            drawBond(fc, c10, bondR*0.9);
            drawBond(fc, c11, bondR*0.9);
            drawBond(fc, c01, bondR*0.9);
        }
    }
    glColor3f(0.05f, 0.2f, 0.7f);
    for (int i = 0; i <= nx; ++i)
    for (int j = 0; j <= ny; ++j)
    for (int k = 0; k <= nz; ++k) {
        drawAtom(fccMake(i*a, j*a, k*a), atomR);
    }
    glColor3f(0.85f, 0.85f, 0.85f);
    for (int k = 0; k <= nz; ++k)
    for (int i = 0; i < nx; ++i)
    for (int j = 0; j < ny; ++j)
        drawAtom(fccMake((i+0.5)*a, (j+0.5)*a, k*a), atomR);
    for (int j = 0; j <= ny; ++j)
    for (int i = 0; i < nx; ++i)
    for (int k = 0; k < nz; ++k)
        drawAtom(fccMake((i+0.5)*a, j*a, (k+0.5)*a), atomR);
    for (int i = 0; i <= nx; ++i)
    for (int j = 0; j < ny; ++j)
    for (int k = 0; k < nz; ++k)
        drawAtom(fccMake(i*a, (j+0.5)*a, (k+0.5)*a), atomR);
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(0, 0, -6.0f);

        drawAxes(1.25f);

        glRotatef(angle2, 1.0f, 0.0f, 0.0f);
        glRotatef(angle,  0.0f, 1.0f, 0.0f);
        glScalef(zoom, zoom, zoom);

        glShadeModel(m_Smooth ? GL_SMOOTH : GL_FLAT);
        setMaterial(m_Highlight);
        switch (current_object) {
        case 0:
            drawSphere(1);
            break;
        case 1:
            drawCone(1, 2);
            break;
        case 2:
            drawRegCon(1,2);
            break;
        case 3:
            glPushMatrix();
                glTranslatef(-2.0f, -2.0f, -2.0f);
                drawFCCMesh(2,2,2,2.0,0.12,0.04,true);
            glPopMatrix();
            break;
        default:
            break;
        };
    glPopMatrix();
    glutSwapBuffers();
}




void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'p':
    case 'P':
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 'w':
    case 'W':
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 'v':
    case 'V':
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case 's':
    case 'S':
        m_Smooth = !m_Smooth;
        break;
    case 'h':
    case 'H':
        m_Highlight = !m_Highlight;
        break;

    case '1':
    case '2':
    case '3':
    case '4':
        current_object = key - '1';
        break;

    case 'Q':
    case 'q':
        exit(0);
        break;

    default:
        break;
    }

    glutPostRedisplay();
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

int main(int argc, char** argv)
{
    cout << "CS3241 Lab 3" << endl << endl;

    cout << "1-4: Draw different objects" << endl;
    cout << "S: Toggle Smooth Shading" << endl;
    cout << "H: Toggle Highlight" << endl;
    cout << "W: Draw Wireframe" << endl;
    cout << "P: Draw Polygon" << endl;
    cout << "V: Draw Vertices" << endl;
    cout << "Q: Quit" << endl << endl;

    cout << "Left mouse click and drag: rotate the object" << endl;
    cout << "Right mouse click and drag: zooming" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS3241 Assignment 3");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    setupLighting();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
        /* aspect ratio */ 1.0,
        /* Z near */ 1.0, /* Z far */ 80.0);
    glMatrixMode(GL_MODELVIEW);
    glutMainLoop();

    return 0;
}
