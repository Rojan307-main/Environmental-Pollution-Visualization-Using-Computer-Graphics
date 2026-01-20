

#define _USE_MATH_DEFINES
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <cmath>
#include <string>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

// ---------------- GLOBALS ----------------
float cloudX[4] = {-900, -400, 100, 600};
float cloudY[4] = {650, 680, 660, 630};

const int numBirds = 9;
float birdX[numBirds] = {-1200, -1270, -1340, -1230, -1300, -1370, -1180, -1250, -1320};
float birdY[numBirds] = {550, 530, 560, 510, 490, 520, 580, 590, 570};
float birdHomeY[numBirds] = {550, 530, 560, 510, 490, 520, 580, 590, 570};

float pollutionLevel = 0.0f;

// --- NEW: RAIN GLOBALS ---
const int numRain = 25;
float rainX[numRain];
float rainY[numRain];

const int numCars = 3;
float carX[numCars] = {-1200, -1400, -1600};
float carY[numCars] = {-650, -720, -620};
float carScale[numCars] = {0.5f, 0.45f, 0.55f};
float carSpeed[numCars] = {1.8f, 2.2f, 1.5f};
float carColor[numCars][3] = {{0.9f, 0.1f, 0.1f}, {0.1f, 0.1f, 0.9f}, {0.1f, 0.8f, 0.1f}};

float factoryX[3] = {-850, -620, -390};
float smokeY[3] = {0, 0, 0};

float treeX[7] = {300, 410, 520, 630, 740, 850, 960};

// ---------------- UTILITIES ----------------
void drawCircle(float cx, float cy, float r, int segments){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= segments; i++){
        float a = 2.0f * M_PI * i / segments;
        glVertex2f(cx + r*cos(a), cy + r*sin(a));
    }
    glEnd();
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// ---------------- OBJECTS ----------------
void drawRain() {
    if (pollutionLevel > 0.3f) {
        // Shift from Blue to Acid Green as pollution increases
        float r = 0.2f * pollutionLevel;
        float g = 0.5f + (0.4f * pollutionLevel);
        float b = 1.0f - pollutionLevel;
        glColor3f(r, g, b);

        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for(int i = 0; i < numRain; i++) {
            glVertex2f(rainX[i], rainY[i]);
            glVertex2f(rainX[i] - 5, rainY[i] - 25); // Slanted raindrop
        }
        glEnd();
    }
}

void drawBuilding(float x, float y, float w, float h) {
    float grime = 0.7f - (pollutionLevel * 0.5f);
    glColor3f(grime, grime, grime + 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    float winBrightness = 0.9f - (pollutionLevel * 0.6f);
    glColor3f(winBrightness, winBrightness, 0.4f);
    for(float i = x + 15; i < x + w - 10; i += 30) {
        for(float j = y + 20; j < y + h - 20; j += 40) {
            glBegin(GL_QUADS);
            glVertex2f(i, j);
            glVertex2f(i + 15, j);
            glVertex2f(i + 15, j + 25);
            glVertex2f(i, j + 25);
            glEnd();
        }
    }
}

void drawCloud(float x, float y){
    float gray = 1.0f - (pollutionLevel * 0.7f);
    glColor3f(gray, gray, gray);
    drawCircle(x, y, 35, 30);
    drawCircle(x+40, y+15, 45, 30);
    drawCircle(x+80, y, 35, 30);
}

void drawTree(float x, float y) {
    glColor3f(0.35f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x - 12, y);
    glVertex2f(x + 12, y);
    glVertex2f(x + 12, y + 60);
    glVertex2f(x - 12, y + 60);
    glEnd();
    // Leaves turn "autumn/dead" colors with pollution
    glColor3f(pollutionLevel * 0.6f, 1.0f - (pollutionLevel * 0.5f), 0.0f);
    drawCircle(x, y + 95, 55, 20);
    drawCircle(x - 30, y + 70, 40, 20);
    drawCircle(x + 30, y + 70, 40, 20);
}

void drawBird(float x, float y){
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x+20, y+25);
    glVertex2f(x+25, y+12);
    glVertex2f(x+50, y);
    glVertex2f(x+30, y+25);
    glVertex2f(x+25, y+12);
    glEnd();
}

void drawCar(float x, float y, float s, float r, float g, float b){
    float smokeAlpha = 0.2f + (pollutionLevel * 0.5f);
    glColor4f(0.4f, 0.4f, 0.4f, smokeAlpha);
    drawCircle(x - 25, y + 40*s, 20*s, 15);
    drawCircle(x - 65, y + 50*s, 30*s, 15);
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
    glVertex2f(x + 400*s, y);
    glVertex2f(x + 400*s, y + 120*s);
    glVertex2f(x, y + 120*s);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(x + 80*s, y + 120*s);
    glVertex2f(x + 320*s, y + 120*s);
    glVertex2f(x + 260*s, y + 200*s);
    glVertex2f(x + 140*s, y + 200*s);
    glEnd();
    glColor3f(1,1,1);
    drawCircle(x + 100*s, y - 10*s, 30*s, 30);
    drawCircle(x + 300*s, y - 10*s, 30*s, 30);
}

void drawFactory(float x, float y){
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
    glVertex2f(x+50, y);
        glVertex2f(x+50, y+180);
    glVertex2f(x, y+180);
    glEnd();
    glColor3f(0.55f, 0.55f, 0.55f);
    glBegin(GL_QUADS);
        glVertex2f(x-60, y);
    glVertex2f(x+110, y);
        glVertex2f(x+110, y+110);
    glVertex2f(x-60, y+110);
    glEnd();
}

// ---------------- DISPLAY ----------------
void display(){
    float skyR = 0.6f*(1-pollutionLevel)+0.4f*pollutionLevel;
    float skyG = 0.85f*(1-pollutionLevel)+0.4f*pollutionLevel;
    float skyB = 1.0f*(1-pollutionLevel)+0.4f*pollutionLevel;
    glClearColor(skyR, skyG, skyB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.9f, 0.0f);
    drawCircle(200, 800, 90, 100);

    drawBuilding(-150, -500, 180, 600);
    drawBuilding(100, -500, 150, 750);

    for(int i=0;i<4;i++)
        drawCloud(cloudX[i], cloudY[i]);
    for(int i=0;i<numBirds;i++)
        drawBird(birdX[i], birdY[i]);

    if(pollutionLevel > 0.05f){
        glColor4f(0.25f, 0.25f, 0.25f, pollutionLevel * 0.5f);
        glBegin(GL_QUADS);
            glVertex2f(-1000, -500);
        glVertex2f(1000, -500);
            glVertex2f(1000, 1000);
        glVertex2f(-1000, 1000);
        glEnd();
    }

    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_QUADS);
        glVertex2f(-1000,-1100);
    glVertex2f(1000,-1100);
        glVertex2f(1000,-500);
    glVertex2f(-1000,-500);
    glEnd();
    glColor3f(1,1,1);
    for(float x=-1000;x<1000;x+=80){
        glBegin(GL_LINES); glVertex2f(x,-800); glVertex2f(x+40,-800); glEnd();
    }

    for(int i=0;i<3;i++){
        drawFactory(factoryX[i], -500);
        for(int j=0; j<3; j++){
            glColor4f(0.7f,0.7f,0.7f, 0.4f);
            drawCircle(factoryX[i]+25, -320 + smokeY[i] + j*40, 20 + j*5, 15);
        }
    }
    
    for(int i=0; i<7; i++)
        drawTree(treeX[i], -500);
    for(int i=0;i<numCars;i++)
        drawCar(carX[i], carY[i], carScale[i], carColor[i][0], carColor[i][1], carColor[i][2]);

    drawRain(); // Draw rain over objects

    if(pollutionLevel > 0.5f) {
        glColor3f(1.0f, 0.2f, 0.2f);
        drawText(-280, 920, "STATUS: HAZARDOUS AIR QUALITY");
    } else {
        glColor3f(0.2f, 1.0f, 0.2f);
        drawText(-250, 920, "STATUS: HEALTHY AIR QUALITY");
    }

    glutSwapBuffers();
}

// ---------------- ANIMATION ----------------
void idle(){
    for(int i=0;i<4;i++){
        cloudX[i]+=0.4f;
        if(cloudX[i]>1200) cloudX[i]=-1300;
    }
    for(int i=0;i<numBirds;i++){
        if(birdY[i] > -490) birdX[i] += 2.8f * (1.0f - pollutionLevel);
        if(birdX[i] > 1200) birdX[i] = -1500;
        if(pollutionLevel > 0.6f){
            if(birdY[i] > -500) birdY[i] -= 4.0f;
        } else {
            if(birdY[i] < birdHomeY[i]) birdY[i] += 2.0f;
        }
    }
    for(int i=0;i<numCars;i++){
        carX[i]+=carSpeed[i];
        if(carX[i]>1200) carX[i]=-1500;
    }
    for(int i=0;i<3;i++){
        smokeY[i]+=1.5f + (pollutionLevel*2);
        if(smokeY[i]>250) smokeY[i]=0;
    }

    // Rain Animation
    for(int i=0; i<numRain; i++){
        rainY[i] -= 18.0f;
        if(rainY[i] < -500) {
            rainY[i] = 1000;
            rainX[i] = (rand() % 2000) - 1000;
        }
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    if(key=='r'||key=='R') { if(pollutionLevel<1.0f) pollutionLevel+=0.05f; }
    if(key=='t'||key=='T') { if(pollutionLevel>0.0f) pollutionLevel-=0.05f; }
}

void init(){
    srand(time(0));
    for(int i=0; i<numRain; i++){
        rainX[i] = (rand() % 2000) - 1000;
        rainY[i] = (rand() % 2000) - 1000;
    }
    gluOrtho2D(-1000,1000,-1000,1000);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1200,800);
    glutCreateWindow("Air pollution & Acid Rain");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

