#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z) {
    Vec3 v = {x, y, z};
    return v;
}

Vec3 add(Vec3 a, Vec3 b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 sub(Vec3 a, Vec3 b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 scale(Vec3 v, float s) {
    return vec3(v.x * s, v.y * s, v.z * s);
}

float length(Vec3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

void normalize(Vec3* v) {
    float len = length(*v);
    if (len > 1e-6f) {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
}

Vec3 cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Plane equation: Ax + By + Cz + D = 0
float A = 1, B = 1, C = 1, D = 1; // Example: x + y + z + 1 = 0
float planeSize = 2.0f;

void drawPlane() {
    Vec3 normal = vec3(A, B, C);
    normalize(&normal);

    Vec3 center;
    if (fabs(C) > 1e-6)
        center = vec3(0, 0, -D/C);
    else if (fabs(B) > 1e-6)
        center = vec3(0, -D/B, 0);
    else
        center = vec3(-D/A, 0, 0);

    Vec3 up = vec3(0, 1, 0);
    Vec3 tangent = cross(normal, up);
    if (length(tangent) < 1e-3)
        tangent = cross(normal, vec3(1, 0, 0));
    normalize(&tangent);

    Vec3 bitangent = cross(normal, tangent);
    normalize(&bitangent);

    Vec3 v1 = sub(sub(center, scale(tangent, planeSize)), scale(bitangent, planeSize));
    Vec3 v2 = add(sub(center, scale(bitangent, planeSize)), scale(tangent, planeSize));
    Vec3 v3 = add(add(center, scale(tangent, planeSize)), scale(bitangent, planeSize));
    Vec3 v4 = sub(add(center, scale(bitangent, planeSize)), scale(tangent, planeSize));

    glColor4f(0.8f, 0.1f, 0.1f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}

void drawCube() {
    glColor3f(0.2f, 0.6f, 0.9f);
    glutWireCube(2.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    gluLookAt(4, 4, 4, 0, 0, 0, 0, 1, 0);

    drawCube();
    drawPlane();

    glutSwapBuffers();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cube + Plane (Ax + By + Cz + D = 0)");

    initOpenGL();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
