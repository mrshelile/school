#include <GL/glut.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.14159265358979323846

// Transformation parameters (initial values)
static float transX =   0.0f, transY =   0.0f;
static float scaleX =   1.0f, scaleY =   1.0f;
static float angle  =   0.0f;            // in degrees
static bool  reflectX = false, reflectY = false;
static float shearX =   0.0f, shearY =   0.0f;

// A simple square centered at the origin
static GLfloat verts[4][2] = {
    { -50.0f, -50.0f },
    {  50.0f, -50.0f },
    {  50.0f,  50.0f },
    { -50.0f,  50.0f }
};

// Helper to draw on‑screen text
void drawText(float x, float y, const char* str) {
    glRasterPos2f(x, y);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str++);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1) Apply all transforms in MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Translate → Rotate → Scale/Reflect → Shear
    glTranslatef(transX, transY, 0.0f);
    glRotatef(angle, 0, 0, 1);
    glScalef(
      scaleX * (reflectX ? -1.0f : 1.0f),
      scaleY * (reflectY ? -1.0f : 1.0f),
      1.0f
    );
    // build shear matrix
    GLfloat shearM[16] = {
      1.0f, shearY, 0, 0,
      shearX, 1.0f, 0, 0,
      0,     0,     1, 0,
      0,     0,     0, 1
    };
    glMultMatrixf(shearM);

    // draw the square
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
      for (int i = 0; i < 4; ++i)
        glVertex2fv(verts[i]);
    glEnd();

    // 2) Draw UI overlay (reset MODELVIEW)
    glLoadIdentity();
    glColor3f(0,0,0);
    char buf[128];

    sprintf(buf, "Translate: (%.0f, %.0f)   [←→/↑↓]", transX, transY);
    drawText(-290, 270, buf);

    sprintf(buf, "Scale: (%.2f, %.2f)        [w/s]", scaleX, scaleY);
    drawText(-290, 250, buf);

    sprintf(buf, "Rotate: %.0f°             [a/d]", angle);
    drawText(-290, 230, buf);

    sprintf(buf, "Reflect X: %s  [r]   Y: %s  [t]",
            reflectX?"On":"Off", reflectY?"On":"Off");
    drawText(-290, 210, buf);

    sprintf(buf, "Shear: (%.2f, %.2f)        [j/k = X]  [u/i = Y]",
            shearX, shearY);
    drawText(-290, 190, buf);

    glutSwapBuffers();
}

// Regular keys for scale, rotate, reflect, shear
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': angle += 5.0f;  break;
        case 'd': angle -= 5.0f;  break;
        case 'w': scaleX += 0.1f; scaleY += 0.1f; break;
        case 's': scaleX = fmaxf(scaleX - 0.1f, 0.1f);
                  scaleY = fmaxf(scaleY - 0.1f, 0.1f);
                  break;
        case 'r': reflectX = !reflectX; break;
        case 't': reflectY = !reflectY; break;

        case 27:  exit(0);              // ESC
    }
    glutPostRedisplay();
}

// Arrow keys for translation
void special(int key, int x, int y) {
    const float delta = 10.0f;
    switch (key) {
        case GLUT_KEY_LEFT:  transX -= delta; break;
        case GLUT_KEY_RIGHT: transX += delta; break;
        case GLUT_KEY_UP:    transY += delta; break;
        case GLUT_KEY_DOWN:  transY -= delta; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Double‑buffered, RGB
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Multi‑Transformation Demo");

    // Set up a simple orthographic view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1,1,1,1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
//    return 0;
}
