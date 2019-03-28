
#include <algorithm>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

#include "Camera.hh"
#include "Skybox.hh"
#include "Terrain.hh"

Terrain terrain;
Skybox sky;
Camera cam {};

mat4 camMatrix;

int mousex;
int mousey;
int buttonState = GLUT_UP;

void init() {
    /* INIT GL */
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    /* SETUP PROGRAMS */
    GLuint terrainShader = loadShaders("assets/shaders/terrain.vert",
                                       "assets/shaders/terrain.frag");
    glUseProgram(terrainShader);
    glUniformMatrix4fv(glGetUniformLocation(terrainShader, "projMatrix"), 1,
                       GL_TRUE, cam.projectionMatrix.m);
    glUniform1i(glGetUniformLocation(terrainShader, "grass"), 0);
    glUniform1i(glGetUniformLocation(terrainShader, "dirt"), 1);

    GLuint skyShader =
        loadShaders("assets/shaders/sky.vert", "assets/shaders/sky.frag");
    glUseProgram(skyShader);
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "projMatrix"), 1,
                       GL_TRUE, cam.projectionMatrix.m);
    glUniform1i(glGetUniformLocation(skyShader, "sky"), 0);

    printError("ERROR: SETUP PROGRAMS");

    /* SETUP TEXTURES */
    GLuint grassTex, dirtTex, skyTex;
    LoadTGATextureSimple("assets/textures/grass.tga", &grassTex);
    LoadTGATextureSimple("assets/textures/dirt.tga", &dirtTex);
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);

    printError("ERROR: SETUP TEXTURES");

    /* LOAD MODELS */
    terrain.generate("assets/textures/terrain.tga");
    terrain.setShader(terrainShader);
    terrain.addTexture(grassTex);
    terrain.addTexture(dirtTex);

    sky.setShader(skyShader);
    sky.loadModel("assets/models/skybox.obj");
    sky.addTexture(skyTex);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sky.draw(cam);
    terrain.draw(cam);

    glutSwapBuffers();
}

void updateCam() {
    float mx = 0;
    float mz = 0;

    if (glutKeyIsDown('p')) cam.increase();
    if (glutKeyIsDown('o')) cam.decrease();
    if (glutKeyIsDown('a')) mx -= 1;
    if (glutKeyIsDown('d')) mx += 1;
    if (glutKeyIsDown('s')) mz += 1;
    if (glutKeyIsDown('w')) mz -= 1;

    if (glutKeyIsDown('r')) cam.up();
    if (glutKeyIsDown('e')) cam.down();

    cam.updatePos(mx, mz);

    float h = terrain.height(cam.x(), cam.z());
    cam.y() = h + 1.86;

    cam.updateCamMatrix();
}

void timer(int i) {
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
    updateCam();
}

void mouseButton(int button, int state, int x, int y) { buttonState = state; }

void mouseMove(int x, int y) {
    if (buttonState == GLUT_DOWN) {
        float dx = (x - mousex) / 200.f;
        float dy = (y - mousey) / 200.f;
        cam.rotateX(dx);
        cam.rotateY(dy);
    }

    mousex = x;
    mousey = y;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1000, 1500);
    glutCreateWindow("WAWW");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(20, &timer, 0);
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();
    exit(0);
}