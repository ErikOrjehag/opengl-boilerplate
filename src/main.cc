#include "VectorUtils3.h"

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "loadobj.h"

#include "Terrain.hh"

Terrain terrain;

void init() {

    mat4 projectionMatrix = frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 50.0);

    /* SETUP PROGRAMS */
    GLuint terrain_program = loadShaders("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");

    glUseProgram(terrain_program);
    glUniformMatrix4fv(glGetUniformLocation(terrain_program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(terrain_program, "grass"), 0);
    glUniform1i(glGetUniformLocation(terrain_program, "dirt"), 1);
    
    /* LOAD TEXTURES */
    GLuint grass, dirt;
    LoadTGATextureSimple("grass.tga", &grass);
    LoadTGATextureSimple("dirt.tga", &dirt);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dirt);

    /* LOAD MODELS */
    terrain.generate("assets/textures/terrain.tga");
    terrain.setProgram(terrain_program);

    /* INIT GL */
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