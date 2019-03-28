
#include <algorithm>

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "MicroGlut.h"
#include "loadobj.h"

#include "Terrain.hh"

Terrain terrain;
mat4 camMatrix;

float yrot = 0;
float xrot = M_PI * 3.f / 4.f;
vec3 camPos = {1, 8, 1};
float camSpeed = 0.7;

int mousex;
int mousey;
int buttonState = GLUT_UP;

void init() {

    camMatrix = MultMat4(Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));

    mat4 projectionMatrix = frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 50.0);

    /* SETUP PROGRAMS */
    GLuint terrain_program = loadShaders("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");

    glUseProgram(terrain_program);
    glUniformMatrix4fv(glGetUniformLocation(terrain_program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniform1i(glGetUniformLocation(terrain_program, "grass"), 0);
    glUniform1i(glGetUniformLocation(terrain_program, "dirt"), 1);

    printError("ERROR: SETUP PROGRAMS");
    
    /* SETUP TEXTURES */
    GLuint grassTex, dirtTex, skyTex;
    LoadTGATextureSimple("assets/textures/grass.tga", &grassTex);
    LoadTGATextureSimple("assets/textures/dirt.tga", &dirtTex);
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);
    
    printError("ERROR: SETUP TEXTURES");

    /* LOAD MODELS */
    terrain.generate("assets/textures/terrain.tga");
    terrain.setProgram(terrain_program);
    terrain.addTexture(grassTex);
    terrain.addTexture(dirtTex);
    
    /* INIT GL */
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    terrain.draw(camMatrix);

    glutSwapBuffers();
}

void updateCam() {
    float mx = 0;
    float mz = 0;

    if (glutKeyIsDown('p')) camSpeed += 0.01;
    if (glutKeyIsDown('o')) camSpeed = std::max<double>(0, camSpeed - 0.01);
    if (glutKeyIsDown('a')) mx -= 1;
    if (glutKeyIsDown('d')) mx += 1;
    if (glutKeyIsDown('s')) mz += 1;
    if (glutKeyIsDown('w')) mz -= 1;
    if (glutKeyIsDown('r')) camPos.y += camSpeed;
    if (glutKeyIsDown('e')) camPos.y -= camSpeed;
    
    camPos.x += mx * camSpeed * cos(xrot) - mz * camSpeed * sin(xrot);
    camPos.z += mx * camSpeed * sin(xrot) + mz * camSpeed * cos(xrot);

    float h = terrain.height(camPos.x, camPos.z);
    camPos.y = h + 1.86;
    
    camMatrix = MultMat4(Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));
}

void timer(int i) {
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
    updateCam();
}

void mouseButton(int button, int state, int x, int y) {
    buttonState = state;
}

void mouseMove(int x, int y) {
    if (buttonState == GLUT_DOWN) {
        float dx = (x - mousex) / 200.f;
        float dy = (y - mousey) / 200.f;
        xrot += dx;
        yrot += dy;
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