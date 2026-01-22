#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <stdio.h>
#include <mmsystem.h>
#include <string>
#pragma comment(lib, "winmm.lib")

// global variables
float cloudPosition[4] = { -3.2f, -2.2f, -1.2f, -0.2f };  // x positions of 4 clouds
float cloudSpeed = 0.008f;  // cloud movement speed

// bird variables
const int NUM_BIRDS = 15;
float birdPositions[NUM_BIRDS][4];  // [x, y, state, timer] for each bird
float birdSpeed = 0.02f;
float birdReturnTimer = 0.0f;
const float BIRD_RETURN_TIME = 3.0f;

// bird perching positions - x,y coordinates
float perchPositions[][2] = {
    {-1.7f, 0.0f}, {-1.0f, 0.1f}, {-0.25f, 0.0f}, {0.6f, 0.15f}, {1.72f, 0.1f},
    {-1.5f, 0.25f}, {-0.9f, 0.4f}, {0.1f, 0.45f}, {0.9f, 0.45f}, {-1.1f, 0.15f},
    {0.55f, 0.15f}, {1.5f, 0.15f}, {-0.6f, -0.45f}, {0.9f, -0.45f}, {-0.1f, 0.1f}
};

// car variables
float carX[3] = { -4.2f, -2.8f, -1.4f };  // initial x positions
float carY[3] = { -0.72f, -0.66f, -0.60f };  // y positions (different lanes)
float carSpeed[3] = { 0.011f, 0.0095f, 0.008f };  // movement speeds
float carColors[3][3] = {
    {0.0f, 0.8f, 0.0f},  // green car - RGB
    {0.0f, 0.0f, 1.0f},  // blue car
    {1.0f, 0.0f, 0.0f}   // red car
};

// bus variables
float busXArray[2] = { -3.0f, 0.8f };  // initial x positions
float busYArray[2] = { -0.46f, -0.42f };  // y positions
float busSpeedArray[2] = { 0.005f, 0.005f };  // movement speeds
float busColors[2][3] = {
    {1.0f, 0.0f, 0.0f},  // red bus - RGB
    {1.0f, 1.0f, 0.0f}   // yellow bus
};

// screen boundary constants
const float CAR_RESET_MARGIN = 0.80f;
const float BUS_RESET_MARGIN = 1.00f;
float skyLimit = 0.2f;  // horizon line y position
GLfloat orthoLeft, orthoRight, orthoBottom, orthoTop;
bool isDay = true;

// sun and moon variables
float sunY = 0.6f;  // initial sun y position
float moonY = -1.5f;  // initial moon y position
float targetSunY = 0.6f;
float targetMoonY = -1.5f;
float sunMoonSpeed = 0.015f;
bool isTransitioning = false;
bool showMoon = false;

// star positions - x,y coordinates
float stars[40][2] = {
    {-1.8f,0.85f},{-1.5f,0.7f},{-1.2f,0.9f},{-0.9f,0.75f},{-0.6f,0.85f},{-0.3f,0.7f},
    {0.0f,0.9f},{0.3f,0.8f},{0.6f,0.85f},{0.9f,0.7f},{1.2f,0.9f},{1.5f,0.8f},
    {1.8f,0.85f},{-1.7f,0.6f},{-1.3f,0.55f},{-0.9f,0.6f},{-0.5f,0.5f},{-0.1f,0.6f},
    {0.3f,0.55f},{0.7f,0.6f},{1.1f,0.5f},{1.5f,0.6f},{-1.6f,0.4f},{-1.2f,0.45f},
    {-0.8f,0.4f},{-0.4f,0.45f},{0.0f,0.4f},{0.4f,0.45f},{0.8f,0.4f},{1.2f,0.45f},
    {1.6f,0.4f},{-1.4f,0.3f},{-0.7f,0.3f},{0.0f,0.3f},{0.7f,0.3f},{1.4f,0.3f}
};

// sound system variables
bool soundActive = false;
float blinkTimer = 0.0f;
float blinkSpeed = 0.1f;
float speakerPositions[4][2] = {
    {-0.9f, -0.3f},   // top left speaker - x,y
    {0.6f, -0.25f},   // top right speaker
    {-0.6f, -0.9f},   // bottom left speaker
    {0.9f, -0.9f}     // bottom right speaker
};

// speaker sign positions - x,y,size
float speakerSigns[4][3] = {
    {-0.95f, -0.25f, 0.08f},   // near top left speaker
    {0.65f, -0.2f, 0.08f},     // near top right speaker
    {-0.65f, -0.85f, 0.1f},    // near bottom left speaker
    {0.95f, -0.85f, 0.1f}      // near bottom right speaker
};

// vehicle horn variables
bool hornActive[5] = {false, false, false, false, false};  // 3 cars + 2 buses
float hornPulse[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float hornPulseSpeed = 0.06f;

// no sound board variables
float noSoundBoardX = 1.8f;      // board center x
float noSoundBoardY = -0.1f;     // board center y
float noSoundBoardSize = 0.14f;  // board scale
float poleBaseY = -0.38f;        // pole bottom y

// sound playing states
bool sound1Playing = false;
bool sound2Playing = false;
bool sound3Playing = false;
bool sound4Playing = false;

// sound alias names for mci
char soundAlias[4][20] = {
    "birdSound",
    "speakerSound",
    "busHorn",
    "carHorn"
};

// sound file paths in my system
const char* soundFiles[4] = {
    "C:\\Users\\siyam.DESKTOP-P8C0FU3\\OneDrive - American International University-Bangladesh\\All the course materials of 9th semester\\COMPUTER GRAPHICS [I]\\Graphics\\@ graphics final project\\SoundPollutionInCityVeiw\\sound_files\\1_speaker_sound.mp3",
    "C:\\Users\\siyam.DESKTOP-P8C0FU3\\OneDrive - American International University-Bangladesh\\All the course materials of 9th semester\\COMPUTER GRAPHICS [I]\\Graphics\\@ graphics final project\\SoundPollutionInCityVeiw\\sound_files\\2_speaker_sound.mp3",
    "C:\\Users\\siyam.DESKTOP-P8C0FU3\\OneDrive - American International University-Bangladesh\\All the course materials of 9th semester\\COMPUTER GRAPHICS [I]\\Graphics\\@ graphics final project\\SoundPollutionInCityVeiw\\sound_files\\3_bus_horn.mp3",
    "C:\\Users\\siyam.DESKTOP-P8C0FU3\\OneDrive - American International University-Bangladesh\\All the course materials of 9th semester\\COMPUTER GRAPHICS [I]\\Graphics\\@ graphics final project\\SoundPollutionInCityVeiw\\sound_files\\4_car_horn.mp3"
};

// individual horn states
bool individualHornActive[5] = {false, false, false, false, false};

// function to play sound
void playSound(int soundIndex, bool loop = false) {
    // check if index is valid
    if (soundIndex < 0 || soundIndex > 3) return;

    // closing any existing instance first
    char closeCmd[100];
    sprintf(closeCmd, "close %s", soundAlias[soundIndex]);
    mciSendString(closeCmd, NULL, 0, NULL);

    // create command to open and play the sound
    char command[512];

    // trying the absolute path first
    MCIERROR error;
    sprintf(command, "open \"%s\" alias %s", soundFiles[soundIndex], soundAlias[soundIndex]);
    error = mciSendString(command, NULL, 0, NULL);

    // if that fails then forward slashes
    if (error != 0) {
        char altPath[512];
        strcpy(altPath, soundFiles[soundIndex]);
        for (int i = 0; altPath[i]; i++) {
            if (altPath[i] == '\\') altPath[i] = '/';
        }
        sprintf(command, "open \"%s\" alias %s", altPath, soundAlias[soundIndex]);
        error = mciSendString(command, NULL, 0, NULL);
    }

    // if still fails, try relative path
    if (error != 0) {
        const char* simpleFiles[4] = {
            "sound_files/1_speaker_sound.mp3",
            "sound_files/2_speaker_sound.mp3",
            "sound_files/3_bus_horn.mp3",
            "sound_files/4_car_horn.mp3"
        };
        sprintf(command, "open \"%s\" alias %s", simpleFiles[soundIndex], soundAlias[soundIndex]);
        error = mciSendString(command, NULL, 0, NULL);
    }

    // showing error if all attempts failed
    if (error != 0) {
        char errorMsg[256];
        mciGetErrorString(error, errorMsg, 256);
        printf("error opening sound %d: %s\n", soundIndex + 1, errorMsg);
        return;
    }

    // plays the sound
    if (loop) {
        sprintf(command, "play %s repeat", soundAlias[soundIndex]);
    } else {
        sprintf(command, "play %s", soundAlias[soundIndex]);
    }

    mciSendString(command, NULL, 0, NULL);

    // updates which sound is playing
    switch(soundIndex) {
        case 0: sound1Playing = true; break;
        case 1: sound2Playing = true; break;
        case 2: sound3Playing = true; break;
        case 3: sound4Playing = true; break;
    }
}

// function to stop a specific sound
void stopSound(int soundIndex) {
    if (soundIndex < 0 || soundIndex > 3) return;

    char command[100];
    sprintf(command, "stop %s", soundAlias[soundIndex]);
    mciSendString(command, NULL, 0, NULL);

    sprintf(command, "close %s", soundAlias[soundIndex]);
    mciSendString(command, NULL, 0, NULL);

    // updates playing state
    switch(soundIndex) {
        case 0: sound1Playing = false; break;
        case 1: sound2Playing = false; break;
        case 2: sound3Playing = false; break;
        case 3: sound4Playing = false; break;
    }
}

// function to stop all sounds at once
void stopAllSounds() {
    for (int i = 0; i < 4; i++) {
        stopSound(i);
    }
}

// helper function to draw circles
void drawCircle(float x, float y, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float a = 2.0f * 3.1415926f * i / 100;
        glVertex2f(x + r * cosf(a), y + r * sinf(a));
    }
    glEnd();
}

// birds are initialized at their perching positions
void initializeBirds() {
    for (int i = 0; i < NUM_BIRDS; i++) {
        int perchIndex = i % 15;
        birdPositions[i][0] = perchPositions[perchIndex][0];
        birdPositions[i][1] = perchPositions[perchIndex][1];
        birdPositions[i][2] = 0.0f;
        birdPositions[i][3] = 0.0f;
    }
}

// bird is drawn in perched or flying state
void drawBird(float x, float y, float size, int state, float wingFlap) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    if (state == 0) {
        // perched bird body - dark gray circle at center
        glColor3f(0.2f, 0.2f, 0.2f);
        drawCircle(0.0f, 0.0f, 0.05f);

        // perched bird head - slightly lighter gray, offset to right
        glColor3f(0.3f, 0.3f, 0.3f);
        drawCircle(0.06f, 0.04f, 0.03f);

        // perched bird beak - orange triangle
        glColor3f(0.8f, 0.5f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.09f, 0.04f);
        glVertex2f(0.12f, 0.04f);
        glVertex2f(0.09f, 0.02f);
        glEnd();

        // perched bird eye - white with black pupil
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(0.07f, 0.05f, 0.008f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(0.07f, 0.05f, 0.004f);

        // perched bird tail - dark gray triangle
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.05f, 0.0f);
        glVertex2f(-0.1f, 0.03f);
        glVertex2f(-0.1f, -0.03f);
        glEnd();

        // perched bird feet - orange lines
        glColor3f(0.8f, 0.5f, 0.2f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex2f(-0.02f, -0.05f);
        glVertex2f(-0.02f, -0.07f);
        glVertex2f(0.02f, -0.05f);
        glVertex2f(0.02f, -0.07f);
        glEnd();
        glLineWidth(1.0f);

    } else {
        // flying bird body - smaller dark gray circle
        glColor3f(0.2f, 0.2f, 0.2f);
        drawCircle(0.0f, 0.0f, 0.04f);

        // flying bird head
        glColor3f(0.3f, 0.3f, 0.3f);
        drawCircle(0.04f, 0.02f, 0.025f);

        // flying bird beak
        glColor3f(0.8f, 0.5f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.06f, 0.02f);
        glVertex2f(0.08f, 0.02f);
        glVertex2f(0.06f, 0.01f);
        glEnd();

        // flying bird eye
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(0.045f, 0.025f, 0.006f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(0.045f, 0.025f, 0.003f);

        // wing animation based on wingFlap timer
        float wingAngle = sinf(wingFlap * 10.0f) * 0.5f;

        // right wing - rotated triangle
        glPushMatrix();
        glTranslatef(-0.02f, 0.0f, 0.0f);
        glRotatef(wingAngle * 30.0f, 0.0f, 0.0f, 1.0f);
        glColor3f(0.15f, 0.15f, 0.15f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.08f, 0.06f);
        glVertex2f(-0.08f, -0.06f);
        glEnd();
        glPopMatrix();

        // left wing - opposite rotation
        glPushMatrix();
        glTranslatef(-0.02f, 0.0f, 0.0f);
        glRotatef(-wingAngle * 30.0f, 0.0f, 0.0f, 1.0f);
        glColor3f(0.15f, 0.15f, 0.15f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(-0.08f, 0.06f);
        glVertex2f(-0.08f, -0.06f);
        glEnd();
        glPopMatrix();

        // flying bird tail - smaller triangle
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.04f, 0.0f);
        glVertex2f(-0.08f, 0.02f);
        glVertex2f(-0.08f, -0.02f);
        glEnd();
    }

    glPopMatrix();
}

// sun and moon positions are updated during transitions
void updateSunMoon() {
    if (isTransitioning) {
        // sun moves toward target position
        if (sunY < targetSunY - 0.01f) {
            sunY += sunMoonSpeed;
        } else if (sunY > targetSunY + 0.01f) {
            sunY -= sunMoonSpeed;
        } else {
            sunY = targetSunY;
        }

        // moon moves toward target position
        if (moonY < targetMoonY - 0.01f) {
            moonY += sunMoonSpeed;
        } else if (moonY > targetMoonY + 0.01f) {
            moonY -= sunMoonSpeed;
        } else {
            moonY = targetMoonY;
            showMoon = (targetMoonY == 0.6f);
        }

        // transition is complete when both reach targets
        if (fabs(sunY - targetSunY) < 0.01f && fabs(moonY - targetMoonY) < 0.01f) {
            isTransitioning = false;
        }
    }
}

// bird positions and states are updated each frame
void updateBirds() {
    // bird return timer counts down when sound is off
    if (!soundActive && birdReturnTimer > 0) {
        birdReturnTimer -= 0.016f;
        if (birdReturnTimer <= 0) {
            birdReturnTimer = 0;
            // birds start returning to perches
            for (int i = 0; i < NUM_BIRDS; i++) {
                if (birdPositions[i][2] == 1.0f) {
                    birdPositions[i][2] = 2.0f;
                    birdPositions[i][3] = 0.0f;
                }
            }
        }
    }

    for (int i = 0; i < NUM_BIRDS; i++) {
        float& x = birdPositions[i][0];
        float& y = birdPositions[i][1];
        float& state = birdPositions[i][2];
        float& timer = birdPositions[i][3];

        // wing flap animation is updated
        if (state > 0) {
            timer += 0.1f;
        }

        if (state == 1.0f) {
            // bird is flying away from sound
            float closestDist = 1000.0f;
            float dirX = 0.0f, dirY = 0.0f;

            // nearest speaker is found
            for (int s = 0; s < 4; s++) {
                float dx = x - speakerPositions[s][0];
                float dy = y - speakerPositions[s][1];
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist < closestDist) {
                    closestDist = dist;
                    dirX = dx / dist;
                    dirY = dy / dist;
                }
            }

            // bird moves away from speaker
            x += dirX * birdSpeed * 1.5f;
            y += dirY * birdSpeed * 1.5f + 0.01f;

            // screen edges are wrapped around
            if (x > orthoRight + 0.5f) x = orthoLeft - 0.5f;
            if (x < orthoLeft - 0.5f) x = orthoRight + 0.5f;
            if (y > orthoTop) y = orthoBottom;

        } else if (state == 2.0f) {
            // bird is returning to perch
            int perchIndex = i % 15;
            float targetX = perchPositions[perchIndex][0];
            float targetY = perchPositions[perchIndex][1];

            // direction to perch is calculated
            float dx = targetX - x;
            float dy = targetY - y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 0.01f) {
                // bird moves toward perch
                x += (dx / dist) * birdSpeed;
                y += (dy / dist) * birdSpeed;
            } else {
                // perch is reached
                x = targetX;
                y = targetY;
                state = 0.0f;
                timer = 0.0f;
            }
        } else if (state == 0.0f && !soundActive) {
            // perched bird has random slight movement
            timer += 0.01f;
            float moveX = sinf(timer * 0.5f + i) * 0.002f;
            float moveY = cosf(timer * 0.3f + i) * 0.001f;

            x += moveX;
            y += moveY;
        }
    }
}

// no sound signal board is drawn at top right
void drawNoSoundBoard() {
    glPushMatrix();
    glTranslatef(noSoundBoardX, noSoundBoardY, 0);

    // board background - white rectangle
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-noSoundBoardSize * 0.9f, -noSoundBoardSize * 0.7f);  // bottom left
    glVertex2f(-noSoundBoardSize * 0.9f, noSoundBoardSize * 0.7f);   // top left
    glVertex2f(noSoundBoardSize * 0.9f, noSoundBoardSize * 0.7f);    // top right
    glVertex2f(noSoundBoardSize * 0.9f, -noSoundBoardSize * 0.7f);   // bottom right
    glEnd();

    // board border - red outline
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-noSoundBoardSize * 0.9f, -noSoundBoardSize * 0.7f);
    glVertex2f(-noSoundBoardSize * 0.9f, noSoundBoardSize * 0.7f);
    glVertex2f(noSoundBoardSize * 0.9f, noSoundBoardSize * 0.7f);
    glVertex2f(noSoundBoardSize * 0.9f, -noSoundBoardSize * 0.7f);
    glEnd();

    // WIFI like sound symbol
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    float wifiScale = 0.4f;
    for (int i = 0; i < 3; i++) {
        float arcRadius = 0.1f * wifiScale + i * 0.025f * wifiScale;
        float startAngle = 45.0f * 3.14159f / 180.0f;
        float endAngle = 135.0f * 3.14159f / 180.0f;

        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(1.2f);

        glBegin(GL_LINE_STRIP);
        for (float angle = startAngle; angle <= endAngle; angle += 0.05f) {
            glVertex2f(cosf(angle) * arcRadius, sinf(angle) * arcRadius - 0.04f);
        }
        glEnd();
    }

    // center dot
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, -0.04f);
    glEnd();
    glPointSize(1.0f);

    glPopMatrix();

    // prohibition circle - red ring
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.5f);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 50; i++) {
        float angle = 2.0f * 3.14159f * i / 50.0f;
        float radius = 0.1f;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();

    // prohibition diagonal line - red crossbar
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.1f, -0.1f);
    glVertex2f(0.1f, 0.1f);
    glEnd();

    glLineWidth(1.0f);
    glPopMatrix();

    // pole - gray rectangle from board to ground
    glColor3f(0.3f, 0.3f, 0.3f);
    float poleTopY = noSoundBoardY - noSoundBoardSize * 0.7f;
    float poleHeight = poleTopY - poleBaseY;

    glRectf(noSoundBoardX - 0.007f, poleBaseY,    // bottom left
            noSoundBoardX + 0.007f, poleTopY);    // top right
}

// speaker is drawn at given position
void drawSpeaker(float x, float y, float size, bool isForeground) {
    // speaker body - dark gray rectangle
    glColor3f(0.3f, 0.3f, 0.3f);
    glRectf(x - 0.07f * size, y - 0.05f * size,   // bottom left
            x + 0.07f * size, y + 0.05f * size);  // top right

    // speaker grill - black/dark gray rectangle
    if (isDay) {
        glColor3f(0.1f, 0.1f, 0.1f);
    } else {
        glColor3f(0.2f, 0.2f, 0.2f);
    }
    glRectf(x - 0.06f * size, y - 0.04f * size,
            x + 0.06f * size, y + 0.04f * size);

    // speaker cones - light gray circles
    glColor3f(0.5f, 0.5f, 0.5f);
    drawCircle(x - 0.03f * size, y, 0.02f * size);  // left cone
    drawCircle(x + 0.03f * size, y, 0.02f * size);  // right cone

    // stand - medium gray rectangle below speaker
    glColor3f(0.4f, 0.4f, 0.4f);
    glRectf(x - 0.01f * size, y - 0.05f * size,
            x + 0.01f * size, y - 0.08f * size);
}

// speaker sound sign is drawn near each speaker
void drawSpeakerSoundSign(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // casing body - black rectangle
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.12f * size, -0.03f * size);  // bottom left
    glVertex2f(-0.12f * size, 0.03f * size);   // top left
    glVertex2f(0.12f * size, 0.03f * size);    // top right
    glVertex2f(0.12f * size, -0.03f * size);   // bottom right
    glEnd();

    // primer circles - light gray circles on left side
    glColor3f(0.8f, 0.8f, 0.8f);
    drawCircle(-0.08f * size, 0.0f, 0.015f * size);  // outer circle
    drawCircle(-0.08f * size, 0.0f, 0.008f * size);  // inner circle

    // sound lines are shown when sound is active
    if (soundActive) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 3; i++) {
            float offset = (i - 1) * 0.02f * size;
            glVertex2f(0.12f * size, offset);      // start at sign edge
            glVertex2f(0.3f * size, offset);       // extend to right
        }
        glEnd();
        glLineWidth(1.0f);
    }

    glPopMatrix();
}

// wifi sound symbol is drawn near speakers when sound is active
void drawWiFiSoundSymbol(float x, float y, float size, bool isRightSide, float blinkAlpha) {
    if (!soundActive) return;
    if (blinkAlpha < 0.5f) return;

    glPushMatrix();

    if (isRightSide) {
        glTranslatef(x + 0.15f * size, y, 0);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    } else {
        glTranslatef(x - 0.15f * size, y, 0);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    }

    // three concentric arcs are drawn
    float alpha = 0.5f + 0.5f * blinkAlpha;

    for (int i = 0; i < 3; i++) {
        float arcRadius = 0.05f * size + i * 0.03f * size;
        float startAngle = 45.0f * 3.14159f / 180.0f;
        float endAngle = 135.0f * 3.14159f / 180.0f;

        glColor3f(0.0f, 0.0f, 0.0f);
        if (i == 2) glLineWidth(1.8f);
        else if (i == 1) glLineWidth(1.5f);
        else glLineWidth(1.2f);

        glBegin(GL_LINE_STRIP);
        for (float angle = startAngle; angle <= endAngle; angle += 0.05f) {
            glVertex2f(cosf(angle) * arcRadius, sinf(angle) * arcRadius);
        }
        glEnd();
    }

    // center dot pulses with blinkAlpha
    float dotSize = 2.0f + blinkAlpha * 2.0f;
    glPointSize(dotSize);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPointSize(1.0f);

    glLineWidth(1.0f);
    glPopMatrix();
}

// sound waves are drawn around speakers when active
void drawSoundWaves() {
    if (!soundActive) return;

    float blinkAlpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14159f * 2.0f);

    for (int i = 0; i < 4; i++) {
        float speakerX = speakerPositions[i][0];
        float speakerY = speakerPositions[i][1];
        float symbolSize = (i < 2) ? 1.0f : 1.2f;  // bottom speakers are larger

        // wifi symbol is drawn on both sides of speaker
        drawWiFiSoundSymbol(speakerX, speakerY, symbolSize, true, blinkAlpha);
        drawWiFiSoundSymbol(speakerX, speakerY, symbolSize, false, blinkAlpha);
    }
}

// vehicle horn sign is drawn above vehicle when horn is active
void drawVehicleHornSign(int vehicleIndex, float x, float y, bool isBus) {
    if (!hornActive[vehicleIndex]) return;

    glPushMatrix();
    glTranslatef(x, y, 0);

    float hornSize = isBus ? 0.15f : 0.12f;  // bus signs are larger

    // triangle background - yellow warning triangle
    glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.2f, 0.2f);      // bottom left corner
    glVertex2f(0.1f, 0.35f);     // top corner
    glVertex2f(0.3f, 0.35f);     // bottom right corner
    glEnd();

    // exclamation mark - black vertical line
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0.2f, 0.25f);
    glVertex2f(0.2f, 0.31f);
    glEnd();

    // exclamation dot
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.2f, 0.23f);
    glEnd();
    glPointSize(1.0f);

    // sound lines pulse when horn is active
    if (hornActive[vehicleIndex]) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        glVertex2f(0.3f, 0.28f);
        glVertex2f(0.4f + hornPulse[vehicleIndex] * 0.1f, 0.28f);

        glVertex2f(0.3f, 0.32f);
        glVertex2f(0.4f + hornPulse[vehicleIndex] * 0.1f, 0.32f);
        glEnd();
        glLineWidth(1.0f);
    }

    glPopMatrix();
}

// sound waves and horn pulses are updated each frame
void updateSoundWaves() {
    // blink timer cycles for wifi symbol animation
    if (soundActive) {
        blinkTimer += blinkSpeed;
        if (blinkTimer > 1.0f) {
            blinkTimer = 0.0f;
        }
    }

    // horn pulses cycle for sound line animation
    for (int i = 0; i < 5; i++) {
        if (hornActive[i]) {
            hornPulse[i] += hornPulseSpeed;
            if (hornPulse[i] > 1.0f) {
                hornPulse[i] = 0.0f;
            }
        }
    }
}

// building is drawn at specified position and size
void drawBuilding(float x, float y, float w, float h) {
    // building base color changes with day/night
    if (isDay) glColor3f(0.35f, 0.35f, 0.4f);    // daytime blue-gray
    else glColor3f(0.1f, 0.1f, 0.15f);           // nighttime dark blue
    glRectf(x, y, x + w, y + h);

    // windows are drawn in grid pattern
    for (float i = 0.04f; i < w - 0.04f; i += 0.08f) {
        for (float j = 0.05f; j < h - 0.05f; j += 0.12f) {
            if (isDay) glColor3f(0.7f, 0.8f, 0.9f);   // daytime light blue
            else glColor3f(1.0f, 0.9f, 0.4f);         // nighttime yellow
            glRectf(x + i, y + j, x + i + 0.04f, y + j + 0.07f);
        }
    }
}

// tree is drawn at specified position
void drawTree(float x, float y, float s) {
    // trunk tree body - brown rectangle
    glColor3f(0.4f, 0.2f, 0.0f);
    glRectf(x - 0.02f * s, y, x + 0.02f * s, y + 0.15f * s);

    // leaves - green circles, darker at night
    if (isDay) glColor3f(0.0f, 0.5f, 0.0f);      // daytime green
    else glColor3f(0.0f, 0.2f, 0.0f);           // nighttime dark green

    // three overlapping circles for foliage
    drawCircle(x, y + 0.25f * s, 0.1f * s);      // top center
    drawCircle(x - 0.06f * s, y + 0.18f * s, 0.08f * s);  // left
    drawCircle(x + 0.06f * s, y + 0.18f * s, 0.08f * s);  // right
}

// street lamp is drawn at specified position
void drawStreetLamp(float x, float y, float poleH, bool flip) {
    // pole - dark gray rectangle
    glColor3f(0.15f, 0.15f, 0.15f);
    glRectf(x, y, x + 0.015f, y + poleH);

    // lamp housing - extends beyond pole
    glRectf(x - 0.04f, y + poleH, x + 0.055f, y + poleH + 0.02f);

    // light is only drawn at night
    if (!isDay) {
        // bulb - yellow circle
        glColor3f(1.0f, 1.0f, 0.7f);
        drawCircle(x + 0.007f, y + poleH, 0.015f);

        // light glow - semi-transparent yellow triangle
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 1.0f, 0.6f, 0.25f);
        glVertex2f(x + 0.007f, y + poleH);       // bulb center
        glVertex2f(x - 0.12f, y + (poleH * -0.2f)); // bottom left
        glVertex2f(x + 0.13f, y + (poleH * -0.2f)); // bottom right
        glEnd();
        glDisable(GL_BLEND);
    }
}

// car is drawn at specified position with given color
void drawCar(float x, float y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.7f, 0.7f, 1);  // cars are scaled down

    // car body - main color rectangle
    glColor3f(r, g, b);
    glRectf(-0.2f, 0, 0.2f, 0.1f);          // lower body
    glRectf(-0.12f, 0.1f, 0.12f, 0.18f);    // upper body

    // windows - color changes with day/night
    if (isDay) {
        glColor3f(0.7f, 0.9f, 1.0f);        // daytime light blue
    } else {
        glColor3f(0.15f, 0.2f, 0.3f);       // nighttime dark blue
    }

    float windowWidth = 0.22f;
    float windowHeight = 0.07f;
    float windowBottom = 0.105f;
    float windowTop = windowBottom + windowHeight;

    // left window
    glBegin(GL_POLYGON);
    glVertex2f(-0.11f, windowBottom);
    glVertex2f(-0.11f, windowTop);
    glVertex2f(-0.005f, windowTop);
    glVertex2f(-0.005f, windowBottom);
    glEnd();

    // right window
    glBegin(GL_POLYGON);
    glVertex2f(0.005f, windowBottom);
    glVertex2f(0.005f, windowTop);
    glVertex2f(0.11f, windowTop);
    glVertex2f(0.11f, windowBottom);
    glEnd();

    // window frames - black outlines
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(0.0f, windowBottom);  // center divider
    glVertex2f(0.0f, windowTop);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.11f, windowBottom);
    glVertex2f(-0.11f, windowTop);
    glVertex2f(0.11f, windowTop);
    glVertex2f(0.11f, windowBottom);
    glEnd();

    // wheels - black circles
    glColor3f(0.2f, 0.2f, 0.2f);
    drawCircle(-0.12f, 0, 0.03f);  // left wheel
    drawCircle(0.12f, 0, 0.03f);   // right wheel

    // wheel hubs - light gray centers
    glColor3f(0.5f, 0.5f, 0.5f);
    drawCircle(-0.12f, 0, 0.015f);
    drawCircle(0.12f, 0, 0.015f);

    // headlights - color changes with day/night
    if (isDay) {
        glColor3f(0.9f, 0.9f, 0.7f);   // daytime dim yellow
    } else {
        glColor3f(1.0f, 0.8f, 0.3f);   // nighttime bright yellow
    }

    drawCircle(-0.18f, 0.05f, 0.012f);  // left headlight
    drawCircle(0.18f, 0.05f, 0.012f);   // right headlight

    // taillights - red, brighter at night
    if (isDay) {
        glColor3f(0.8f, 0.2f, 0.2f);   // daytime dark red
    } else {
        glColor3f(1.0f, 0.1f, 0.1f);   // nighttime bright red
    }
    drawCircle(-0.18f, 0.02f, 0.01f);  // left taillight
    drawCircle(0.18f, 0.02f, 0.01f);   // right taillight

    glLineWidth(1.0f);
    glPopMatrix();
}

// bus is drawn at specified position with given color
void drawBus(float x, float y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(1.3f, 1.3f, 1);  // buses are scaled up

    // bus body - main color rectangle
    glColor3f(r, g, b);
    glRectf(-0.35f, 0, 0.35f, 0.15f);

    // windows - color changes with day/night
    if (isDay) {
        glColor3f(0.7f, 0.9f, 1.0f);        // daytime light blue
    } else {
        glColor3f(0.15f, 0.2f, 0.3f);       // nighttime dark blue
    }

    float windowHeight = 0.06f;
    float windowBottom = 0.09f;
    float windowTop = windowBottom + windowHeight;

    // four separate windows across bus
    glBegin(GL_POLYGON);
    glVertex2f(-0.32f, windowBottom);  // window 1 left
    glVertex2f(-0.32f, windowTop);
    glVertex2f(-0.17f, windowTop);
    glVertex2f(-0.17f, windowBottom);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-0.15f, windowBottom);  // window 2 left
    glVertex2f(-0.15f, windowTop);
    glVertex2f(0.0f, windowTop);
    glVertex2f(0.0f, windowBottom);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.02f, windowBottom);   // window 3 left
    glVertex2f(0.02f, windowTop);
    glVertex2f(0.17f, windowTop);
    glVertex2f(0.17f, windowBottom);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.19f, windowBottom);   // window 4 left
    glVertex2f(0.19f, windowTop);
    glVertex2f(0.34f, windowTop);
    glVertex2f(0.34f, windowBottom);
    glEnd();

    // window frames - black outlines
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2f(-0.16f, windowBottom);  // between window 1-2
    glVertex2f(-0.16f, windowTop);
    glVertex2f(0.01f, windowBottom);   // between window 2-3
    glVertex2f(0.01f, windowTop);
    glVertex2f(0.18f, windowBottom);   // between window 3-4
    glVertex2f(0.18f, windowTop);
    glEnd();

    // window outlines
    for (int i = 0; i < 4; i++) {
        float left = -0.32f + i * 0.17f;
        float right = left + 0.15f;

        glBegin(GL_LINE_LOOP);
        glVertex2f(left, windowBottom);
        glVertex2f(left, windowTop);
        glVertex2f(right, windowTop);
        glVertex2f(right, windowBottom);
        glEnd();
    }

    // wheels - black circles
    glColor3f(0.2f, 0.2f, 0.2f);
    drawCircle(-0.22f, 0, 0.035f);  // left wheel
    drawCircle(0.22f, 0, 0.035f);   // right wheel

    // wheel hubs - light gray centers
    glColor3f(0.5f, 0.5f, 0.5f);
    drawCircle(-0.22f, 0, 0.017f);
    drawCircle(0.22f, 0, 0.017f);

    // headlights - color changes with day/night
    if (isDay) {
        glColor3f(0.9f, 0.9f, 0.7f);   // daytime dim yellow
    } else {
        glColor3f(1.0f, 0.8f, 0.3f);   // nighttime bright yellow
    }

    drawCircle(-0.32f, 0.04f, 0.015f);  // left headlight
    drawCircle(0.32f, 0.04f, 0.015f);   // right headlight

    // taillights - red, brighter at night
    if (isDay) {
        glColor3f(0.8f, 0.2f, 0.2f);   // daytime dark red
    } else {
        glColor3f(1.0f, 0.1f, 0.1f);   // nighttime bright red
    }
    drawCircle(-0.32f, 0.02f, 0.012f);  // left taillight
    drawCircle(0.32f, 0.02f, 0.012f);   // right taillight

    glLineWidth(1.0f);
    glPopMatrix();
}

// day/night transition is started
void startDayNightTransition(bool toDay) {
    if (isTransitioning) return;

    isTransitioning = true;

    if (toDay) {
        // transition to day: sun rises, moon sets
        targetSunY = 0.6f;
        targetMoonY = -1.5f;
        showMoon = false;
        isDay = true;
    } else {
        // transition to night: sun sets, moon rises
        targetSunY = -1.5f;
        targetMoonY = 0.6f;
        showMoon = true;
        isDay = false;
    }
}

// main display function - everything is drawn here
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // sky - top quad from horizon to top of screen
    glBegin(GL_QUADS);
    if (isDay) glColor3f(0.5f, 0.8f, 1.0f); else glColor3f(0.05f, 0.05f, 0.15f);
    glVertex2f(orthoLeft, skyLimit);      // bottom left
    glVertex2f(orthoRight, skyLimit);     // bottom right
    glVertex2f(orthoRight, orthoTop);     // top right
    glVertex2f(orthoLeft, orthoTop);      // top left
    glEnd();

    // celestial bodies
    if (isDay) {
        // sun is drawn at current position
        glColor3f(1, 1, 0);  // yellow
        drawCircle(-0.3f, sunY, 0.18f);

        // white clouds are drawn at four positions
        for (int i = 0; i < 4; i++) {
            glColor3f(1,1,1);
            drawCircle(cloudPosition[i] - 0.09f, 0.75f, 0.11f);  // left cloud part
            drawCircle(cloudPosition[i], 0.75f, 0.15f);          // center cloud part
            drawCircle(cloudPosition[i] + 0.09f, 0.75f, 0.11f);  // right cloud part
        }
    } else {
        // sun is drawn if still visible during transition
        if (sunY > -1.0f) {
            glColor3f(1, 1, 0);
            drawCircle(-0.3f, sunY, 0.18f);
        }

        // moon is drawn if showing
        if (showMoon && moonY > -1.0f) {
            glColor3f(1, 1, 1);  // white moon
            drawCircle(-0.3f, moonY, 0.15f);

            // moon crater - dark circle over white
            glColor3f(0.05f, 0.05f, 0.15f);
            drawCircle(-0.25f, moonY + 0.02f, 0.15f);
        }

        // stars are drawn at night above horizon
        if (!isDay && showMoon) {
            glColor3f(1, 1, 1);
            for (int i = 0; i < 40; i++) {
                if (stars[i][1] > skyLimit) {
                    drawCircle(stars[i][0], stars[i][1], 0.008f);
                }
            }
        }

        // ash colored clouds at night
        for (int i = 0; i < 4; i++) {
            glColor3f(0.5f, 0.5f, 0.55f);
            drawCircle(cloudPosition[i] - 0.09f, 0.75f, 0.11f);
            drawCircle(cloudPosition[i], 0.75f, 0.15f);
            drawCircle(cloudPosition[i] + 0.09f, 0.75f, 0.11f);
        }
    }

    // background grass - middle section
    if (isDay) glColor3f(0.4f, 0.8f, 0.4f); else glColor3f(0.12f, 0.25f, 0.12f);
    glRectf(orthoLeft, -0.4f, orthoRight, skyLimit);

    // background items - trees, buildings, street lamps
    drawTree(-1.7f, -0.4f, 1.2f);
    drawBuilding(-1.5f, -0.25f, 0.35f, 0.5f);
    drawTree(-1.0f, -0.2f, 1.0f);
    drawBuilding(-0.9f, -0.4f, 0.45f, 0.8f);
    drawTree(-0.25f, -0.4f, 1.1f);
    drawBuilding(0.1f, -0.2f, 0.35f, 0.65f);
    drawTree(0.6f, -0.125f, 0.9f);
    drawBuilding(0.9f, -0.3f, 0.45f, 0.75f);
    drawTree(1.72f, -0.35f, 1.3f);

    drawStreetLamp(-1.1f, -0.45f, 0.35f, false);
    drawStreetLamp(0.55f, -0.45f, 0.35f, false);
    drawStreetLamp(1.5f, -0.45f, 0.35f, false);

    // road - middle dark band
    if (isDay) glColor3f(0.2f, 0.2f, 0.2f); else glColor3f(0.08f, 0.08f, 0.08f);
    glRectf(orthoLeft, -0.8f, orthoRight, -0.4f);

    // foreground grass - bottom section
    if (isDay) glColor3f(0.2f, 0.6f, 0.2f); else glColor3f(0.08f, 0.2f, 0.08f);
    glRectf(orthoLeft, orthoBottom, orthoRight, -0.8f);

    // foreground buildings - partial buildings at bottom
    drawBuilding(-1.4f, -1.0f, 0.37f, 0.15f);
    drawBuilding(-0.1f, -1.0f, 0.42f, 0.185f);
    drawBuilding(1.2f, -1.0f, 0.3f, 0.2f);

    drawStreetLamp(-0.6f, -0.8f, 0.35f, false);
    drawStreetLamp(0.9f, -0.8f, 0.35f, false);

    // sound speakers are drawn at four positions
    drawSpeaker(speakerPositions[0][0], speakerPositions[0][1], 1.0f, false);  // top left
    drawSpeaker(speakerPositions[1][0], speakerPositions[1][1], 1.0f, false);  // top right
    drawSpeaker(speakerPositions[2][0], speakerPositions[2][1], 1.2f, true);   // bottom left (larger)
    drawSpeaker(speakerPositions[3][0], speakerPositions[3][1], 1.2f, true);   // bottom right (larger)

    // sound waves are drawn when speakers are active
    drawSoundWaves();

    // sound signs are drawn near each speaker
    for (int i = 0; i < 4; i++) {
        drawSpeakerSoundSign(speakerSigns[i][0], speakerSigns[i][1], speakerSigns[i][2]);
    }

    // zebra borders - yellow/black stripes at road edges
    float seg = 0.15f;
    for (float x = orthoLeft; x < orthoRight; x += seg * 2) {
        glColor3f(1, 1, 0);  // yellow stripe
        glRectf(x, -0.81f, x + seg, -0.79f);      // bottom road edge
        glRectf(x, -0.41f, x + seg, -0.39f);      // top road edge

        glColor3f(0, 0, 0);  // black gap
        glRectf(x + seg, -0.81f, x + (seg * 2), -0.79f);
        glRectf(x + seg, -0.41f, x + (seg * 2), -0.39f);
    }

    // no sound board is drawn at top right
    drawNoSoundBoard();

    // birds are drawn only during day
    if (isDay) {
        for (int i = 0; i < NUM_BIRDS; i++) {
            float birdSize = 0.08f + (i % 3) * 0.02f;  // varied sizes
            drawBird(birdPositions[i][0], birdPositions[i][1], birdSize,
                     (int)birdPositions[i][2], birdPositions[i][3]);
        }
    }

    // cars are drawn with horn signs if active
    for (int i = 0; i < 3; i++) {
        drawCar(carX[i], carY[i], carColors[i][0], carColors[i][1], carColors[i][2]);
        drawVehicleHornSign(i, carX[i], carY[i], false);
    }

    // buses are drawn with horn signs if active
    for (int i = 0; i < 2; i++) {
        drawBus(busXArray[i], busYArray[i],
                busColors[i][0], busColors[i][1], busColors[i][2]);
        drawVehicleHornSign(i + 3, busXArray[i], busYArray[i], true);
    }

    glutSwapBuffers();
}

// update function is called for animation
void update(int value) {
    // clouds move across screen
    for (int i = 0; i < 4; i++) {
        cloudPosition[i] += cloudSpeed;
        if (cloudPosition[i] > orthoRight + 0.5f) cloudPosition[i] = orthoLeft - 0.5f;
    }

    // cars move right, reset when off screen
    for (int i = 0; i < 3; i++) {
        carX[i] += carSpeed[i];
        if (carX[i] > orthoRight + CAR_RESET_MARGIN) carX[i] = orthoLeft - CAR_RESET_MARGIN;
    }

    // buses move right, reset when off screen
    for (int i = 0; i < 2; i++) {
        busXArray[i] += busSpeedArray[i];
        if (busXArray[i] > orthoRight + BUS_RESET_MARGIN) busXArray[i] = orthoLeft - BUS_RESET_MARGIN;
    }

    // celestial bodies are updated
    updateSunMoon();

    // bird positions are updated
    updateBirds();

    // sound effects are updated
    updateSoundWaves();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // ~60fps
}

// keyboard input is handled here
void keyboard(unsigned char key, int x, int y) {
    if (key == 'd' || key == 'D') {
        // day is switched to
        startDayNightTransition(true);
        initializeBirds();

        // bird sound is played during day
        if (isDay) {
            playSound(0, true);  // bird sound looped
        } else {
            stopSound(0);  // bird sound stopped
        }
    }
    if (key == 'n' || key == 'N') {
        // night is switched to
        startDayNightTransition(false);
        stopSound(0);  // bird sound is stopped at night
    }
    if (key == 's' || key == 'S') {
        // sound is toggled
        soundActive = !soundActive;
        blinkTimer = 0.0f;

        if (soundActive) {
            // speaker sound is played
            playSound(1);

            if (isDay) {
                // birds fly away when sound starts
                for (int i = 0; i < NUM_BIRDS; i++) {
                    if (birdPositions[i][2] == 0.0f) {
                        birdPositions[i][2] = 1.0f;
                        birdPositions[i][3] = 0.0f;
                    }
                }
                birdReturnTimer = BIRD_RETURN_TIME;
            }
        } else {
            // speaker sound is stopped
            stopSound(1);

            if (isDay) {
                // return timer is started when sound stops
                birdReturnTimer = BIRD_RETURN_TIME;
            }
        }
    }

    // individual horn controls
    if (key == '1') {
        individualHornActive[0] = !individualHornActive[0];
        if (individualHornActive[0]) {
            hornActive[0] = true;
            hornPulse[0] = 0.0f;
            playSound(3);  // car horn for car 1
        } else {
            hornActive[0] = false;
            stopSound(3);
        }
    }
    if (key == '2') {
        individualHornActive[1] = !individualHornActive[1];
        if (individualHornActive[1]) {
            hornActive[1] = true;
            hornPulse[1] = 0.0f;
            playSound(3);  // car horn for car 2
        } else {
            hornActive[1] = false;
            stopSound(3);
        }
    }
    if (key == '3') {
        individualHornActive[2] = !individualHornActive[2];
        if (individualHornActive[2]) {
            hornActive[2] = true;
            hornPulse[2] = 0.0f;
            playSound(3);  // car horn for car 3
        } else {
            hornActive[2] = false;
            stopSound(3);
        }
    }
    if (key == '4') {
        individualHornActive[3] = !individualHornActive[3];
        if (individualHornActive[3]) {
            hornActive[3] = true;
            hornPulse[3] = 0.0f;
            playSound(2);  // bus horn for bus 1
        } else {
            hornActive[3] = false;
            stopSound(2);
        }
    }
    if (key == '5') {
        individualHornActive[4] = !individualHornActive[4];
        if (individualHornActive[4]) {
            hornActive[4] = true;
            hornPulse[4] = 0.0f;
            playSound(2);  // bus horn for bus 2
        } else {
            hornActive[4] = false;
            stopSound(2);
        }
    }

    // all horns are toggled with H
    if (key == 'h' || key == 'H') {
        bool anyActive = false;
        for (int i = 0; i < 5; i++) {
            if (hornActive[i]) anyActive = true;
        }

        if (anyActive) {
            // all horns are turned off
            for (int i = 0; i < 5; i++) {
                hornActive[i] = false;
                individualHornActive[i] = false;
            }
            stopSound(2);  // bus horns stopped
            stopSound(3);  // car horns stopped
        } else {
            // all horns are turned on
            for (int i = 0; i < 5; i++) {
                hornActive[i] = true;
                individualHornActive[i] = true;
                hornPulse[i] = 0.0f;
            }
            // both sounds are played together
            playSound(2);  // bus horns
            playSound(3);  // car horns
        }
    }

    // birds are reset to perches
    if (key == 'b' || key == 'B') {
        initializeBirds();
        // bird sound is restarted if it was playing
        if (isDay && sound1Playing) {
            stopSound(0);
            playSound(0, true);
        }
    }

    // escape key exits program
    if (key == 27) {
        stopAllSounds();
        exit(0);
    }
}

// window is reshaped when resized
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // orthographic projection is adjusted for aspect ratio
    if (w >= h) {
        orthoLeft = -aspect;
        orthoRight = aspect;
        orthoBottom = -1.0f;
        orthoTop = 1.0f;
    } else {
        orthoLeft = -1.0f;
        orthoRight = 1.0f;
        orthoBottom = -1.0f / aspect;
        orthoTop = 1.0f / aspect;
    }

    gluOrtho2D(orthoLeft, orthoRight, orthoBottom, orthoTop);
    glMatrixMode(GL_MODELVIEW);
}

// main function - program starts here
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Sound Pollution City View");

    // birds are initialized at perches
    initializeBirds();

    // controls are printed to console
    printf(" =========================================\n");
    printf("        Sound Pollution City View \n");
    printf(" =========================================\n\n");
    printf(" controls:\n");
    printf(" d or D - switch to day (with bird sounds)\n");
    printf(" n or N - switch to night (stops bird sounds)\n");
    printf(" s or S - toggle speaker sound on/off\n");
    printf(" h or H - toggle all vehicle horns on/off\n");
    printf(" 1 - toggle car 1 horn (plays car horn)\n");
    printf(" 2 - toggle car 2 horn (plays car horn)\n");
    printf(" 3 - toggle car 3 horn (plays car horn)\n");
    printf(" 4 - toggle bus 1 horn (plays bus horn)\n");
    printf(" 5 - toggle bus 2 horn (plays bus horn)\n");
    printf(" b - reset birds to perches\n");
    printf(" esc - exit program\n\n");
    printf(" sound effects:\n");
    printf(" sound 1: bird sounds during day\n");
    printf(" sound 2: speaker sound when turned on\n");
    printf(" sound 3: bus horns for buses\n");
    printf(" sound 4: car horns for cars\n");
    printf(" =============================================================\n\n");

    // opengl functions are registered
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    // bird sound is started initially (daytime)
    playSound(0, true);

    // main loop is entered
    glutMainLoop();

    // sounds are cleaned up on exit
    stopAllSounds();
    return 0;
}
