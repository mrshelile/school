#include <stdio.h>
#include <stdlib.h>
#include<GL/glut.h>
#include<GL/glu.h>
#include<math.h>

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
void BresenHam(int x0,int y0,int x1,int y1){
 //declaring all variables
 int dy,dx,p;

 // calculate dy,dx
 dx = x1 - x0;
 dy = y1 - y0;

 // set current point
 int x = x0;
 int y = y0;

 // calculate P(decision parameter)
 p = 2*dy-2*dx;

  // Begin drawing points
 glBegin(GL_POINTS);
 glVertex2f(x, y);  // Plot the first point

 // plot the points until we reached destination point
 while(x<x1){


    if(p>0){
        p = p + 2*dy - 2*dx;
        x++;
        y++;
        glVertex2i(x,y);
    }
    else{
      p = p + 2*dy;
      y=y;
      x++;
      glVertex2i(x,y);
    }
 }
  glEnd();

}
void DrawLine(){

 // Clear background color to the way i initialized
   glClear(GL_COLOR_BUFFER_BIT);
   BresenHam(110,10,210,30);

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
  glutCreateWindow("BresenHam Draw Line");

    // my initialization
    myInit();

  //display callback function
  glutDisplayFunc(DrawLine);

  // window loop
  glutMainLoop();

    return 0;
}
