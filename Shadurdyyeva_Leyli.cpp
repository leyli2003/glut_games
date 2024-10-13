/*********
   CTIS164 - Homework 1
----------
STUDENT : Leyli Shadurdyyeva
ID NUMBER: 22201183
SECTION : 2
HOMEWORK: 1
----------
PROBLEMS: No problems
----------
ADDITIONAL FEATURES: by clicking on the screen, the color of the rocket changes randomly;
by pressing on the "s" or "S" key, the size of the moon becomes smaller, when pressing "b" or "B" the size of the moon becomes bigger;
by using a random number generator, I created the twinkling stars on the night sky;
by using quads and rectangles, I created an open window with see-through glass rather than a closed window :) 
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

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int TIMER = 1; 
int colR = 0, colG = 0, colB = 255;
int radius = 80;
//
// to draw circle, center at (x,y)
// radius r
//

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
//draw moon
void drawMoon(int x, int y, int radius)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 255);
    glVertex2f(x, y);
    glColor3ub(110, 110, 110);
    //creates a gradient for the moon
    for (int i = 0; i < 120; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

//draw the rocket
int yrocket = -240;
void drawRocket() {
    //the fins of the rocket
    glColor3ub(colR, colG, colB);
    glBegin(GL_TRIANGLES);
    glVertex2f(-30, 0 + yrocket);
    glVertex2f(30, 0 + yrocket);
    glVertex2f(0, 40 + yrocket);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30, 0 + yrocket);
    glVertex2f(-30, -50 + yrocket);
    glVertex2f(-70, -50 + yrocket);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(30, 0 + yrocket);
    glVertex2f(30, -50 + yrocket);
    glVertex2f(70, -50 + yrocket);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30, -90 + yrocket);
    glVertex2f(-80, -200 + yrocket);
    glVertex2f(-30, -200 + yrocket);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(30, -90 + yrocket);
    glVertex2f(80, -200 + yrocket);
    glVertex2f(30, -200 + yrocket);
    glEnd();
    //the fire engine
    glColor3f(1.0, 0, 0);
    glBegin(GL_TRIANGLE_STRIP);  // start drawing a triangle strip

    glVertex2f(-20, -200 + yrocket);  // first vertex
    glVertex2f(-60, -260 + yrocket);   // second vertex
    glVertex2f(0, -230 + yrocket);   // third vertex
    glVertex2f(60, -260 + yrocket); // fourth vertex
    glVertex2f(20, -200 + yrocket);  // fifth vertex

    glEnd();
    glColor3f(1.0, 1.0, 0);
    glBegin(GL_TRIANGLE_STRIP);  // start drawing a triangle strip

    glVertex2f(-15, -200 + yrocket);  // first vertex
    glVertex2f(-50, -250 + yrocket);   // second vertex
    glVertex2f(0, -210 + yrocket);   // third vertex
    glVertex2f(50, -250 + yrocket); // fourth vertex
    glVertex2f(15, -200 + yrocket);  // fifth vertex

    glEnd();
    //the rocket body
    glColor3ub(131, 139, 131);
    glRectf(-30, 0 + yrocket, 30, -200 + yrocket);
    //the window on the rocket
    glColor3f(1, 1, 1);
    circle(0, -60 + yrocket, 27);

    glColor3f(0, 0, 0);
    circle(0, -60 + yrocket, 20);

}


void displayWall()
{
    //create the wall
    glColor3f(1, 0.5, 0.2);
    glRectf(-500, 350, 500, 267);
    glColor3f(1, 0.5, 0.2);
    glRectf(500, 267, 372, -350);
    glColor3f(1, 0.5, 0.2);
    glRectf(372, -350, -500, -270);
    glColor3f(1, 0.5, 0.2);
    glRectf(-500, -270, -372, 350);
    glLineWidth(5);//creates brown wall
    glColor3ub(139, 69, 19);
    for (int i = 0; i < 3; i++)//creates the lines on the wall
    {
        glBegin(GL_LINES);
        glVertex2f(-490 + (40 * i), 350);
        glVertex2f(-490 + (40 * i), -350);
        glEnd();
    }
    for (int i = 0; i < 20; i++)//creates the lines on the wall
    {
        glBegin(GL_LINES);
        glVertex2f(-370 + (40 * i), 350);
        glVertex2f(-370 + (40 * i), 290);
        glVertex2f(-370 + (40 * i), -290);
        glVertex2f(-370 + (40 * i), -350);
        glEnd();
    }
    for (int i = 0; i < 3; i++)//creates the lines on the wall
    {
        glBegin(GL_LINES);
        glVertex2f(430 + (40 * i), 350);
        glVertex2f(430 + (40 * i), -350);
        glEnd();
    }

    glLineWidth(50);//creates brown border
    glColor3ub(92, 51, 23);
    glRectf(-400, 290, 400, 250);
    glRectf(400, 290, 370, -285);
    glRectf(-400, -290, 400, -250);
    glRectf(-400, -290, -370, 285);


    }
    void display_background() {
        
        glPointSize(2.0); // set the size of the stars
        glColor3f(1.0, 1.0, 1.0); // set the color of the stars to white
        glBegin(GL_POINTS);
        for (int i = 0; i < 50; i++)
        {
            glVertex2f(rand() % -370, rand() % 260);
            glVertex2f(rand() % 370, -rand() % 260);
            glVertex2f(-rand() % 370, rand() % 260);
            glVertex2f(-rand() % 370, -rand() % 260);
        }
        glEnd(); 
        drawMoon(-200, 0, radius);
    }

    void displayWindows()
    {
        //creates the open windows and the glass
        glColor4f(0, 0, 1, 0.5);
        glBegin(GL_QUADS);
        glVertex2f(-370, 260);
        glVertex2f(-180, 170);
        glVertex2f(-180, -190);
        glVertex2f(-370, -260);
        glEnd();

        glColor4f(0, 0, 1, 0.5);
        glBegin(GL_QUADS);
        glVertex2f(370, 260);
        glVertex2f(180, 170);
        glVertex2f(180, -190);
        glVertex2f(370, -260);
        glEnd();
        glLineWidth(20);
        glColor3ub(92, 51, 23);
        glBegin(GL_LINES);
        glVertex2f(-370, 260);
        glVertex2f(-180, 170);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(-180, 179);
        glVertex2f(-180, -189);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(-185, -180);
        glVertex2f(-370, -260);
        glEnd();

        glLineWidth(20);
        glColor3ub(92, 51, 23);
        glBegin(GL_LINES);
        glVertex2f(370, 260);
        glVertex2f(180, 170);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(180, 179);
        glVertex2f(180, -189);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(185, -180);
        glVertex2f(370, -260);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(-270, 210);
        glVertex2f(-270, -210);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(185, -180);
        glVertex2f(370, -260);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(-370, 0);
        glVertex2f(-180, 0);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(260, 200);
        glVertex2f(260, -210);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(370, 0);
        glVertex2f(180, 0);
        glEnd();
    }
void display() {
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    display_background();
    displayWindows();
    drawRocket();
    displayWall();
    //labels
    vprint(-440, 330, GLUT_BITMAP_8_BY_13, "Leyli Shadurdyyeva");
    vprint(-440, 310, GLUT_BITMAP_8_BY_13, "22201183");
    vprint(-70, 330, GLUT_BITMAP_9_BY_15, " - Homework -");
    glColor3ub(255, 255, 255);
    vprint(-150, 300, GLUT_BITMAP_HELVETICA_18, "Above The City by Leyli Shadurdyyeva");
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
    if (key == 32)
    {
        if (TIMER == 1)
            TIMER = 0;
        else if (TIMER == 0)
            TIMER = 1;
    }
    if (key == 'b' || key == 'B') // Make bigger
        if (radius <= 250)
            radius += 5;

    if (key == 's' || key == 'S') // Make smaller
        if (radius > 25)
            radius -= 5;


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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        colR = rand() % 256;
        colG = rand() % 256;
        colB = rand() % 256;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.

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


#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (TIMER == 1)
        yrocket++;
    if (yrocket >= 550)
        yrocket = -290;

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
    glutCreateWindow("Homework 1");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);

    glutKeyboardUpFunc(onKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
    
    Init();

    glutMainLoop();
}
