//ID   : 22-47958-2
//NAME : Raihanul Haque Rifan

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define PI 3.14159265359
#define MAX_DROPS 500
#define MAX_BUBBLES 500

#define PLANE_RED_R 217
#define PLANE_RED_G 31
#define PLANE_RED_B 42

#define PLANE_WHITE_R 245
#define PLANE_WHITE_G 245
#define PLANE_WHITE_B 245

#define PLANE_GREY_R 115
#define PLANE_GREY_G 115
#define PLANE_GREY_B 115

#define PLANE_WINDOW_R 68
#define PLANE_WINDOW_G 104
#define PLANE_WINDOW_B 116

//ocean
bool rifanoceanWaveActive = false;
float rifanwaveOffsetOcean = 0.0f;

// sky
float rifansunX = -830, rifansunY = 900;
bool rifansunset = false;
bool rifanmoonRise = false;
float rifanbgR = 194, rifanbgG = 237, rifanbgB = 245;  // Initial day sky color
float rifanmoonY = 700;
int rifanstarBlink = 0;
bool rifanshowStars = false;

//Plane
bool rifanmovePlane = false;
float rifanplaneX = 100; // Start off-screen to the right
float rifanplaneY = 800;  // Vertical position
float rifanplaneScale = 0.5;

//ship
bool rifanmoveShip = false;
int rifanshipDirection = 1; // Moving right
float rifanshipX = -100; // starting from right side
float rifanpollutionLevel = 0.0f;

//rain
bool rifanrainActive = false;
float rifanrainGroundY = -450.0f;

float rifandropX[MAX_DROPS];
float rifandropY[MAX_DROPS];
float rifandropSpeed[MAX_DROPS];
int rifandropCount = 0;

float rifanbubbleX[MAX_BUBBLES];
float rifanbubbleY[MAX_BUBBLES];
float rifanbubbleRadius[MAX_BUBBLES];
float rifanbubbleAlpha[MAX_BUBBLES];
int rifanbubbleActive[MAX_BUBBLES];
int rifanbubbleCount = 0;



void rifancircle(float radius, float xc, float yc, float r, float g, float b)
{
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
	for(int i=0;i<200;i++)
        {
            glColor3ub(r,g,b);
            float pi=3.1416;
            float A=(i*pi*2)/200;
            float r=radius;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+xc,y+yc);
        }

	glEnd();
}


// Draw a circle for bubbles
void rifandrawCircle(float x, float y, float radius, float alpha) {
    int segments = 32;
    glColor4f(0.5f, 0.7f, 1.0f, alpha);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(x + cosf(angle) * radius, y + sinf(angle) * radius);
    }
    glEnd();
}


void rifantree1(float x, float y, float z)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

     glTranslatef(x,y,z); /*Transalation*/


    glBegin(GL_POLYGON); //trinagle tree
    glColor3ub(167, 35, 19 );
    glVertex2f(-400,380);
    glVertex2f(-395,380);
    glVertex2f(-395,500);
    glVertex2f(-400,500);
    glEnd();


    glBegin(GL_POLYGON);
    glColor3ub(127, 196, 83 );
    glVertex2f(-420,500);
    glVertex2f(-370,500);
    glVertex2f(-395,700);
    glEnd();

	glPopMatrix();

}


void rifantree2(float x, float y, float z)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(x,y,z); /*Transalation*/

	//glRotatef(200,0.0f, 0.0f, 1.0f); /*Rotation*/

	//glScalef(0.1f, -2.0f, 0.0f); /*Scaling*/

    glBegin(GL_POLYGON); //round tree
    glColor3ub(167, 35, 19 );
    glVertex2f(-310,380);
    glVertex2f(-300,380);
    glVertex2f(-300,500);
    glVertex2f(-310,500);
    glEnd();

    rifancircle(50,-305,580,79, 184, 12 );
    rifancircle(50,-260,530,79, 184, 12 );
    rifancircle(50,-345,530,79, 184, 12 );

	glEnd();
	glPopMatrix();

}

// Function to draw stars across the whole sky
void rifandrawStars() {
    if (!rifanshowStars) return;
    glColor3ub(255, 255, 255);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (int i = 0; i < 150; ++i) {
        float x = (rand() % 2400) - 1200; // Full sky width
        float y = (rand() % 700) + 500;   // Sky height
        if ((i + rifanstarBlink) % 4 < 2) {
            glVertex2f(x, y);
        }
    }
    glEnd();
}


// Sky with sun/moon and distant greenery
void rifansky() {
    // Sky background
    glBegin(GL_POLYGON);
    glColor3ub((int)rifanbgR, (int)rifanbgG, (int)rifanbgB);
    glVertex2f(1200, 500);
    glVertex2f(-1300, 500);
    glVertex2f(-1300, 1200);
    glVertex2f(1200, 1200);
    glEnd();

    if (!rifansunset) {
        rifancircle(130, rifansunX, rifansunY, 247, 247, 40); // Sun
    }

    if (rifansunset && rifanmoonRise) {
        rifancircle(80, 900, rifanmoonY, 220, 220, 255); // Moon rising
    }

    rifandrawStars();

    // Distance greenery
    int R = 161, G = 189, B = 135;
    float y = 450;
    rifancircle(70, 630, y, R, G, B);
    rifancircle(100, 700, y, R, G, B);
    rifancircle(130, 800, y, R, G, B);
    rifancircle(160, 950, y, R, G, B);
    rifancircle(200, 1150, y, R, G, B);
    rifancircle(60, -220, y, R, G, B);
    rifancircle(70, -280, y, R, G, B);
    rifancircle(100, -430, y, R, G, B);
    rifancircle(130, -530, y, R, G, B);
    rifancircle(160, -700, y, R, G, B);
    rifancircle(200, -850, y, R, G, B);
    rifancircle(130, -1050, y, R, G, B);
    rifancircle(90, -1150, y, R, G, B);

}

void rifandrawPlane(float x, float y, float scale) {


    glPushMatrix(); // Save the current transformation matrix

    // Move and scale the plane to the desired position and size
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0);

    // To get smooth outlines
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.5);

    // --- Draw Far Wing ---
    // Fill
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_WHITE_R, PLANE_WHITE_G, PLANE_WHITE_B);
    glVertex2f(-120, 10);
    glVertex2f(40, 15);
    glVertex2f(10, -80);
    glVertex2f(-60, -80);
    glEnd();
    // Outline
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(-120, 10);
    glVertex2f(40, 15);
    glVertex2f(10, -80);
    glVertex2f(-60, -80);
    glEnd();
    // Red Tip
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(10, -80);
    glVertex2f(-60, -80);
    glVertex2f(-55, -90);
    glVertex2f(15, -90);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(10, -80);
    glVertex2f(-60, -80);
    glVertex2f(-55, -90);
    glVertex2f(15, -90);
    glEnd();

    // --- Draw Far Horizontal Stabilizer ---
    // Fill
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(-270, 15);
    glVertex2f(-220, 18);
    glVertex2f(-215, -20);
    glVertex2f(-260, -20);
    glEnd();
    // Outline
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(-270, 15);
    glVertex2f(-220, 18);
    glVertex2f(-215, -20);
    glVertex2f(-260, -20);
    glEnd();

    // --- Draw Fuselage (Main Body) ---
    // Red Bottom Half
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(300, 0);  // Nose tip
    glVertex2f(280, -40);
    glVertex2f(-250, -40);
    glVertex2f(-280, -20);
    glVertex2f(-280, 20); // Tail middle point
    glVertex2f(-250, 40);
    glVertex2f(280, 40);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(300, 0);
    glVertex2f(280, -40);
    glVertex2f(-250, -40);
    glVertex2f(-280, -20);
    glVertex2f(-280, 20);
    glVertex2f(-250, 40);
    glVertex2f(280, 40);
    glEnd();

    // White Top Half
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_WHITE_R, PLANE_WHITE_G, PLANE_WHITE_B);
    glVertex2f(280, 40);
    glVertex2f(-250, 40);
    glVertex2f(-265, 55); // Taper up to tail
    glVertex2f(180, 60);  // Top of cockpit
    glVertex2f(240, 55);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(280, 40);
    glVertex2f(-250, 40);
    glVertex2f(-265, 55);
    glVertex2f(180, 60);
    glVertex2f(240, 55);
    glEnd();

    // Separator Line
    glBegin(GL_LINES);
    glColor3ub(PLANE_GREY_R, PLANE_GREY_G, PLANE_GREY_B);
    glVertex2f(-250, 40);
    glVertex2f(280, 40);
    glEnd();

    // Passenger Windows
    for (int i = 0; i < 9; ++i) {
        float winX = 180 - (i * 35);
        glBegin(GL_QUADS);
        glColor3ub(PLANE_GREY_R, PLANE_GREY_G, PLANE_GREY_B);
        glVertex2f(winX, 45);
        glVertex2f(winX + 15, 45);
        glVertex2f(winX + 15, 55);
        glVertex2f(winX, 55);
        glEnd();
    }

    // --- Draw Near Wing ---
    // Fill
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_WHITE_R, PLANE_WHITE_G, PLANE_WHITE_B);
    glVertex2f(-100, 30);
    glVertex2f(60, 25);
    glVertex2f(90, 150);
    glVertex2f(-50, 160);
    glEnd();
    // Outline
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(-100, 30);
    glVertex2f(60, 25);
    glVertex2f(90, 150);
    glVertex2f(-50, 160);
    glEnd();
    // Red Tip
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(90, 150);
    glVertex2f(-50, 160);
    glVertex2f(-45, 170);
    glVertex2f(95, 160);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(90, 150);
    glVertex2f(-50, 160);
    glVertex2f(-45, 170);
    glVertex2f(95, 160);
    glEnd();

    // --- Draw Engine ---
    // Pod (Red)
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(50, 40);
    glVertex2f(50, 80);
    glVertex2f(-20, 80);
    glVertex2f(-20, 40);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(50, 40);
    glVertex2f(50, 80);
    glVertex2f(-20, 80);
    glVertex2f(-20, 40);
    glEnd();
    // Intake (Grey)
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_GREY_R, PLANE_GREY_G, PLANE_GREY_B);
    glVertex2f(50, 45);
    glVertex2f(50, 75);
    glVertex2f(60, 75);
    glVertex2f(60, 45);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(50, 45);
    glVertex2f(50, 75);
    glVertex2f(60, 75);
    glVertex2f(60, 45);
    glEnd();

    // --- Draw Near Horizontal Stabilizer ---
    // Fill
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(-280, 30);
    glVertex2f(-230, 28);
    glVertex2f(-225, 75);
    glVertex2f(-270, 75);
    glEnd();
    // Outline
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(-280, 30);
    glVertex2f(-230, 28);
    glVertex2f(-225, 75);
    glVertex2f(-270, 75);
    glEnd();

    // --- Draw Vertical Tail Fin ---
    // Fill
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_RED_R, PLANE_RED_G, PLANE_RED_B);
    glVertex2f(-200, 58);
    glVertex2f(-260, 55);
    glVertex2f(-280, 130);
    glVertex2f(-240, 130);
    glEnd();
    // Outline
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(-200, 58);
    glVertex2f(-260, 55);
    glVertex2f(-280, 130);
    glVertex2f(-240, 130);
    glEnd();

    // --- Draw Details ---
    // Cockpit Window
    glBegin(GL_POLYGON);
    glColor3ub(PLANE_WINDOW_R, PLANE_WINDOW_G, PLANE_WINDOW_B);
    glVertex2f(285, 20);
    glVertex2f(280, 40);
    glVertex2f(240, 55);
    glVertex2f(215, 58);
    glVertex2f(215, 40);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 0);
    glVertex2f(285, 20);
    glVertex2f(280, 40);
    glVertex2f(240, 55);
    glVertex2f(215, 58);
    glVertex2f(215, 40);
    glEnd();



    // Door
    glBegin(GL_QUADS);
    glColor3ub(PLANE_WHITE_R, PLANE_WHITE_G, PLANE_WHITE_B);
    glVertex2f(190, 40);
    glVertex2f(210, 40);
    glVertex2f(210, 60);
    glVertex2f(190, 60);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(190, 40);
    glVertex2f(210, 40);
    glVertex2f(210, 60);
    glVertex2f(190, 60);
    glEnd();


    glPopMatrix(); // Restore the original transformation matrix
}



void rifangrass(float x, float y, float z)
{

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x,y,z); //Transalation

    glBegin(GL_POLYGON);
    glColor3ub(41, 168, 13);
    glVertex2f(-1300,-450);
    glVertex2f(-1280,-420);
    glVertex2f(-1260,-450);
    glEnd();

	glPopMatrix();

	//LowGrass
    glBegin(GL_POLYGON);
    glColor3ub(41, 168, 13);
    glVertex2f(800,-450);//V1
    glVertex2f(-1300,-450);//W1
    glVertex2f(-1300,-500);//V
    glVertex2f(800,-500);//Z1
    glEnd();

}



void rifanocean()
{

    //OCEAN
    glBegin(GL_POLYGON);
   // glColor3ub(79, 202, 244 );
    glColor3ub(70 , 193 , 202 );
    glVertex2f(-1300,430);//W1
    glVertex2f(1300,430);
    glVertex2f(1200,-500);//C2
    glVertex2f(-1300,-500);//T1
    glEnd();


    //DISTANCE Ocean
    glBegin(GL_POLYGON);
    glColor3ub(135, 206, 235 );
    glVertex2f(1200,500);//S1
    glVertex2f(1200,430);
    glVertex2f(-1300,430);//U1
    glVertex2f(-1300,500);//T1
    glEnd();

    //BEACH
    glBegin(GL_POLYGON);
    glColor3ub(249, 238, 144);
    glVertex2f(-400,430);//V1
    glVertex2f(-1300,430);//W1
    glVertex2f(-1300,-500);//V
    glVertex2f(0,-500);//Z1
    glEnd();


}

void rifandrawSeaTrash()
{
    // ================= TIRE 1 =================
    // Outer black tire
    rifancircle(40, 500, 60, 73 , 68 , 68);

    // Inner hollow (ocean color)
    rifancircle(18, 500, 60, 70 , 193 , 202);


    // ================= TIRE 2 =================
    // Outer black tire
    rifancircle(35, 650, -80, 73 , 68 ,68);

    // Inner hollow (ocean color)
    rifancircle(15, 650, -80, 70 , 193 , 202);


    // ================= TIRE 3 =================
    // Outer black tire
    rifancircle(40, -320, 200, 73 , 68 , 68);

    // Inner hollow
    rifancircle(18, -320, 200, 249, 238, 144);

    // Bottle body
    glColor3ub(190, 220, 230);
    glBegin(GL_POLYGON);
    glVertex2f(-970, -350);
    glVertex2f(-920, -350);
    glVertex2f(-920, -420);
    glVertex2f(-970, -420);
    glEnd();

    glColor3ub(255,158,0);
    glBegin(GL_POLYGON);
    glVertex2f(-910, -350);
    glVertex2f(-940, -410);
    glVertex2f(-880, -410);
    glEnd();



    // ================= TIRE 4 =================
    // Outer black tire
    rifancircle(40, -980, -400, 73 , 68 , 68);

    // Inner hollow
    rifancircle(18, -980, -400, 249, 238, 144);


    // ================= REALISTIC PLASTIC BOTTLE Beach=================


    // Bottle body
    glColor3ub(190, 220, 230);
    glBegin(GL_POLYGON);
    glVertex2f(-300, 150);
    glVertex2f(-250, 150);
    glVertex2f(-250, 80);
    glVertex2f(-300, 80);
    glEnd();

    // Bottle neck
    glColor3ub(170, 200, 210);
    glBegin(GL_POLYGON);
    glVertex2f(-275, 175);
    glVertex2f(-255, 175);
    glVertex2f(-255, 150);
    glVertex2f(-275, 150);
    glEnd();

    // Bottle cap
    glColor3ub(90, 90, 90);
    glBegin(GL_POLYGON);
    glVertex2f(-280, 190);
    glVertex2f(-250, 190);
    glVertex2f(-250, 175);
    glVertex2f(-280, 175);
    glEnd();

    // ================= REALISTIC PLASTIC BOTTLE SEA =================
    // Bottle body
    glColor3ub(190, 220, 230);
    glBegin(GL_POLYGON);
    glVertex2f(280, 90);
    glVertex2f(340, 90);
    glVertex2f(350, 40);
    glVertex2f(270, 40);
    glEnd();

    // Bottle neck
    glColor3ub(170, 200, 210);
    glBegin(GL_POLYGON);
    glVertex2f(305, 115);
    glVertex2f(325, 115);
    glVertex2f(325, 90);
    glVertex2f(305, 90);
    glEnd();

    // Bottle cap
    glColor3ub(90, 90, 90);
    glBegin(GL_POLYGON);
    glVertex2f(300, 130);
    glVertex2f(330, 130);
    glVertex2f(330, 115);
    glVertex2f(300, 115);
    glEnd();

    // ================= USED CANS =================
    glColor3ub(170, 170, 170);
    glBegin(GL_POLYGON);
    glVertex2f(480, -40);
    glVertex2f(520, -40);
    glVertex2f(520, -80);
    glVertex2f(480, -80);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(560, -150);
    glVertex2f(600, -150);
    glVertex2f(600, -190);
    glVertex2f(560, -190);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-345, 115);
    glVertex2f(-305, 115);
    glVertex2f(-305, 155);
    glVertex2f(-345, 155);
    glEnd();

    // ================= PAPERS =================
    glColor3ub(245, 245, 245);
    glBegin(GL_POLYGON);
    glVertex2f(620, 10);
    glVertex2f(690, 10);
    glVertex2f(680, -25);
    glVertex2f(610, -25);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(750, -70);
    glVertex2f(820, -70);
    glVertex2f(810, -110);
    glVertex2f(740, -110);
    glEnd();

    glColor3ub(245, 245, 245);
    glBegin(GL_POLYGON);
    glVertex2f(-350, 100);
    glVertex2f(-290, 100);
    glVertex2f(-300, 80);
    glVertex2f(-370, 80);
    glEnd();

    // ================= CHIPS PACKETS =================
    glColor3ub(220, 80, 80);
    glBegin(GL_POLYGON);
    glVertex2f(840, -100);
    glVertex2f(910, -100);
    glVertex2f(900, -150);
    glVertex2f(830, -150);
    glEnd();

    glColor3ub(240, 160, 60);
    glBegin(GL_POLYGON);
    glVertex2f(950, 40);
    glVertex2f(1020, 40);
    glVertex2f(1010, 0);
    glVertex2f(940, 0);
    glEnd();

    // ================= DEAD FISH 1 =================
    glColor3ub(160, 160, 160);
    glBegin(GL_POLYGON);
    glVertex2f(120, -90);
    glVertex2f(170, -80);
    glVertex2f(210, -100);
    glVertex2f(170, -120);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(120, -90);
    glVertex2f(90, -70);
    glVertex2f(90, -110);
    glEnd();

    // Fish eye
    glColor3ub(0, 0, 0);
    glBegin(GL_POINTS);
    glVertex2f(195, -100);
    glEnd();

    // ================= DEAD FISH 2 =================
    glColor3ub(150, 150, 150);
    glBegin(GL_POLYGON);
    glVertex2f(350, -200);
    glVertex2f(400, -190);
    glVertex2f(440, -210);
    glVertex2f(400, -230);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(350, -200);
    glVertex2f(320, -180);
    glVertex2f(320, -220);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(425, -210);
    glEnd();

    // ================= DEAD FISH 3 =================
    glColor3ub(140, 140, 140);
    glBegin(GL_POLYGON);
    glVertex2f(700, -180);
    glVertex2f(750, -170);
    glVertex2f(790, -190);
    glVertex2f(750, -210);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(700, -180);
    glVertex2f(670, -160);
    glVertex2f(670, -200);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(775, -190);
    glEnd();
}



void rifandrawDustbin()
{

    // ================= TRASH CAN BODY =================
    glBegin(GL_POLYGON);
    glColor3ub(0, 180, 0); // Green bin
    glVertex2f(-1130, -450);
    glVertex2f(-990, -450);
    glVertex2f(-1020, -190);
    glVertex2f(-1100, -190);
    glEnd();

    // Body outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glVertex2f(-1130, -450);
    glVertex2f(-990, -450);
    glVertex2f(-1020, -190);
    glVertex2f(-1100, -190);
    glEnd();

    // ================= TRASH CAN LID =================
    glBegin(GL_POLYGON);
    glColor3ub(0, 150, 0);
    glVertex2f(-1150, -190);
    glVertex2f(-970, -190);
    glVertex2f(-995, -155);
    glVertex2f(-1125, -155);
    glEnd();

    // Lid outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glVertex2f(-1150, -190);
    glVertex2f(-970, -190);
    glVertex2f(-995, -155);
    glVertex2f(-1125,-155);
    glEnd();

    // ================= RECYCLE SYMBOL =================
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glLineWidth(3);
    glVertex2f(-1085, -330);
    glVertex2f(-1060, -280);

    glVertex2f(-1060, -280);
    glVertex2f(-1035, -330);

    glVertex2f(-1035, -330);
    glVertex2f(-1085, -330);
    glEnd();

    glFlush();
}



void rifandrawShip() {

    float x = rifanshipX , y = 380; // Ship base position (waterline)

    // Lower Hull - black and red base
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0); // Black upper hull
    glVertex2f(x, y);
    glVertex2f(x + 500, y);
    glVertex2f(x + 450, y - 50);
    glVertex2f(x + 50, y - 50);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(178, 34, 34); // Red lower hull
    glVertex2f(x + 50, y - 50);
    glVertex2f(x + 450, y - 50);
    glVertex2f(x + 400, y - 100);
    glVertex2f(x + 100, y - 100);
    glEnd();

    // Deck
    glBegin(GL_POLYGON);
    glColor3ub(220, 220, 220); // Light gray
    glVertex2f(x + 100, y + 30);
    glVertex2f(x + 400, y + 30);
    glVertex2f(x + 380, y);
    glVertex2f(x + 120, y);
    glEnd();

    // Cabin 1
    glBegin(GL_POLYGON);
    glColor3ub(245, 245, 245);
    glVertex2f(x + 140, y + 30);
    glVertex2f(x + 180, y + 30);
    glVertex2f(x + 180, y + 80);
    glVertex2f(x + 140, y + 80);
    glEnd();

    // Cabin 2 (with red roof)
    glBegin(GL_POLYGON);
    glColor3ub(245, 245, 245);
    glVertex2f(x + 200, y + 30);
    glVertex2f(x + 280, y + 30);
    glVertex2f(x + 280, y + 80);
    glVertex2f(x + 200, y + 80);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(200, 0, 0); // Red roof
    glVertex2f(x + 195, y + 80);
    glVertex2f(x + 285, y + 80);
    glVertex2f(x + 280, y + 90);
    glVertex2f(x + 200, y + 90);
    glEnd();

    // Tower
    glBegin(GL_POLYGON);
    glColor3ub(240, 240, 240);
    glVertex2f(x + 240, y + 80);
    glVertex2f(x + 300, y + 80);
    glVertex2f(x + 280, y + 140);
    glVertex2f(x + 260, y + 140);
    glEnd();

    // Antenna
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2f(x + 270, y + 140);
    glVertex2f(x + 270, y + 160);
    glEnd();

    // Windows (long row)
    glColor3ub(10, 10, 60);
    for (int i = 0; i < 8; i++) {
        glBegin(GL_POLYGON);
        glVertex2f(x + 110 + i * 40, y + 5);
        glVertex2f(x + 130 + i * 40, y + 5);
        glVertex2f(x + 130 + i * 40, y + 25);
        glVertex2f(x + 110 + i * 40, y + 25);
        glEnd();
    }

    // Windows on tower
    glColor3ub(10, 10, 60);
    glBegin(GL_POLYGON);
    glVertex2f(x + 250, y + 90);
    glVertex2f(x + 270, y + 90);
    glVertex2f(x + 270, y + 110);
    glVertex2f(x + 250, y + 110);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 270, y + 90);
    glVertex2f(x + 290, y + 90);
    glVertex2f(x + 290, y + 110);
    glVertex2f(x + 270, y + 110);
    glEnd();

    // Mast
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3ub(0, 0, 0);
    glVertex2f(x + 400, y);
    glVertex2f(x + 400, y + 200);
    glEnd();

    // Sail (white curved triangle)
    glBegin(GL_TRIANGLES);
    glColor3ub(255, 255, 255);
    glVertex2f(x + 400, y + 200);  // Top
    glVertex2f(x + 400, y);        // Bottom of mast
    glVertex2f(x + 500, y);        // Outer bottom corner
    glEnd();

}



void rifandrawVerticalWaveBulges() {

    //glColor3ub(79, 202, 244);
    glColor3ub(70 , 193 ,202 );
    glBegin(GL_POLYGON);

    // EXTREME LEFT
    glVertex2f(-150, 280);

    for (float y = 380; y >= -480; y -= 60) {
        float x = -150 + 65 * sin((y + rifanwaveOffsetOcean) * 0.04);
        glVertex2f(x, y);
    }

    // Bottom stays correct
    glVertex2f(100, -500);

    glEnd();
}



void rifanhouse2()
{
    glBegin(GL_POLYGON); //LEFT HOUSE
    glColor3ub(200, 20, 77);
    glVertex2f(-1200,450);//G1
    glVertex2f(-1040,390);//H1
    glVertex2f(-1040,275);//I1
    glVertex2f(-1200,275);//J1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); //LEFT HOUSE
    glColor3ub(0,0,0);
    glVertex2f(-1200,450);//G1
    glVertex2f(-1040,390);//H1
    glVertex2f(-1040,275);//I1
    glVertex2f(-1200,275);//J1
    glEnd();

    glBegin(GL_POLYGON); // ROOF
    glColor3ub(230, 185, 18);
    glVertex2f(-1200,450);//G1
    glVertex2f(-1040,390);//H1
    glVertex2f(-1020,360);//K1
    glVertex2f(-1200,390);//L1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // ROOF
    glColor3ub(0,0,0);
    glVertex2f(-1200,450);//G1
    glVertex2f(-1040,390);//H1
    glVertex2f(-1020,360);//K1
    glVertex2f(-1200,390);//L1
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(196, 178, 103); //LOWER PART
    glVertex2f(-1200,275);//J1
    glVertex2f(-1040,275);//I1
    glVertex2f(-1000,260);//M1
    glVertex2f(-1200,240);//N1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0); //LOWER PART
    glVertex2f(-1200,275);//J1
    glVertex2f(-1040,275);//I1
    glVertex2f(-1000,260);//M1
    glVertex2f(-1200,240);//N1
    glEnd();


    glBegin(GL_POLYGON);//WINDOW
    glColor3ub(20, 199, 234);
    glVertex2f(-1080,360);//O1
    glVertex2f(-1080,300);//P1
    glVertex2f(-1170,300);//Q1
    glVertex2f(-1170,375);//R1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);//WINDOW
    glColor3ub(0,0,0);
    glVertex2f(-1080,360);//O1
    glVertex2f(-1080,300);//P1
    glVertex2f(-1170,300);//Q1
    glVertex2f(-1170,375);//R1
    glEnd();

    //UMBRELLA

    glBegin(GL_POLYGON);
    glColor3ub(196, 71, 17);
    glVertex2f(-980,530);//W
    glVertex2f(-1150,450);//Z
    glVertex2f(-820,450);//A1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(-980,530);//W
    glVertex2f(-1150,450);//Z
    glVertex2f(-820,450);//A1
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3ub(0,0,0);

    glVertex2f(-980,530);//W
    glVertex2f(-1050,450);//B1

    glVertex2f(-980,530);//W
    glVertex2f(-1010,450);//C1

    glVertex2f(-980,530);//W
    glVertex2f(-960,450);//D1

    glVertex2f(-980,530);//W
    glVertex2f(-930,450);//E1

    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex2f(-1130,450);//Z
    glVertex2f(-1130,220);//F1
     glEnd();


}


void rifanhouse1()
{
    glBegin(GL_POLYGON); // Lower part back
    glColor3ub(196, 178, 103);
    glVertex2f(-950, 300); //m
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 210);//p
    glVertex2f(-1000, 270);//o
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Lower part back
    glColor3ub(0,0,0);
    glVertex2f(-950, 300); //m
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 210);//p
    glVertex2f(-1000, 270);//o
    glEnd();

    glBegin(GL_POLYGON); // Lower part front
    glColor3ub(196, 178, 103);
    glVertex2f(-410, 250);//n
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 210);//p
    glVertex2f(-370, 210);//q
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Lower part front
    glColor3ub(0,0,0);
    glVertex2f(-410, 250);//n
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 210);//p
    glVertex2f(-370, 210);//q
    glEnd();

    glBegin(GL_POLYGON); // House body
    glColor3ub(232, 87, 23);
    glVertex2f(-950, 390);//j
    glVertex2f(-680, 400);//k
    glVertex2f(-680, 250);//l
    glVertex2f(-950, 300);//m
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // House body
    glColor3ub(0,0,0);
    glVertex2f(-950, 390);//j
    glVertex2f(-680, 400);//k
    glVertex2f(-680, 250);//l
    glVertex2f(-950, 300);//m
    glEnd();

    glBegin(GL_POLYGON); // Front body
    glColor3ub(196, 71, 17);
    glVertex2f(-520, 500);//i
    glVertex2f(-410, 370);//h
    glVertex2f(-410, 250);//n
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 400);//k
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Front body
    glColor3ub(0,0,0);
    glVertex2f(-520, 500);//i
    glVertex2f(-410, 370);//h
    glVertex2f(-410, 250);//n
    glVertex2f(-680, 250);//l
    glVertex2f(-680, 400);//k
    glEnd();


    rifancircle(50,-510,340, 43, 160, 214);

    glBegin(GL_POLYGON); //GATE
    glColor3ub(43, 160, 214);
    glVertex2f(-560,360);
    glVertex2f(-460,360);
    glVertex2f(-460,250);
    glVertex2f(-560,250);
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3ub(0,0,0);
    glVertex2f(-510,390);
    glVertex2f(-510,250);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(43, 160, 214); //WINDOW1
    glVertex2f(-780,355);//H3
    glVertex2f(-730,360);//I3
    glVertex2f(-730,280);//J3
    glVertex2f(-780,295);//K3
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0); //WINDOW1
    glVertex2f(-780,355);//H3
    glVertex2f(-730,360);//I3
    glVertex2f(-730,280);//J3
    glVertex2f(-780,295);//K3
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(43, 160, 214); //WINDOW2
    glVertex2f(-850,360);//H3
    glVertex2f(-800,360);//I3
    glVertex2f(-800,285);//J3
    glVertex2f(-850,295);//K3
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);   //WINDOW2
    glVertex2f(-850,360);//H3
    glVertex2f(-800,360);//I3
    glVertex2f(-800,285);//J3
    glVertex2f(-850,295);//K3
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(43, 160, 214); //WINDOW3
    glVertex2f(-920,365);//H3
    glVertex2f(-870,365);//I3
    glVertex2f(-870,290);//J3
    glVertex2f(-920,300);//K3
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);      //WINDOW3
    glVertex2f(-920,365);//H3
    glVertex2f(-870,365);//I3
    glVertex2f(-870,290);//J3
    glVertex2f(-920,300);//K3
    glEnd();

    glBegin(GL_POLYGON); // Dark SHADOW side
    glColor3ub(148, 118, 7);
    glVertex2f(-460, 540);//b
    glVertex2f(-350, 376);//e
    glVertex2f(-410, 370);//h
    glVertex2f(-520, 500);//i
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Dark SHADOW side
    glColor3ub(0,0,0);
    glVertex2f(-460, 540);//b
    glVertex2f(-350, 376);//e
    glVertex2f(-410, 370);//h
    glVertex2f(-520, 500);//i
    glEnd();

    glBegin(GL_POLYGON); // Signboard
    glColor3ub(151, 76, 63);
    glVertex2f(-420, 400);//u
    glVertex2f(-575, 400);//t
    glVertex2f(-580, 450);//r
    glVertex2f(-415, 450);//s
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Signboard
    glColor3ub(0,0,0);
    glVertex2f(-420, 400);//u
    glVertex2f(-575, 400);//t
    glVertex2f(-580, 450);//r
    glVertex2f(-415, 450);//s
    glEnd();

    glBegin(GL_POLYGON); // Roof
    glColor3ub(230, 185, 18);
    glVertex2f(-860, 500);//a
    glVertex2f(-460, 540);//b
    glVertex2f(-650, 400);//c
    glVertex2f(-1000, 380);//d
    glEnd();

    glLineWidth(2);
    glBegin(GL_LINE_LOOP); // Roof
    glColor3ub(0,0,0);
    glVertex2f(-860, 500);//a
    glVertex2f(-460, 540);//b
    glVertex2f(-650, 400);//c
    glVertex2f(-1000, 380);//d
    glEnd();

}


// Draw a single trunk segment (rounded rectangle)
void rifandrawTrunkSegment(float x, float y, float width, float height) {
    glColor3ub(139, 69, 19); // SaddleBrown
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * 3.1416 / 180.0;
        float dx = width / 2 * cos(angle);
        float dy = height / 2 * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// Draw the full curved trunk with segments
void rifandrawCurvedTrunk(float startX, float startY) {
    float x = startX;
    float y = startY;
    float width = 28;
    float height = 30;

    for (int i = 0; i < 20; i++) {
        rifandrawTrunkSegment(x, y, width, height);
        x += 10;  // slight curve to right
        y += 25;  // move up
    }
}

// Draw a straight palm trunk
void rifandrawStraightTrunk(float startX, float startY) {
    float x = startX;
    float y = startY;
    float width = 20;
    float height = 30;

    for (int i = 0; i < 26; i++) {
        rifandrawTrunkSegment(x, y, width, height);
        y += 25;  // straight up
    }
}

// Draw one palm leaf with jagged edges
void rifandrawPalmLeaf(float cx, float cy, float angleDeg) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angleDeg, 0, 0, 1);

    glColor3ub(34, 139, 34); // Forest Green
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);       // base of leaf (attached to trunk)
    glVertex2f(140, 65);
    glVertex2f(180, 50);
    glVertex2f(140, 30);
    glVertex2f(180, 15);
    glVertex2f(140, 0);
    glEnd();

    glPopMatrix();
}

// Draw one **shorter** palm leaf for the straight tree
void rifandrawShortPalmLeaf(float cx, float cy, float angleDeg) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(angleDeg, 0, 0, 1);

    glColor3ub(34, 139, 34); // Forest Green
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);       // base of leaf
    glVertex2f(110, 55);
    glVertex2f(140, 40);
    glVertex2f(110, 20);
    glVertex2f(140, 10);
    glVertex2f(110, 0);
    glEnd();

    glPopMatrix();
}

// Draw the full set of palm leaves
void rifandrawAllLeaves(float centerX, float centerY) {
    rifandrawPalmLeaf(centerX, centerY, -150);
    rifandrawPalmLeaf(centerX, centerY, -120);
    rifandrawPalmLeaf(centerX, centerY, -90);
    rifandrawPalmLeaf(centerX, centerY, -60);
    rifandrawPalmLeaf(centerX, centerY, -30);
    rifandrawPalmLeaf(centerX, centerY, 0);
    rifandrawPalmLeaf(centerX, centerY, 30);
    rifandrawPalmLeaf(centerX, centerY, 60);
    rifandrawPalmLeaf(centerX, centerY, 90);
    rifandrawPalmLeaf(centerX, centerY, 120);
    rifandrawPalmLeaf(centerX, centerY, 150);
    rifandrawPalmLeaf(centerX, centerY, 180);
}

// Draw full leaves for straight palm
void rifandrawShortLeaves(float centerX, float centerY) {
    rifandrawShortPalmLeaf(centerX, centerY, -150);
    rifandrawShortPalmLeaf(centerX, centerY, -120);
    rifandrawShortPalmLeaf(centerX, centerY, -90);
    rifandrawShortPalmLeaf(centerX, centerY, -60);
    rifandrawShortPalmLeaf(centerX, centerY, -30);
    rifandrawShortPalmLeaf(centerX, centerY, 0);
    rifandrawShortPalmLeaf(centerX, centerY, 30);
    rifandrawShortPalmLeaf(centerX, centerY, 60);
    rifandrawShortPalmLeaf(centerX, centerY, 90);
    rifandrawShortPalmLeaf(centerX, centerY, 120);
    rifandrawShortPalmLeaf(centerX, centerY, 150);
    rifandrawShortPalmLeaf(centerX, centerY, 180);
}

void rifanstoneGrass()
{
    //GRASS
    glBegin(GL_POLYGON);
    glColor3ub(21, 182, 13);
    glVertex2f(-1025,150);
    glVertex2f(-1055,230);
    glVertex2f(-1075,150);
    glEnd();

      //GRASS
    glBegin(GL_POLYGON);
    glColor3ub(126, 222, 19);
    glVertex2f(-1010,120);
    glVertex2f(-1030,200);
    glVertex2f(-1060,120);
    glEnd();

    //GRASS
    glBegin(GL_POLYGON);
    glColor3ub(126, 222, 19);
    glVertex2f(-1040,120);
    glVertex2f(-1090,200);
    glVertex2f(-1070,120);
    glEnd();

     //STONE
    glBegin(GL_POLYGON);
    glColor3ub(111, 135, 113);
    glVertex2f(-1010,165);//w4
    glVertex2f(-1060,155);//z4
    glVertex2f(-1040,160);//a5
    glVertex2f(-1080,115);//b5
    glVertex2f(-980,100);//c5
    glVertex2f(-1000,150);//d5
    glEnd();

    //STONE
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(-1010,165);//w4
    glVertex2f(-1060,155);//z4
    glVertex2f(-1040,160);//a5
    glVertex2f(-1080,115);//b5
    glVertex2f(-980,100);//c5
    glVertex2f(-1000,150);//d5
    glEnd();

    //STONE
    glBegin(GL_POLYGON);
    glColor3ub(100, 137, 98);
    glVertex2f(-1050,155);//w4
    glVertex2f(-1100,145);//z4
    glVertex2f(-1120,105);//b5
    glVertex2f(-1020,90);//c5
    glVertex2f(-1040,140);//d5
    glEnd();

    //STONE
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(-1050,155);//w4
    glVertex2f(-1100,145);//z4
    glVertex2f(-1120,105);//b5
    glVertex2f(-1020,90);//c5
    glVertex2f(-1040,140);//d5
    glEnd();

    //GRASS
    glBegin(GL_POLYGON);
    glColor3ub(21, 182, 13);
    glVertex2f(-1155,140);
    glVertex2f(-1140,210);
    glVertex2f(-1200,140);
    glEnd();

    //STONE
    glBegin(GL_POLYGON);
    glColor3ub(111, 135, 113);
    glVertex2f(-1160,165);//w4
    glVertex2f(-1210,155);//z4
    glVertex2f(-1230,115);//b5
    glVertex2f(-1130,100);//c5
    glVertex2f(-1150,150);//d5
    glEnd();

    //STONE
    glBegin(GL_LINE_LOOP);
    glColor3ub(0,0,0);
    glVertex2f(-1160,165);//w4
    glVertex2f(-1210,155);//z4
    glVertex2f(-1230,115);//b5
    glVertex2f(-1130,100);//c5
    glVertex2f(-1150,150);//d5
    glEnd();


}


void rifandrawStones() {

    // ---- Stone 2 BACK  ----
    glColor3ub(120, 72, 40); // Brown fill
    glBegin(GL_POLYGON);
    glVertex2f(900, -500);
    glVertex2f(940, -350);
    glVertex2f(970, -330);
    glVertex2f(1050, -380);
    glVertex2f(1080, -500);
    glEnd();

    glColor3ub(30, 20, 10); // Dark border
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(900, -500);
    glVertex2f(940, -350);
    glVertex2f(970, -330);
    glVertex2f(1050, -380);
    glVertex2f(1080, -500);
    glEnd();


    // ---- Stone 1 (Bigger) ----
    glColor3ub(101, 67, 33); // Brown fill
    glBegin(GL_POLYGON);
    glVertex2f(770, -500);
    glVertex2f(830, -390);
    glVertex2f(870, -370);
    glVertex2f(930, -410);
    glVertex2f(980, -500);
    glEnd();

    glColor3ub(30, 20, 10); // Dark border
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(770, -500);
    glVertex2f(830, -390);
    glVertex2f(870, -370);
    glVertex2f(930, -410);
    glVertex2f(980, -500);
    glEnd();

    // ---- Stone 2 (Bigger) ----
    glColor3ub(120, 72, 40); // Brown fill
    glBegin(GL_POLYGON);
    glVertex2f(970, -500);
    glVertex2f(1005, -420);
    glVertex2f(1040, -400);
    glVertex2f(1070, -450);
    glVertex2f(1100, -500);
    glEnd();

    glColor3ub(30, 20, 10); // Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(970, -500);
    glVertex2f(1005, -420);
    glVertex2f(1040, -400);
    glVertex2f(1070, -450);
    glVertex2f(1100, -500);
    glEnd();

    // ---- Stone 3 BACK ----
    glColor3ub(110, 60, 30); // Brown fill
    glBegin(GL_POLYGON);
    glVertex2f(1050, -500);
    glVertex2f(1100, -420);
    glVertex2f(1125, -400);
    glVertex2f(1155, -440);
    glVertex2f(1175, -500);
    glEnd();

    glColor3ub(30, 20, 10); // Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(1050, -500);
    glVertex2f(1100, -420);
    glVertex2f(1125, -400);
    glVertex2f(1155, -440);
    glVertex2f(1175, -500);
    glEnd();


    // ---- Stone 4 (Bigger) ----
    glColor3ub(110, 60, 30); // Brown fill
    glBegin(GL_POLYGON);
    glVertex2f(1140, -500);
    glVertex2f(1165, -470);
    glVertex2f(1195, -460);
    glVertex2f(1225, -490);
    glVertex2f(1245, -500);
    glEnd();

    glColor3ub(30, 20, 10); // Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(1140, -500);
    glVertex2f(1165, -470);
    glVertex2f(1195, -460);
    glVertex2f(1225, -490);
    glVertex2f(1245, -500);
    glEnd();
}

void rifandrawDrops() {
    glColor3f(0.2f, 0.4f, 1.0f);
    for (int i = 0; i < rifandropCount; ++i) {
        glBegin(GL_LINES);
        glVertex2f(rifandropX[i], rifandropY[i]);
        glVertex2f(rifandropX[i], rifandropY[i] - 10.0f);
        glEnd();
    }
}

void rifandrawBubbles() {
    for (int i = 0; i < rifanbubbleCount; ++i) {
        if (rifanbubbleActive[i]) {
            rifandrawCircle(rifanbubbleX[i], rifanbubbleY[i], rifanbubbleRadius[i], rifanbubbleAlpha[i]);
        }
    }
}


void rifanupdateRain(int value) {
    if (rifanrainActive) {
        if (rifandropCount < MAX_DROPS && (rand() % 3) == 0) {
            rifandropX[rifandropCount] = (rand() % 2400) - 1200;
            rifandropY[rifandropCount] = 1000.0f;
            rifandropSpeed[rifandropCount] = 5.0f + (rand() % 30 / 10.0f);
            rifandropCount++;
        }

        for (int i = 0; i < rifandropCount;) {
            rifandropY[i] -= rifandropSpeed[i];
            if (rifandropY[i] <= rifanrainGroundY) {
                if (rifanbubbleCount < MAX_BUBBLES) {
                    rifanbubbleX[rifanbubbleCount] = rifandropX[i];
                    rifanbubbleY[rifanbubbleCount] = rifanrainGroundY;
                    rifanbubbleRadius[rifanbubbleCount] = 8.0f;
                    rifanbubbleAlpha[rifanbubbleCount] = 1.0f;
                    rifanbubbleActive[rifanbubbleCount] = 1;
                    rifanbubbleCount++;
                }
                rifandropX[i] = rifandropX[rifandropCount - 1];
                rifandropY[i] = rifandropY[rifandropCount - 1];
                rifandropSpeed[i] = rifandropSpeed[rifandropCount - 1];
                rifandropCount--;
            } else {
                i++;
            }
        }

        for (int i = 0; i < rifanbubbleCount; ++i) {
            if (rifanbubbleActive[i]) {
                rifanbubbleRadius[i] += 0.8f;
                rifanbubbleAlpha[i] -= 0.01f;
                if (rifanbubbleAlpha[i] <= 0.0f) {
                    rifanbubbleActive[i] = 0;
                }
            }
        }

        glutPostRedisplay();
    }

    glutTimerFunc(30, rifanupdateRain, 0);
}



// Function to render text with shadow and spacing
void rifanrenderTextWithShadow(float x, float y, const char* text, float spacing) {
    // --- Shadow Layer ---
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); // Shadow color
    glTranslatef(x - 5, y - 5, 0); // Offset for shadow
    glScalef(0.2f, 0.15f, 1.0f);
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glTranslatef(spacing, 0, 0); // Add spacing
    }
    glPopMatrix();

    // --- Main Text Layer ---
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); // Text color
    glTranslatef(x, y, 0); // Main position
    glScalef(0.2f, 0.15f, 1.0f);
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glTranslatef(spacing, 0, 0); // Add spacing
    }
    glPopMatrix();
}


void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Background color
    glClear(GL_COLOR_BUFFER_BIT);        // Clear screen
    glLineWidth(2);

    rifansky();

    rifanocean();
      rifandrawVerticalWaveBulges();


      // Draw grass using a loop
   for (int i = 80; i <= 2080; i += 30)
   {
         rifangrass(i, 0, 0);
   }

    rifandrawShip();

    rifantree1(-680,-80,0);
    rifantree1(-550,-80,0);
    rifantree1(-610,-100,0);


    rifanhouse2();
    rifanhouse1();

    // Draw "SHOP" with shadow, spaced out
    rifanrenderTextWithShadow(-570, 415, "RESORT", 55); // Start position(x,y), "name" ,spacing between letters

    // Left: straight palm tree
    rifandrawStraightTrunk(-1180, 135);   // Trunk from bottom
   rifandrawShortLeaves(-1180, 785);  // Leaves at top of trunk

     //Right curve tree
    rifandrawCurvedTrunk(-1030,135);     // Trunk from bottom
    rifandrawAllLeaves(-830,635);    // Leaves at top of trunk

    rifanstoneGrass();
    rifandrawStones();

   rifandrawDustbin();

   rifandrawSeaTrash();


      if (rifanrainActive) {
        rifandrawDrops();
        rifandrawBubbles();
    }


    rifandrawPlane(rifanplaneX, rifanplaneY, rifanplaneScale);



    glFlush();
    glutSwapBuffers();  // for smooth animation

}

//WAVE ANIMATION
void rifanupdateOcean(int value) {
    if (!rifanoceanWaveActive) return; // Only animate if flag is active

    rifanwaveOffsetOcean += 1.5f;
    if (rifanwaveOffsetOcean > 360.0f) rifanwaveOffsetOcean -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(30, rifanupdateOcean, 0);
}

void rifanupdateShip(int value) {

    if (rifanmoveShip) {
        rifanshipX += rifanshipDirection * 2.0f; // Adjust speed here

        if (rifanshipX > 1000) { // Assuming screen width ~1280
            rifanshipX = -300; // Reset to left of screen (off-screen)
        }

        glutPostRedisplay();
    }

    // Call again after 50 milliseconds
    glutTimerFunc(50, rifanupdateShip, 0);
}


// Animate moon rise
void rifanupdateMoon(int value) {
    if (rifanmoonY < 900) {
        rifanmoonY += 1.5;
        glutPostRedisplay();
        glutTimerFunc(50, rifanupdateMoon, 0);
    }
}

// Animate star blinking
void rifanupdateStars(int value) {
    rifanstarBlink++;
    glutPostRedisplay();
    glutTimerFunc(500, rifanupdateStars, 0);
}



// Update sun and background
void rifanupdateSun(int value) {
    if (!rifansunset) {
        rifansunX += 2;
        rifansunY -= 1;

        // Transition to evening (orange)
        if (rifanbgR < 255) rifanbgR += 0.5;
        if (rifanbgG > 165) rifanbgG -= 0.5;
        if (rifanbgB > 0)   rifanbgB -= 1.0;

        if (rifansunX >= 600) {
            rifansunset = true;
            rifanmoonRise = false;
        }

        glutPostRedisplay();
        glutTimerFunc(50, rifanupdateSun, 0);

    } else if (!rifanmoonRise) {
        // Transition to night (dark blue)
        if (rifanbgR > 25) rifanbgR -= 0.5;
        if (rifanbgG > 50) rifanbgG -= 0.5;
        if (rifanbgB < 112) rifanbgB += 0.5;

        if (rifanbgR <= 25 && rifanbgG <= 50 && rifanbgB >= 112) {
            rifanmoonRise = true;
            rifanshowStars = true;
            glutTimerFunc(0, rifanupdateMoon, 0);
            glutTimerFunc(0, rifanupdateStars, 0);
        } else {
            glutPostRedisplay();
            glutTimerFunc(50, rifanupdateSun, 0);
        }
    }
}

void rifanupdatePlane(int value) {
    if (rifanmovePlane) {
        rifanplaneX += 5.0f; // Move right

        // If the plane goes off-screen right, reset to left
        if (rifanplaneX > 1300) {
            rifanplaneX = -1300;
        }

        glutPostRedisplay(); // Redraw scene
        glutTimerFunc(30, rifanupdatePlane, 0); // Repeat timer
    }
}



void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            rifanoceanWaveActive = true;
            glutTimerFunc(0, rifanupdateOcean, 0);
            break;
        case 'n':
        case 'N':
            glutTimerFunc(0, rifanupdateSun, 0);
            break;
        case 's':
        case 'S':
            rifanmoveShip = true;
            glutTimerFunc(0, rifanupdateShip, 0);
            break;
        case 'r':
        case 'R':
            rifanrainActive = !rifanrainActive;
            glutTimerFunc(0,rifanupdateRain,0);
            break;
        case 'p':
        case 'P':
            rifanmovePlane = true;
            glutTimerFunc(0, rifanupdatePlane, 0);
            break;


        default:
            break;
    }
}

// Mouse click to toggle back to day if clicking moon
void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && rifansunset && rifanmoonRise) {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        float openglX = (float)x / winWidth * (1200 - (-1200)) + (-1200);
        float openglY = (float)(winHeight - y) / winHeight * (1000 - (-500)) + (-500);

        float dx = openglX - 900;
        float dy = openglY - rifanmoonY;

        if (dx * dx + dy * dy <= 80 * 80) {
            // Reset to day mode
            rifansunX = -830;
            rifansunY = 900;
            rifanbgR = 194;
            rifanbgG = 237;
            rifanbgB = 245;
            rifanmoonY = 700;
            rifansunset = false;
            rifanmoonRise = false;
            rifanshowStars = false;
            rifanstarBlink = 0;
            glutPostRedisplay();
        }
    }

}



void rifaninit() {
    srand((unsigned int)time(0));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Water Pollution Rifan");
    glutInitWindowSize(1200,600);
    glutInitWindowPosition(100, 100);
    gluOrtho2D(-1200, 1200, -500, 1000); // Coordinate system
    glutDisplayFunc(display);
    glMatrixMode(GL_PROJECTION);
    rifaninit();
    glLoadIdentity();
    glutTimerFunc(0, rifanupdateOcean, 0);
    glutTimerFunc(0, rifanupdateShip, 0);
    glutTimerFunc(0, rifanupdateRain, 0);
    glutTimerFunc(0, rifanupdatePlane, 0);

    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouse);  // Register mouse callback

    glutMainLoop();
    return 0;
}
