#include <stdio.h>
#include <stdlib.h>
#include<GL/glut.h>
#include<GL/glu.h>
#include<math.h>
#define PI 3.14

int abs(int n){return ((n>0) ? n: (n * (-1)));}

// function to initialize
void myInit (void)
{

	// making background color black as first
	// 3 arguments all are 0.0
	glClearColor(0, 0, 0, 1);

	// making picture color green (in RGB mode), as middle argument is 1.0
	glColor3f(1,0,0);

	 // Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLineWidth(10.0); // linewidth
    gluOrtho2D(0.0, 2000, 0.0, 1500); // scale change for visibility(this is a coordinate systems)

	// breadth of picture boundary is 1 pixel
	/*glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// setting window dimension in X- and Y- direction
	gluOrtho2D(-780, 780, -420, 420);*/
}

void DDA(float X0,float Y0,float X1,float Y1){

 float dy;
 float dx;
 float X;
 float Y;


// Calculate the difference between start and end points
 dx = X1-X0;
 dy = Y1-Y0;

  // Determine the number of steps based on the greater difference (dx or dy)
 int steps = abs(dx)>abs(dy) ? abs(dx) : abs(dy);

  // Calculate increments for each step
 float Xincrement = dx / (float)steps;
 float Yincrement = dy / (float)steps;

   // Initialize the starting point
 X = X0;
 Y = Y0;

  // Begin drawing points
 glBegin(GL_POINTS);
 glVertex2f(X, Y);  // Plot the first point


 // Draw all points using incremental steps
 for( int v=0; v <steps;v++){
    X = X + Xincrement;
    Y = Y + Yincrement;

    glVertex2f(X,Y);

 }
 glEnd();
}

void rotateLine1(float x0, float y0, float x1, float y1, float angle) {
    // Calculate the midpoint
    float mx = (x0 + x1) / 2.0;
    float my = (y0 + y1) / 2.0;

    // Translate the line to the origin
    float tx0 = x0 - mx;
    float ty0 = y0 - my;
    float tx1 = x1 - mx;
    float ty1 = y1 - my;

    // Perform the rotation
    float radians = angle * PI / 180.0;
    float cosTheta = cos(radians);
    float sinTheta = sin(radians);

    float rx0 = tx0 * cosTheta - ty0 * sinTheta;
    float ry0 = tx0 * sinTheta + ty0 * cosTheta;
    float rx1 = tx1 * cosTheta - ty1 * sinTheta;
    float ry1 = tx1 * sinTheta + ty1 * cosTheta;

    // Translate the line back to its original position
    rx0 += mx;
    ry0 += my;
    rx1 += mx;
    ry1 += my;

    // Draw the rotated line
    DDA(rx0, ry0, rx1, ry1);
}

void transformation(){
    // draw shape
    int Xa =200,Ya=100;
    int Xb=500,Yb=600;

    // Draw initial line
    DDA(Xa,Ya,Xb,Yb);

    // Set color for the rotated line (e.g., green)
    glColor3f(0.0f, 1.0f, 0.0f);

    //Translation
    int Tfx= 100,Tfy= 300;
    Xa =Xa+Tfx,Ya=Ya+Tfy;
    Xb=Xb+Tfx,Yb=Yb+Tfx;

    // draw translated line
    //DDA(Xa,Ya,Xb,Yb);

    //change color
    glColor3f(0.0f, 1.0f, 1.0f);

    //Scaling
/*    int Sfx=5,Sfy=5;
     Xa =Xa*Sfx,Ya=Ya*Sfy;
     Xb=Xb*Sfx,Yb=Yb*Sfx;

    // draw translated line
     DDA(Xa,Ya,Xb,Yb); */
    //Rotation
     rotateLine1(Xa,Ya,Xb,Yb,90);


    // what is being drawn, will immediately pushed in buffer. Render the changes
    glFlush();
}
void rotateLine(int x0, int y0, int x1, int y1, float angle) {
    double c = cos(angle * PI / 180.0);
    double s = sin(angle * PI / 180.0);

    // Compute rotated coordinates using temporary variables
    int rx0 = (int)floor(x0 * c - y0 * s);
    int ry0 = (int)floor(x0 * s + y0 * c);
    int rx1 = (int)floor(x1 * c - y1 * s);
    int ry1 = (int)floor(x1 * s + y1 * c);

    // Draw the rotated line
    DDA(rx0, ry0, rx1, ry1);
}

void DrawLine(){
   int x0 = 300,y0=100,x1=300,y1=350;
   // Clear background color to the way i initialized
   glClear(GL_COLOR_BUFFER_BIT);
   DDA(x0,y0,x1,y1);

   // Set color for the rotated line (e.g., green)
    glColor3f(0.0f, 1.0f, 0.0f);
    rotateLine1(x0,y0,x1,y1,90);

   // what is being drawn, will immediately pushed in buffer. Render the changes
   glFlush();
}
int main(int argC, char *argV[])
{
    // Initialize GLUT
  glutInit(&argC,argV);

  // set window init position
   glutInitWindowPosition(450,100);

   //window size init
   glutInitWindowSize(600,600);

   // mode color
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

  // create Window
  glutCreateWindow("2d Transformation");

    // my initialization
    myInit();

  //display callback function
  glutDisplayFunc(transformation);

  // window loop
  glutMainLoop();
    return 0;
}
