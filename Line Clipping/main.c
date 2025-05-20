#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // for bool, true, false
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

// Region codes
#define INSIDE 0  // 0000
#define LEFT   1  // 0001
#define RIGHT  2  // 0010
#define BOTTOM 4  // 0100
#define TOP    8  // 1000

// Clipping window boundaries
float xmin = 50.0f, ymin = 10.0f, xmax = 80.0f, ymax = 40.0f;
// Viewport boundaries
float vxmin = 200.0f, vymin = 50.0f, vxmax = 350.0f, vymax = 150.0f;

// Original line endpoints (p1 inside, p2 outside)
float x_start = 70.0f, y_start = 20.0f;  // inside window
float x_end   =  100.0f, y_end   = 10.0f;  // outside window
// Clipped line endpoints
float cx0, cy0, cx1, cy1;
bool clipped = false;

// Function prototypes
int ComputeOutCode(float x, float y);
bool CohenSutherlandLineClipAndDraw(float xs, float ys, float xe, float ye);
void DDA(float X0, float Y0, float X1, float Y1);
void DrawClippingWindow();
void DrawViewport();
void display(void);
void keyboard(unsigned char key, int x, int y);
void init(void);

// DDA line drawing
void DDA(float X0, float Y0, float X1, float Y1) {
    float dx = X1 - X0;
    float dy = Y1 - Y0;
    int steps = (int)(fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy));
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;
    float X = X0, Y = Y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(X, Y);
        X += Xinc;
        Y += Yinc;
    }
    glEnd();
}

// Draw the clipping window in green
void DrawClippingWindow() {
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();
}

// Draw the viewport in yellow
void DrawViewport() {
    glColor3f(1, 1, 0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(vxmin, vymin);
        glVertex2f(vxmax, vymin);
        glVertex2f(vxmax, vymax);
        glVertex2f(vxmin, vymax);
    glEnd();
}

// Main display function
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw original line in red
    glColor3f(1, 0, 0);
    DDA(x_start, y_start, x_end, y_end);

    // Draw clipping window
    DrawClippingWindow();

    if (clipped) {
        // Draw clipped line in cyan
        glColor3f(0, 1, 1);
        DDA(cx0, cy0, cx1, cy1);

        // Draw viewport
        DrawViewport();

        // Map clipped line into viewport and draw in magenta
        float sx = (vxmax - vxmin) / (xmax - xmin);
        float sy = (vymax - vymin) / (ymax - ymin);
        float vx0 = vxmin + (cx0 - xmin) * sx;
        float vy0 = vymin + (cy0 - ymin) * sy;
        float vx1 = vxmin + (cx1 - xmin) * sx;
        float vy1 = vymin + (cy1 - ymin) * sy;

        glColor3f(1, 0, 1);
        DDA(vx0, vy0, vx1, vy1);
    }

    glFlush();
}

// Handle keypress: 'c' to clip
void keyboard(unsigned char key, int x, int y) {
    if (key == 'c' || key == 'C') {
        clipped = CohenSutherlandLineClipAndDraw(x_start, y_start,
                                                 x_end,   y_end);
        glutPostRedisplay();
    }
}

// Compute region outcode for point (x,y)
int ComputeOutCode(float x, float y) {
    int code = INSIDE;
    if (x < xmin)      code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin)      code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

// Cohen-Sutherland line clipping algorithm
bool CohenSutherlandLineClipAndDraw(float xs, float ys, float xe, float ye) {
    float x0 = xs, y0 = ys, x1 = xe, y1 = ye;
    int out0 = ComputeOutCode(x0, y0);
    int out1 = ComputeOutCode(x1, y1);
    bool accept = false;

    while (true) {
        if ((out0 | out1) == INSIDE) {
            accept = true;
            break;
        } else if (out0 & out1) {
            break;
        } else {
            int outcodeOut = out0 ? out0 : out1;
            float x, y;

            if (outcodeOut & TOP) {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & BOTTOM) {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & RIGHT) {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else { // LEFT
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == out0) {
                x0 = x; y0 = y; out0 = ComputeOutCode(x0, y0);
            } else {
                x1 = x; y1 = y; out1 = ComputeOutCode(x1, y1);
            }
        }
    }

    if (accept) {
        cx0 = x0; cy0 = y0;
        cx1 = x1; cy1 = y1;
    }
    return accept;
}

// OpenGL initialization
void init(void) {
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 200.0);
}

// Entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 300);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Cohen-Sutherland Clipping & Viewport Mapping");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
