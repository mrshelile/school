
// C program to demonstrate
// drawing a circle using
// OpenGL
#include <stdlib.h>
#include<stdio.h>
#include<GL/glut.h>
#include<GL/glu.h>
#include<math.h>
//#define pi 3.142857

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
    gluOrtho2D(0.0, 420, 0.0, 200); // scale change for visibility(this is a coordinate systems)

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
void DrawLine(){

   // Clear background color to the way i initialized
   glClear(GL_COLOR_BUFFER_BIT);
   DDA(110,10,80,30);


   // what is being drawn, will immediately pushed in buffer. Render the changes
   glFlush();
}
int main (int argC, char *argV[])
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
  glutCreateWindow("DDA Draw Line");

    // my initialization
    myInit();

  //display callback function
  glutDisplayFunc(DrawLine);

  // window loop
  glutMainLoop();
  return 0;
}
