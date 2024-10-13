/*********
   CTIS164 - Template Source Program
----------
STUDENT : Leyli Shadurdyyeva
SECTION : 2
HOMEWORK:2
----------
PROBLEMS:none
----------
ADDITIONAL FEATURES: rocks remove one point, made gradient for planets, used ovals to create rings
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
int dy = 0;
bool laser = false;
int moving = 0;
int rockx[100], rocky[100], rockr[100];
int movement = 0, rockn = 0;
int yp[5] = { -600,-520,-430,-300,-200 };
int r[5] = { 45,30,40,50,50 };
int x2;
int time_passed = 0;
int game_on = 1, points = 0;
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, pause = false;
int  winWidth, winHeight; // current Window width and height


int yrocket = -240;

void DrawEllipse(float radiusX, float radiusY, int x, int y)
{
    int i;

    glBegin(GL_LINE_LOOP);

    for (i = 0; i < 360; i++)
    {
        float rad = i * D2R;
        glVertex2f(cos(rad) * radiusX + x,
            sin(rad) * radiusY + y);
    }

    glEnd();
}
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}
void create_rock(int &n) {
    rockx[n] = rand() % 100 + 500;
    rocky[n] = rand() % 700 - 350;
    rockr[n++] = rand() % 50;
}
// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void displayboard(){
    glColor3ub(255, 255, 255);
    glRectf(-500, 350, -400, -350);
    glColor3ub(0, 0, 0);
    vprint2(-500, 300, 0.15, "TIME LEFT");
    if(game_on == false) vprint2(-500, 250, 0.15, "00:00");
    else vprint2(-500, 250, 0.15, " %d:%0.2d", 19 - time_passed * 16 / 1000, 1000 - time_passed * 16  % 1000);
    vprint2(-500, 150, 0.2, "POINTS");
    vprint2(-470, 100, 0.2, "%d", points);
    glColor3ub(1, 1, 1);
    glRectf(-500, -10, -400, 10);
    glColor3ub(0, 0, 0);
    vprint2(-500, -50, 0.2, "F1 key"); 
    vprint2(-500, -100, 0.2, "Pause/"); 
    vprint2(-500, -130, 0.2, "Restart"); 

    vprint2(-500, -250, 0.15, "<Spacebar>");
    vprint2(-500, -290, 0.2, "SHOOTS");
}

void drawDeathStar(int x, int radius)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(x, dy);
    glColor3ub(110, 110, 110);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + radius * cos(angle), dy + radius * sin(angle));
    }
    glEnd();
    glColor3ub(0, 0 , 0);
    circle(x + 20, dy + 20, radius / 5);
    glColor3ub(1, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(x - radius, dy);
    glVertex2f(x + radius, dy);
    glEnd();
}

void drawLaser(int x, int radius)
{
    glColor3ub(0, 0, 0);
    circle(x + 20, dy + 20, radius / 5);
    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    glVertex2f(x + 20, dy + 35);
    glVertex2f(x + 20, dy + 20);
    glEnd();
    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    glVertex2f(x + 8, dy + 10);
    glVertex2f(x + 20, dy + 20);
    glEnd();
    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    glVertex2f(x + 32, dy + 10);
    glVertex2f(x + 20, dy + 20);
    glEnd();

    int x1 = x+20, y1 = dy+16, y2 = dy+24, y3 = dy+18, y4 = dy+22;
    x2 = x1 + moving;
    glColor3f(0, 1, 0);
    glRectf(x1, y1, x2, y2);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0, 1, 0);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glColor3f(1, 1, 1);
    glVertex2f(x1, y3);
    glVertex2f(x2, y3);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0, 1, 0);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glColor3f(1, 1, 1);
    glVertex2f(x1, y4);
    glVertex2f(x2, y4);
    glEnd();
    
}

void drawSaturn(int xp, int radius)
{
    
    float angle;
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0);
    glVertex2f(xp, yp[0]);
    glColor3ub(189, 70, 24);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(xp + radius * cos(angle), yp[0] + radius * sin(angle));
    }
    glEnd();

    glColor3ub(189, 70, 24);
    int i;

    glBegin(GL_LINE_STRIP);

    for (i = 110; i < 430; i++)
    {
        float rad = i * D2R;
        glVertex2f(cos(rad) * 100 + xp,
            sin(rad) * 30 + yp[0]);
    }

    glEnd();
    
}
void drawNeptune(int xp, int radius)
{
   
    float angle;
    glBegin(GL_POLYGON);
    glColor3ub(70, 128, 245);
    glVertex2f(xp, yp[1] );
    glColor3ub(27, 27, 245);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(xp + radius * cos(angle), yp[1] + radius * sin(angle));
    }
    glEnd();

    
}
void drawrock(int x, int y, int r) {
    glColor3ub(150, 75, 0);
    circle(x, y, r);
}
void drawMars(int x, int radius)
{
    glColor3ub(250, 250, 250);
    DrawEllipse(70, 50, x, yp[2]);



    float angle;

    glBegin(GL_POLYGON);
    glColor3ub(250, 0, 0);
    glVertex2f(x, yp[2]);
    glColor3ub(245, 210, 54);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + 40 * cos(angle), yp[2] + 40 * sin(angle));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0);
    glVertex2f(x, yp[2]);
    glColor3ub(231, 127, 24);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + 25 * cos(angle), yp[2] + 25 * sin(angle));
    }
    glEnd();
}
void drawEarth(int x, int radius)
{
    glColor3ub(55, 180, 80);
    circle(x, yp[3], radius);

    glColor3ub(76, 175, 228);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, yp[3]+radius);
    glVertex2f(x - radius/2 , yp[3]);
    glVertex2f(x + radius/2 , yp[3]);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x + radius / 2, yp[3]);
    glVertex2f(x - radius / 2, yp[3]);
    glVertex2f(x, yp[3] - radius);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x, yp[3] + radius / 2 );
    glVertex2f(x,yp[3] - radius / 2 );
    glVertex2f( x - radius,yp[3]);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x, yp[3] - radius / 2);
    glVertex2f(x, yp[3] + radius / 2);
    glVertex2f(x + radius, yp[3] );
    glEnd();
    
}
void drawSun(int xp, int radius)
{
    
    float angle;
    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(xp, yp[4]);
    glColor3ub(245, 242, 27);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(xp + radius * cos(angle), yp[4] + radius * sin(angle));
    }
    glEnd();

   
}

int posx1;
int posx2;
int posx3;
int posx4;
int posx5;
void drawblackhole(int x , int y)
{
    glColor3ub(250, 250, 250);
    DrawEllipse(140, 50, x, y);
    


    float angle;

    glBegin(GL_POLYGON);
    glColor3ub(250, 0, 0);
    glVertex2f(x, y);
    glColor3ub(245, 210, 54);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + 40 * cos(angle), y + 40 * sin(angle));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 0);
    glVertex2f(x, y);
    glColor3ub(231, 127, 24);
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + 25 * cos(angle), y + 25 * sin(angle));
    }
    glEnd();

}

void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    glPointSize(2.0); // set the size of the stars
    glColor3f(1.0, 1.0, 1.0); // set the color of the stars to white
    glBegin(GL_POINTS);
    //srand(1234);
    for (int i = 0; i < 50; i++)
    {
        glVertex2f(rand() % -500, rand() % 350);
        glVertex2f(rand() % 500, -rand() % 350);
        glVertex2f(-rand() % 500, rand() % 350);
        glVertex2f(-rand() % 500, -rand() % 350);
    }
    glEnd();

    for (int i = 0; i < 100; i++) {
        drawrock(rockx[i], rocky[i], rockr[i]);
    }

    drawDeathStar(-300, 80);
   
    drawSaturn(posx1,r[0]);
    drawMars(posx2, r[1]);
    drawEarth(posx3, r[2]);
    drawSun(posx4, r[3]);
    drawNeptune(posx5, r[4]);
    if (laser == true)
        drawLaser(-300, 80);

    glColor3ub(255, 255, 255);
    vprint(-340, 330, GLUT_BITMAP_8_BY_13, "Leyli Shadurdyyeva");
    vprint(-340, 310, GLUT_BITMAP_8_BY_13, "22201183");
    vprint(-70, 330, GLUT_BITMAP_9_BY_15, " - Homework -");
    glColor3ub(255, 0, 0);
    vprint(-50, 300, GLUT_BITMAP_HELVETICA_18, "Star Wars");
    displayboard();
    if (game_on == false) {
        glLineWidth(5);
        glColor3f(1, 0, 0);
        vprint2(-300, 150, 1, "MAY THE");
        vprint2(-300, 0, 1, "FORCE BE");
        vprint2(-300, -150, 1, "WITH YOU");
    }
        
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ') {
        if (pause == false) {
            laser = true;
            moving++;
        }
    }
    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ') {
        if (pause == false) {
            laser = false;
            moving = 0;
        }
        
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1: 
        if (game_on == false) { game_on = true; time_passed = 0; points = 0; }
        else pause = !pause;

        break;
    }
    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (pause == false) {
        if (points < 0)points = 0;
        if (game_on == true) {
            time_passed++;
            if (time_passed % 200 == 0)create_rock(rockn);
            for (int i = 0; i < 100; i++) {
                rockx[i]-=2;
                if (sqrt((rockx[i] + 300) * (rockx[i] + 300) + (rocky[i] - dy - 20) * (rocky[i] - dy - 20)) < 100) {
                    points--;
                    rockx[i] = -1251;
                }
            }
            if (time_passed * 16 / 1000 == 20)game_on = 0;
            if (up == true) {
                dy += 1;
                if (dy > 267)dy = 267;
            }
            if (down == true) {
                if (dy < -267)dy = -267;
                dy -= 1;
            }
            if (laser == true)
                moving += 5;

            if (yp[0] >= 400)
                yp[0] = -400;
            if (yp[1] >= 400)
                yp[1] = -400;
            if (yp[2] >= 400)
                yp[2] = -400;
            if (yp[3] >= 400)
                yp[3] = -400;
            if (yp[4] >= 400)
                yp[4] = -400;

            if (TIMER_ON == 1) {
                yp[0]++;
                yp[1]++;
                yp[2]++;
                yp[3]++;
                yp[4]++;
                yrocket++;
            }

            printf("%d \n", yp[0]);
            if (yrocket >= 550)
                yrocket = -290;
            for (int i = 0; i < 5; i++)
            {
                if (abs(dy + 20 - yp[i]) < r[i]) {
                    if (moving - 280 > posx1 - 30)
                        if (i == 0) { posx1 = rand() % 530 - 100; yp[i] = -500; points++; }
                    if (moving - 280 > posx2 - 30)
                        if (i == 1) { posx2 = rand() % 530 - 100; yp[i] = -520; points++; }
                    if (moving - 280 > posx3 - 40)
                        if (i == 2) { posx3 = rand() % 530 - 100; yp[i] = -530; points++; }
                    if (moving - 280 > posx4 - 50)
                        if (i == 3) { posx4 = rand() % 530 - 100; yp[i] = -400; points++; }
                    if (moving - 280 > posx5 - 50)
                        if (i == 4) { posx5 = rand() % 530 - 100; yp[i] = -500; points++; }
                }

            }
        }
    }
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    srand(time(NULL));
    posx1 = rand() % 530 - 100;
    posx2 = rand() % 530 - 100;
    posx3 = rand() % 530 - 100;
    posx4 = rand() % 530 - 100;
    posx4 = rand() % 530 - 100;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("LEYLI STAR WARS GAME");
    
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}