/*********
   CTIS164 - Template Source Program
----------
STUDENT : Leyli Shadurdyyeva
SECTION : 2
HOMEWORK: Homework 3
----------
PROBLEMS: none
----------
ADDITIONAL FEATURES: changes color of the bird with each click, when the bird catches the pig, a burst appears for a certain amount of time and then the game starts over
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
#include "vec.h"
bool burst = false;
float mouseClicked = false;
bool birdexist = true;
int birdradius = 30;
bool animation = 1;
bool pig = 1;
double xB = -350, yB = -150; //BALLOON's initial position
int radiusB = 45; //BALLOON's initial radius
int r = 100, g = 172, b = 50; // initial color
int Vx = 30*16/70; 
float time = 0;
float c, h, k;

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define velocity 2
#define D2R 0.0174532
GLfloat colors[][3] = {
    {1.0, 0.0, 0.0}, // Red
    {0.0, 1.0, 0.0}, // Green
    {0.0, 0.0, 1.0}, // Blue
};
int colorIndex = 0;

typedef struct {
    vec_t pos;
    vec_t vel;
    double angle;
}bird_t;

bird_t bird = { 0,0,0};

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//


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
void halfcircle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 50; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void oval(int x, int y, int rx, int ry)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + rx * cos(angle), y + ry * sin(angle));
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
void drawBurst(int x , int y)
{
    glColor3f(1, 1, 0);
    oval(x+4, y-2, 40, 35);
    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x-40, y+17);
    glVertex2f(x+30, y+25);
    glVertex2f(x, y+70);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 12, y + 17);
    glVertex2f(x -20, y-20);
    glVertex2f(x-90, y+30);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 20, y);
    glVertex2f(x - 30, y - 30);
    glVertex2f(x - 90, y - 30);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x, y-20);
    glVertex2f(x - 40, y - 20);
    glVertex2f(x - 50, y - 85);
    glEnd();
   
    glBegin(GL_TRIANGLES);
    glVertex2f(x+30, y + 20);
    glVertex2f(x + 45, y + 8);
    glVertex2f(x + 90, y +25);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - 20);
    glVertex2f(x + 40, y - 20);
    glVertex2f(x +50, y - 85);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x +40, y - 20);
    glVertex2f(x + 36, y +10);
    glVertex2f(x + 90, y );
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 40, y - 20);
    glVertex2f(x + 36, y + 10);
    glVertex2f(x + 90, y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(x -10, y - 20);
    glVertex2f(x + 10, y -20);
    glVertex2f(x, y -80);
    glEnd();
   
    glColor3f(0, 0, 0);
    print(x-16, y-7, "POP", GLUT_BITMAP_TIMES_ROMAN_24);

}
void drawBird()
{
    vec_t point1 = addV(bird.pos, pol2rec({ 31, bird.angle + 130 }));
    vec_t point2 = addV(bird.pos, pol2rec({ 20, bird.angle + 240}));
    vec_t point3 = addV(bird.pos, pol2rec({ 70, bird.angle + 225 }));

    vec_t point4 = addV(bird.pos, pol2rec({ 31, bird.angle + 53 }));
    vec_t point5 = addV(bird.pos, pol2rec({ 20, bird.angle + 310 }));
    vec_t point6 = addV(bird.pos, pol2rec({ 70, bird.angle + 325 }));

    vec_t points7 = addV(bird.pos, pol2rec({ 30, bird.angle + 90 }));
    vec_t points8 = addV(bird.pos, pol2rec({ 49, bird.angle + 100}));
    vec_t points9 = addV(bird.pos, pol2rec({ 49, bird.angle + 80 }));

    vec_t points10 = addV(bird.pos, pol2rec({ 70, bird.angle + 90}));

    vec_t points11 = addV(bird.pos, pol2rec({ 40, bird.angle + 100 }));
    vec_t points12 = addV(bird.pos, pol2rec({ 40, bird.angle + 80 }));

    glColor3fv(colors[colorIndex]);
    glBegin(GL_TRIANGLES);
    glVertex2f(point1.x, point1.y);
    glVertex2f(point2.x, point2.y);
    glVertex2f(point3.x, point3.y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(point4.x, point4.y);
    glVertex2f(point5.x, point5.y);
    glVertex2f(point6.x, point6.y);
    glEnd();

    circle(bird.pos.x, bird.pos.y, birdradius);
    circle(points7.x, points7.y, birdradius/1.5);
    glColor3f(1, 1, 1);
    circle(points11.x, points11.y, birdradius / 6);
    circle(points12.x, points12.y, birdradius / 6);
    glColor3f(0, 0, 0);
    circle(points11.x, points11.y, birdradius / 8);
    circle(points12.x, points12.y, birdradius / 8);
    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(points8.x, points8.y);
    glVertex2f(points9.x, points9.y);
    glVertex2f(points10.x, points10.y);
    glEnd();
}
void drawCloud(int x, int y) {
    glColor3f(1, 1, 1); 
    circle(0 + x, 0 + y, 30);
    circle(-25 + x, y, 20);
    circle(25 + x, -2 + y, 20);
    circle(21 + x, -19 + y, 10);
}


void drawGradient(int x1, int y1, int w, int h, float r, float g, float b) {
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glColor3f(r + 0.4, g + 0.4, b + 0.4);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();
}
void drawBalloon() {
    
    glColor3f(0.55, 0.67, 0.64);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(xB - 38, yB);
    glVertex2f(xB - 38, yB + 90);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(xB + 35, yB);
    glVertex2f(xB + 35, yB + 90);
    glEnd();
    glColor3f(0.08, 0.72, 0.54);
    halfcircle(xB, yB + 85, 50);
    glColor3f(0.34, 0.8, 0.14); // yellow
    
    circle(xB - 17, yB + 30, 9);
    circle(xB, yB + 33, 9);
    oval(xB, yB, 40, 30);

    glColor3f(1, 1, 1); 
    circle(xB - 17, yB, 10);//eyes
    circle(xB + 17, yB, 10);//eyes
    glColor3f(0.39, 0.96, 0.14);
    oval(xB, yB - 4, 12, 10);
    glColor3f(0, 0, 0);
    circle(xB - 17, yB, 3);
    circle(xB + 17, yB, 3);
    oval(xB - 5, yB - 4, 3, 4);
    oval(xB + 5, yB - 4, 2, 4);
    oval(xB - 17, yB + 15, 7, 2);
    oval(xB + 17, yB + 15, 7, 2);

  

}
void drawBackground() {
    drawGradient(-400, 300, 800, 600, 0.7, 0.7, 1); //sky
    drawGradient(-400, -200, 800, 100, 0, 0.7, 0); //sea

    

    //title
    glColor3f(1, 1, 1);
    print(-90, 275, "-= ANGRY BIRDS =-", GLUT_BITMAP_TIMES_ROMAN_24);

    //bottom info panel
    glColor3f(0, 0, 0);
    glRectf(-400, -280, 400, -300);

    //info message
    glColor3f(1, 1, 0); //yellow
    

    //draw 3 clouds
    drawCloud(-300, 150);
    drawCloud(300, 100);
    drawCloud(0, 200);
    drawCloud(0, -50);
}
//
// To display onto window using OpenGL commands
//
int burstx, bursty;
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    if(pig)
        drawBalloon();
    if(birdexist)
        if(mouseClicked == true)
         drawBird();//the input will be from a click
    if (burst == true) {
        drawBurst(burstx, bursty);
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



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
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
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
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
void onClick(int button, int state, int x, int y)
{
    // Write your codes here.
   
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Normalize the mouse coordinates to the OpenGL coordinate system
            float normalizedX = x - WINDOW_WIDTH / 2;
            float normalizedY = -y + WINDOW_HEIGHT / 2;

            bird.pos.x = normalizedX;
            bird.pos.y = normalizedY;
            mouseClicked = true;
            colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));

            glutPostRedisplay();   // Trigger a redisplay to update the object's position
        
    }

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

int q = 0;

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    
    bird.vel = mulV(velocity, unitV(subV({ xB, yB }, bird.pos)));
    bird.angle = angleV(subV({ xB, yB }, bird.pos)) + 270;
    bird.pos = addV(bird.pos, bird.vel);
   
    if (burst) {
        q++;
        if (q == 80) {
            burst = false;
            q = 0;
           animation = 1;
        }
    }
    if (animation) {
     
        xB += 0.2 * Vx;
        yB += Vx * 0.15;
        if (yB > 380) {
            yB = -150;
            xB = -350;
        }

    }

   
    if (sqrtf(powf((bird.pos.x - xB), 2) + powf((bird.pos.y - yB), 2)) < 50 && mouseClicked == true) {
        yB = -250;
        xB = -550;
        mouseClicked = false;
        burst = true;
        burstx = bird.pos.x;
        bursty = bird.pos.y;
        animation = false;
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Template File");

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