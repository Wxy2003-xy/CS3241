// CS3241Lab1.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <time.h>
#include <iostream>
#include <thread> // Required for std::this_thread::sleep_for
#include <chrono> // Required for std::chrono::milliseconds
#include <bits/stdc++.h>
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

#define numStars 100
#define numPlanets 8



class planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;
	float alpha;
	planet()
	{
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
		alpha = 1.0;
	}
    planet(float d, float spd, float r, float g, float b, float s, float ang, float a=1.0f) {
        distFromRef = d;
        angularSpeed = spd;
        color[0] = r; color[1] = g; color[2] = b;
        size = s;
        angle = ang;
        alpha = a;
    }
    void setAlpha(float a) {
        this->alpha = a;
    }
    void setAngle(float angle) {
        this->angle = angle;
    }
};

class PlanetListNode {
    bool is_head;
    planet p;
    vector<PlanetListNode*> satellites;
    public:
    PlanetListNode(planet pl, bool head=true):is_head(head), p(pl) {}
    void insert(planet pl) {
        satellites.push_back(new PlanetListNode(pl, false));
    }  
    bool is_planet() {
        return this->is_head;
    }
    planet& get() { return p; }
    const planet& get() const { return p; }

    vector<PlanetListNode*>& getSatellites() { 
        return satellites; 
    }
    const vector<PlanetListNode*>& getSatellites() const { 
        return satellites; 
    }
    int getNumSatellites() {
        return satellites.size();
    }

};

GLfloat PI = 3.14;
float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
vector<PlanetListNode> planetList;
bool clockMode = false;
time_t seconds = 0;
time_t mins = 0;
time_t hrs = 0;

struct tm * timeinfo;
float timer = 0.1; // this is a dumb way to control the speed of rotations


void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (0.0, 0.0, 0.3, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void generatePlanets()
{
    srand(static_cast<unsigned>(time(nullptr)));
    planet pl(0, 0, 1,0,0,2,0);
    planetList.emplace_back(pl, true);
    for (int i = 1; i < numPlanets; i++) {
        // Random color
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;

        planet pl(3.2f+(i),
                  2.5f*(i+1),
                  r, g, b,
                  (10.0f-i)/10.0f,
                  360 * static_cast<float>(rand()) / RAND_MAX);                     // angle

        planetList.emplace_back(pl, true);

        int numSatellites = rand() % 6;
        for (int j = 0; j < numSatellites; j++) {
            float sr = static_cast<float>(rand()) / RAND_MAX;
            float sg = static_cast<float>(rand()) / RAND_MAX;
            float sb = static_cast<float>(rand()) / RAND_MAX;

            planet sat(j*0.1f+1.2f,
                       15.0f*(j+1),
                       sr, sg, sb,
                       max(0.1f, 0.3f - j*0.2f),
                       360 * static_cast<float>(rand()) / RAND_MAX, 0.1f);                // angle

            planetList[i].insert(sat);
        }
    }
}


void drawPlanet(planet p, int* circle_points) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < *circle_points; i++) {
        float angle = 2*PI*i/ *circle_points;
        if(i>*circle_points/2)
            glColor4f(p.color[0], p.color[1], p.color[2], p.alpha);
        else
            glColor4f(p.color[0]-0.2, p.color[1]-0.2, p.color[2]-0.2, p.alpha);
        glVertex2f(p.size * cos(angle), p.size *sin(angle));
    }
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

	int circle_points = 50;
	float angle = 0;
    drawPlanet(planetList[0].get(), &circle_points);

    for (int i = 1; i < planetList.size(); i++) {
        glPushMatrix();
            glRotatef(planetList[i].get().angle, 0, 0, 1);
            glTranslatef(0, planetList[i].get().distFromRef, 0);	
            drawPlanet(planetList[i].get(), &circle_points);
            for (int j = 0; j < planetList[i].getNumSatellites(); j++) {
                glPushMatrix();
                    auto& sats = planetList[i].getSatellites();
                    glRotatef(sats[j]->get().angle, 0, 0, 1);
                    // cout<< planetList[3].getSatellites()[2]->get().angle <<endl;
                    glTranslatef(0, sats[j]->get().distFromRef, 0);
                    drawPlanet(sats[j]->get(), &circle_points);
                glPopMatrix();     
            }
        glPopMatrix();
    }   
	glPopMatrix();
	glFlush ();
}

void idle() {
    if (!clockMode) {
        for (int i = 0; i < numPlanets; ++i) {
            auto& pl = planetList[i].get();
            pl.setAlpha(1.0f);
            pl.setAngle(pl.angle + pl.angularSpeed * timer);
            for (auto* satNode : planetList[i].getSatellites()) {
                auto& sat = satNode->get();
                sat.setAlpha(1.0f);
                sat.setAngle(sat.angle + sat.angularSpeed * timer);
            }
        }
    } else {
        for (int i = 0; i < numPlanets; ++i) {
            auto& pl = planetList[i].get();
            pl.setAlpha(1.0f);
            if (i < 3) {
                seconds = time(NULL);
                timeinfo = localtime(&seconds);
                float secAngle = timeinfo->tm_sec * -6.0f;
                pl.setAngle(secAngle);
                for (auto* satNode : planetList[i].getSatellites()) {
                    auto& sat = satNode->get();
                    sat.setAlpha(1.0f);
                    sat.setAngle(sat.angle + sat.angularSpeed * timer);
                }
            } else if (i < 5) {
                mins = time(NULL);
                timeinfo = localtime(&mins);
                float minAngle = timeinfo->tm_min * -6.0f;
                pl.setAngle(minAngle);
                for (auto* satNode : planetList[i].getSatellites()) {
                    auto& sat = satNode->get();
                    sat.setAlpha(1.0f);
                    sat.setAngle(sat.angle + sat.angularSpeed * timer);
                }
            } else {
                hrs = time(NULL);
                timeinfo = localtime(&hrs);
                float hrAngle = timeinfo->tm_hour * -15.0f;
                pl.setAngle(hrAngle);
                for (auto* satNode : planetList[i].getSatellites()) {
                    auto& sat = satNode->get();
                    sat.setAlpha(1.0f);
                    sat.setAngle(sat.angle + sat.angularSpeed * timer);
                }
            }
            // for (auto* satNode : planetList[i].getSatellites()) {
            //     satNode->get().setAlpha(1.0f);
            //     satNode->get().setAngle(secAngle);
            // }
        }
    }

    glutPostRedisplay();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

        case 27: // press ESC to exit
		case 'q':
		case 'Q':
            exit(0);
	
		case 't':
			clockMode = !clockMode;
			if (clockMode)
				cout << "Current Mode: Clock mode." << endl;
			else
				cout << "Current Mode: Solar mode." << endl;
			break;

		default:
			break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Toggle Time Mode: T\n";
    cout<<"Exit: ESC or q/Q\n";
	cout << "Current Mode: Clock mode." << endl;


	generatePlanets();

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
