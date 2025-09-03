// peter_primitives.cpp
// Draws a stylized Peter Griffin using only OpenGL primitives (legacy/GLUT)
// Build (macOS): clang++ peter_primitives.cpp -framework OpenGL -framework GLUT -o peter

#define GL_SILENCE_DEPRECATION
#include <cmath>
#ifdef __APPLE__
 #include <GLUT/glut.h>
#else
 #include <GL/glut.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---- tiny helpers -----------------------------------------------------------
static void circle(float cx, float cy, float r, int seg=64) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i=0;i<=seg;i++){
        float a = (float)i * 2.0f*(float)M_PI / seg;
        glVertex2f(cx + r*std::cos(a), cy + r*std::sin(a));
    }
    glEnd();
}

static void ellipse(float cx, float cy, float rx, float ry, int seg=64) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i=0;i<=seg;i++){
        float a = (float)i * 2.0f*(float)M_PI / seg;
        glVertex2f(cx + rx*std::cos(a), cy + ry*std::sin(a));
    }
    glEnd();
}

static void outlineCircle(float cx, float cy, float r, int seg=64) {
    glBegin(GL_LINE_LOOP);
    for (int i=0;i<seg;i++){
        float a = (float)i * 2.0f*(float)M_PI / seg;
        glVertex2f(cx + r*std::cos(a), cy + r*std::sin(a));
    }
    glEnd();
}

static void rect(float x0, float y0, float x1, float y1) {
    glBegin(GL_QUADS);
    glVertex2f(x0,y0); glVertex2f(x1,y0); glVertex2f(x1,y1); glVertex2f(x0,y1);
    glEnd();
}

static void tri(float x0,float y0,float x1,float y1,float x2,float y2){
    glBegin(GL_TRIANGLES);
    glVertex2f(x0,y0); glVertex2f(x1,y1); glVertex2f(x2,y2);
    glEnd();
}

static void arc(float cx,float cy,float r,float a0_deg,float a1_deg,int seg=36){
    float a0=a0_deg*(float)M_PI/180.f, a1=a1_deg*(float)M_PI/180.f;
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<=seg;i++){
        float t = (float)i/seg;
        float a = a0 + (a1-a0)*t;
        glVertex2f(cx + r*std::cos(a), cy + r*std::sin(a));
    }
    glEnd();
}

// ---- colors -----------------------------------------------------------------
struct RGB{ float r,g,b; };
static const RGB SKIN  = {1.00f, 0.86f, 0.78f};
static const RGB SHIRT = {1.00f, 1.00f, 1.00f};
static const RGB PANTS = {0.07f, 0.45f, 0.12f}; // green
static const RGB SHOE  = {0.35f, 0.20f, 0.05f}; // brown
static const RGB BELT  = {0.10f, 0.10f, 0.10f}; // black
static const RGB BUCKL = {0.95f, 0.80f, 0.15f}; // gold
static const RGB HAIR  = {0.30f, 0.18f, 0.08f};
static const RGB LINEC = {0.00f, 0.00f, 0.00f};

// ---- the guy ----------------------------------------------------------------
static void drawPeter(float x, float y, float s=1.0f) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(s,s,1);

    // Body (white shirt)
    glColor3f(SHIRT.r, SHIRT.g, SHIRT.b);
    rect(-0.55f, -0.05f, 0.55f, -0.55f);

    // Collar (two white triangles)
    tri(-0.15f, 0.05f,  0.00f, -0.05f, -0.35f, -0.05f);
    tri( 0.15f, 0.05f,  0.35f, -0.05f,  0.00f, -0.05f);

    // Belt + buckle
    glColor3f(BELT.r, BELT.g, BELT.b); rect(-0.55f, -0.25f, 0.55f, -0.32f);
    glColor3f(BUCKL.r,BUCKL.g,BUCKL.b); rect(-0.07f, -0.25f, 0.07f, -0.32f);

    // Pants
    glColor3f(PANTS.r, PANTS.g, PANTS.b);
    rect(-0.55f, -0.32f, 0.55f, -0.85f);

    // Leg separation line
    glColor3f(LINEC.r, LINEC.g, LINEC.b);
    glLineWidth(2.0f);
    glBegin(GL_LINES); glVertex2f(0.0f,-0.32f); glVertex2f(0.0f,-0.85f); glEnd();

    // Shoes
    glColor3f(SHOE.r, SHOE.g, SHOE.b);
    ellipse(-0.22f, -0.92f, 0.22f, 0.09f);
    ellipse( 0.22f, -0.92f, 0.22f, 0.09f);

    // Sleeves
    glColor3f(SHIRT.r, SHIRT.g, SHIRT.b);
    tri(-0.55f,-0.05f, -0.75f,-0.10f, -0.55f,-0.25f);
    tri( 0.55f,-0.05f,  0.75f,-0.10f,  0.55f,-0.25f);

    // Arms (simple skin ellipses)
    glColor3f(SKIN.r, SKIN.g, SKIN.b);
    ellipse(-0.80f, -0.15f, 0.09f, 0.06f);
    ellipse( 0.80f, -0.15f, 0.09f, 0.06f);

    // Head (big ellipse) + chin (circle)
    glColor3f(SKIN.r, SKIN.g, SKIN.b);
    ellipse(0.0f, 0.35f, 0.36f, 0.42f);     // head
    circle(0.00f, 0.10f, 0.15f);            // chin

    // Hair cap (simple brown arc fill)
    glColor3f(HAIR.r, HAIR.g, HAIR.b);
    ellipse(0.0f, 0.66f, 0.30f, 0.08f);

    // Eyes
    glColor3f(1,1,1);
    circle(-0.10f, 0.50f, 0.07f);           // left eye white
    circle( 0.10f, 0.50f, 0.07f);           // right eye white
    glColor3f(0,0,0);
    circle(-0.10f, 0.50f, 0.025f);          // pupils
    circle( 0.10f, 0.50f, 0.025f);

    // Glasses frames (lines)
    glColor3f(0,0,0); glLineWidth(2.5f);
    outlineCircle(-0.10f, 0.50f, 0.085f, 48);
    outlineCircle( 0.10f, 0.50f, 0.085f, 48);
    glBegin(GL_LINES);
        glVertex2f(-0.015f, 0.50f); glVertex2f( 0.015f, 0.50f); // bridge
        glVertex2f(-0.185f, 0.51f); glVertex2f(-0.35f, 0.55f);  // left arm
        glVertex2f( 0.185f, 0.51f); glVertex2f( 0.35f, 0.55f);  // right arm
    glEnd();

    // Nose (ellipse)
    glColor3f(SKIN.r*0.98f, SKIN.g*0.98f, SKIN.b*0.98f);
    ellipse(0.00f, 0.43f, 0.09f, 0.05f);

    // Mouth (smile arc)
    glColor3f(0,0,0); glLineWidth(3.0f);
    arc(0.0f, 0.22f, 0.13f, 210.f, 330.f, 28);

    // Chin crease
    glLineWidth(2.0f);
    arc(0.0f, 0.10f, 0.10f, 200.f, 340.f, 20);

    glPopMatrix();
}

// ---- GLUT boilerplate -------------------------------------------------------
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw centered, scale to taste
    drawPeter(0.0f, 0.05f, 1.0f);

    glutSwapBuffers();
}

static void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // keep a simple 2D orthographic world [-1,1] x [-1,1]
    if (w >= h) {
        float a = (float)w/(float)h;
        glOrtho(-1.f*a, 1.f*a, -1.f, 1.f, -1.f, 1.f);
    } else {
        float a = (float)h/(float)w;
        glOrtho(-1.f, 1.f, -1.f*a, 1.f*a, -1.f, 1.f);
    }
}

static void init(){
    glClearColor(0.95f,0.95f,0.95f,1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.0f);
}

int main(int argc,char**argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow("Peter (primitives only)");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
