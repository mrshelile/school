#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

//---------------------------------------------------------------------
// Initialization Function: myInit
// This function sets up the OpenGL environment (background color,
// drawing color, projection matrix, etc.) before rendering.
void myInit(void) {
    // Set the background color to black (RGBA: 0,0,0,1)
    glClearColor(0, 0, 0, 1);

    // Set the drawing color to red (RGB: 1, 0, 0)
    glColor3f(1, 0, 0);

    // Set up the projection matrix for 2D drawing:
    // glMatrixMode(GL_PROJECTION) tells OpenGL that we want to modify the projection matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // gluOrtho2D defines a 2D orthographic projection matrix.
    // Here, the coordinate system is set from (0,0) to (420,200).
    // This defines the visible region (viewport) of the window.
    gluOrtho2D(0.0, 420, 0.0, 200);

    // The commented-out code below shows an alternative method to set the
    // window dimension and point size, but it is not active here.
    /*
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-780, 780, -420, 420);
    */
}

//---------------------------------------------------------------------
// Function: EighWaySymmetryPlot
// Purpose: Plot the eight symmetrical points of a circle based on the
// current point (x, y) and the center (p, q). This uses the eight-way
// symmetry property of circles, reducing the computations needed.
void EighWaySymmetryPlot(int p, int q, int x, int y) {
    // For a circle, if (x, y) is a point on the circle, then the following
    // eight points are also on the circle:
    glVertex2i(x + p, y + q);   // Point 1: (x+p, y+q)
    glVertex2i(y + p, x + q);   // Point 2: (y+p, x+q)
    glVertex2i(-y + p, x + q);  // Point 3: (-y+p, x+q)
    glVertex2i(-x + p, y + q);  // Point 4: (-x+p, y+q)
    glVertex2i(-x + p, -y + q); // Point 5: (-x+p, -y+q)
    glVertex2i(-y + p, -x + q); // Point 6: (-y+p, -x+q)
    glVertex2i(y + p, -x + q);  // Point 7: (y+p, -x+q)
    glVertex2i(x + p, -y + q);  // Point 8: (x+p, -y+q) -- note the correction here
}

//---------------------------------------------------------------------
// Function: BresenhamCircle
// Purpose: Use Bresenham's circle drawing algorithm to compute the points
// on a circle and then plot them using eight-way symmetry. The circle is
// defined by its center (p, q) and radius r.
void BresenhamCircle(int p, int q, int r) {
    int x = 0;       // Start at x = 0
    int y = r;       // y starts at the radius value (top of the circle)

    // Initialize the decision parameter 'd'. This helps determine whether
    // to move horizontally or diagonally when plotting the next point.
    int d = 3 - (2 * r);

    // Begin drawing points:
    glBegin(GL_POINTS);

    // Plot the initial set of eight points for the starting point (0, r)
    EighWaySymmetryPlot(p, q, x, y);

    // The loop continues until x > y. This is because the algorithm
    // computes only one octant and uses symmetry for the rest.
    while (x <= y) {
        // Decision parameter check:
        // If d is less than or equal to 0, the midpoint is inside the circle.
        if (d <= 0) {
            // Choose the horizontal pixel. Update d accordingly.
            d += (4 * x) + 6;
        } else {
            // Otherwise, choose the diagonal pixel:
            // Update the decision parameter 'd' to reflect the diagonal move.
            d += 4 * (x - y) + 10;
            // Decrement y because we are moving from the outer edge toward the center.
            y--;
        }
        // Increment x for the next iteration.
        x++;
        // Plot the eight symmetrical points for the new (x, y)
        EighWaySymmetryPlot(p, q, x, y);
    }

    // End drawing points.
    glEnd();
}

//---------------------------------------------------------------------
// Function: DrawLine (Display Callback)
// Purpose: This is the callback function that clears the window and draws
// the circle by calling BresenhamCircle. It is registered with GLUT so
// that it is called whenever the window needs to be redrawn.
void DrawLine(void) {
    // Clear the color buffer to set the background as initialized.
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the circle using Bresenham's algorithm.
    // The circle's center is set at (100,100) and its radius is 10.
    BresenhamCircle(100, 100, 20);

    // Flush the OpenGL commands to the window.
    glFlush();
}

//---------------------------------------------------------------------
// Main function: Program Entry Point
// Purpose: Set up the GLUT environment, register the display callback, and
// start the main event loop.
int main(int argC, char *argV[]) {
    // Initialize GLUT with command-line arguments.
    glutInit(&argC, argV);

    // Set the initial position of the window on the screen.
    glutInitWindowPosition(450, 100);

    // Set the initial size of the window (width x height).
    glutInitWindowSize(600, 600);

    // Specify the display mode: RGB color and single buffering.
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    // Create the window with the title "BresenHam Draw Circle".
    glutCreateWindow("BresenHam Draw Circle");

    // Call our initialization function.
    myInit();

    // Register the display callback function which is called to redraw the window.
    glutDisplayFunc(DrawLine);

    // Enter the GLUT main loop. This loop waits for events and calls
    // the registered callback functions accordingly.
    glutMainLoop();

    // Return statement (not reached because glutMainLoop is an infinite loop).
    return 0;
}
