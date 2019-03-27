#include "VectorUtils3.h"

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "loadobj.h"

#include "Terrain.hh"

Terrain* terrain;

void init() {
    terrain = new Terrain();
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1000, 1500);
    glutCreateWindow("WAWW");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    exit(0);
}