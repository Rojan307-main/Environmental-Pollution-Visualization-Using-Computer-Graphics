#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>

// --- Global Variables ---

// Clouds
float cloudPosition[4] = { -3.2f, -2.2f, -1.2f, -0.2f };
float cloudSpeed = 0.008f;

// Birds
float birdX[3] = { -1.5f, -1.8f, -2.1f };
float birdY[3] = { 0.5f, 0.5f, 0.5f };
float birdSpeed = 0.015f;

// Cars (Green, Blue, Red)
float carX[3]       = { -4.2f,  -2.8f,  -1.4f };
float carY[3]       = { -0.72f, -0.66f, -0.60f };
float carSpeed[3]   = { 0.011f, 0.0095f, 0.008f };
float carColors[3][3] = {
    {0.0f, 0.8f, 0.0f},   // Green
    {0.0f, 0.0f, 1.0f},   // Blue
    {1.0f, 0.0f, 0.0f}    // Red
};

// Buses
float busXArray[2]    = { -3.0f,  0.8f };
float busYArray[2]    = { -0.46f, -0.42f };
float busSpeedArray[2] = { 0.0055f, 0.0048f };

// Margins for resetting off-screen
const float CAR_RESET_MARGIN = 0.80f;
const float BUS_RESET_MARGIN = 1.00f;

float skyLimit = 0.2f;
GLfloat orthoLeft, orthoRight, orthoBottom, orthoTop;

float sunY = 0.6f;
float moonY = 0.6f;
bool isDay = true;

// Stars
float stars[40][2] = {
    {-1.8f,0.85f},{-1.5f,0.7f},{-1.2f,0.9f},{-0.9f,0.75f},
    {-0.6f,0.85f},{-0.3f,0.7f},{0.0f,0.9f},{0.3f,0.8f},
    {0.6f,0.85f},{0.9f,0.7f},{1.2f,0.9f},{1.5f,0.8f},
    {1.8f,0.85f},{-1.7f,0.6f},{-1.3f,0.55f},{-0.9f,0.6f},
    {-0.5f,0.5f},{-0.1f,0.6f},{0.3f,0.55f},{0.7f,0.6f},
    {1.1f,0.5f},{1.5f,0.6f},{-1.6f,0.4f},{-1.2f,0.45f},
    {-0.8f,0.4f},{-0.4f,0.45f},{0.0f,0.4f},{0.4f,0.45f},
    {0.8f,0.4f},{1.2f,0.45f},{1.6f,0.4f},{-1.4f,0.3f},
    {-0.7f,0.3f},{0.0f,0.3f},{0.7f,0.3f},{1.4f,0.3f}
};

// --- Drawing Functions ---

void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float a = 2.0f * 3.1415926f * i / 100;
        glVertex2f(x + r * cosf(a), y + r * sinf(a));
    }
    glEnd();
}

void drawPolygon(const float* v, int c) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < c; i++)
        glVertex2f(v[i*2], v[i*2+1]);
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
    glEnd();
}

void drawCloud(float x, float y) {
    glColor3f(1,1,1);
    drawCircle(x - 0.09f, y, 0.11f);
    drawCircle(x,        y, 0.15f);
    drawCircle(x + 0.09f, y, 0.11f);
}

void drawBird(float x, float y, float s) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(s, s, 1);
    glColor3f(0,0,0);
    float b[] = { -0.5f,0.15f, -0.3f,0.25f, -0.1f,0.15f, -0.3f,0.09f };
    drawPolygon(b, 4);
    float w[] = { -0.4f,0.2f, -0.4f,0.25f, -0.25f,0.35f, -0.3f,0.28f, -0.35f,0.2f };
    drawPolygon(w, 5);
    drawTriangle(-0.4f, 0.2f, -0.2f, 0.31f, -0.1f, 0.22f);
    drawCircle(-0.45f, 0.18f, 0.015f);
    glPopMatrix();
}

void drawCar(float x, float y, float s, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(s, s, 1);
    glColor3f(r,g,b);
    float body[] = {-0.2f,0, -0.2f,0.1f, 0.2f,0.1f, 0.2f,0};
    drawPolygon(body, 4);
    float top[]  = {-0.12f,0.1f, -0.12f,0.18f, 0.12f,0.18f, 0.12f,0.1f};
    drawPolygon(top, 4);
    glColor3f(1,1,1);
    drawCircle(-0.12f, 0, 0.03f);
    drawCircle( 0.12f, 0, 0.03f);
    glPopMatrix();
}

void drawBus(float x, float y, float s) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(s * 1.4f, s * 1.4f, 1.0f);
    glColor3f(0.9f, 0.3f, 0.1f);
    float body[] = {-0.35f,0, -0.35f,0.15f, 0.35f,0.15f, 0.35f,0};
    drawPolygon(body, 4);
    glColor3f(0.7f, 0.9f, 1.0f);
    float window[] = {-0.25f,0.08f, -0.25f,0.14f, 0.25f,0.14f, 0.25f,0.08f};
    drawPolygon(window, 4);
    glColor3f(1,1,1);
    drawCircle(-0.22f, 0, 0.035f);
    drawCircle( 0.22f, 0, 0.035f);
    glPopMatrix();
}

void drawCrescentMoon(float x, float y) {
    glColor3f(1,1,1);
    drawCircle(x, y, 0.15f);
    glColor3f(0.05f, 0.05f, 0.2f);
    drawCircle(x + 0.05f, y + 0.02f, 0.15f);
}

void drawZebraBorder(float y) {
    float segment = 0.15f;
    bool yellow = true;
    for (float x = orthoLeft; x < orthoRight; x += segment) {
        if (yellow) glColor3f(1,1,0);
        else        glColor3f(0,0,0);
        glBegin(GL_QUADS);
        glVertex2f(x, y-0.01f);
        glVertex2f(x+segment, y-0.01f);
        glVertex2f(x+segment, y+0.01f);
        glVertex2f(x, y+0.01f);
        glEnd();
        yellow = !yellow;
    }
}

// --- Core Logic ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Sky
    glBegin(GL_QUADS);
    if (isDay) glColor3f(0.5f,0.8f,1.0f);
    else       glColor3f(0.05f,0.05f,0.2f);
    glVertex2f(orthoLeft, skyLimit);
    glVertex2f(orthoRight, skyLimit);
    glVertex2f(orthoRight, orthoTop);
    glVertex2f(orthoLeft, orthoTop);
    glEnd();

    // Grass
    if (isDay) glColor3f(0.4f,0.8f,0.4f);
    else       glColor3f(0.15f,0.3f,0.15f);
    glBegin(GL_QUADS);
    glVertex2f(orthoLeft, -0.4f);
    glVertex2f(orthoRight, -0.4f);
    glVertex2f(orthoRight, skyLimit);
    glVertex2f(orthoLeft, skyLimit);
    glEnd();

    // Road
    if (isDay) glColor3f(0.2f,0.2f,0.2f);
    else       glColor3f(0.08f,0.08f,0.08f);
    glBegin(GL_QUADS);
    glVertex2f(orthoLeft, -0.8f);
    glVertex2f(orthoRight, -0.8f);
    glVertex2f(orthoRight, -0.4f);
    glVertex2f(orthoLeft, -0.4f);
    glEnd();

    // Ground below road
    if (isDay) glColor3f(0.2f,0.6f,0.2f);
    else       glColor3f(0.08f,0.25f,0.08f);
    glBegin(GL_QUADS);
    glVertex2f(orthoLeft, orthoBottom);
    glVertex2f(orthoRight, orthoBottom);
    glVertex2f(orthoRight, -0.8f);
    glVertex2f(orthoLeft, -0.8f);
    glEnd();

    drawZebraBorder(-0.8f);
    drawZebraBorder(-0.4f);

    if (isDay) {
        glColor3f(1,1,0);
        drawCircle(-0.3f, sunY, 0.2f);
        for (int i = 0; i < 4; i++) drawCloud(cloudPosition[i], 0.75f);
        for (int i = 0; i < 3; i++) drawBird(birdX[i], birdY[i], 0.25f);
    } else {
        drawCrescentMoon(-0.3f, moonY);
        glColor3f(1,1,1);
        for (int i = 0; i < 40; i++)
            if (stars[i][1] > skyLimit)
                drawCircle(stars[i][0], stars[i][1], 0.01f);
    }

    for (int i = 0; i < 3; i++)
        drawCar(carX[i], carY[i], 0.75f, carColors[i][0], carColors[i][1], carColors[i][2]);

    for (int i = 0; i < 2; i++)
        drawBus(busXArray[i], busYArray[i], 1.0f);

    glutSwapBuffers();
}

void update(int value) {
    // Clouds
    for (int i = 0; i < 4; i++) {
        cloudPosition[i] += cloudSpeed;
        if (cloudPosition[i] > orthoRight + 0.5f)
            cloudPosition[i] = orthoLeft - 0.5f;
    }

    // Birds
    for (int i = 0; i < 3; i++) {
        birdX[i] += birdSpeed;
        if (birdX[i] > orthoRight + 0.4f)
            birdX[i] = orthoLeft - 0.4f;
    }

    // Cars - UPDATED LOGIC FOR FULL SCREEN
    for (int i = 0; i < 3; i++) {
        carX[i] += carSpeed[i];
        // If car passes right edge + margin, reset to left edge - margin
        if (carX[i] > orthoRight + CAR_RESET_MARGIN) {
            carX[i] = orthoLeft - CAR_RESET_MARGIN - (i * 0.5f); // staggered start
        }
    }

    // Buses
    for (int i = 0; i < 2; i++) {
        busXArray[i] += busSpeedArray[i];
        if (busXArray[i] > orthoRight + BUS_RESET_MARGIN) {
            busXArray[i] = orthoLeft - BUS_RESET_MARGIN - 0.5f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'd' || key == 'D') isDay = true;
    if (key == 'n' || key == 'N') isDay = false;
    if (key == 27) exit(0); // ESC to exit
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w >= h) {
        orthoLeft   = -aspect;
        orthoRight  =  aspect;
        orthoBottom = -1.0f;
        orthoTop    =  1.0f;
    } else {
        orthoLeft   = -1.0f;
        orthoRight  =  1.0f;
        orthoBottom = -1.0f / aspect;
        orthoTop    =  1.0f / aspect;
    }
    gluOrtho2D(orthoLeft, orthoRight, orthoBottom, orthoTop);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 650);
    glutCreateWindow("Road Scene - Optimized Reset");

    // Start in full screen if desired:
    // glutFullScreen();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
