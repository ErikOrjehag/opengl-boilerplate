
#include <algorithm>
#include <memory>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

#include "Camera.hh"
#include "FrameBuffer.hh"
#include "Skybox.hh"
#include "Terrain.hh"
#include "Water.hh"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const vec4 waterPlane(0, -1, 0, 3);

std::unique_ptr<Terrain> terrain;
std::unique_ptr<Skybox> sky;
std::unique_ptr<Water> water;
std::unique_ptr<Camera> cam;
std::unique_ptr<FrameBuffer> reflectionFBO;
std::unique_ptr<FrameBuffer> refractionFBO;

mat4 camMatrix;

int mousex;
int mousey;
int buttonState = GLUT_UP;

void bindDefaultFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void init() {
    /* INIT GL */
    glClearColor(0.2, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    printError("GL inits");

    /* CREATE CAMERA */
    cam = std::make_unique<Camera>();

    /* SETUP FRAME BUFFERS */
    reflectionFBO = std::make_unique<FrameBuffer>(SCREEN_HEIGHT / 4,
                                                  SCREEN_WIDTH / 4, false);
    refractionFBO =
        std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, true);
    bindDefaultFramebuffer();

    /* SETUP PROGRAMS */
    GLuint terrainShader = loadShaders("assets/shaders/terrain.vert",
                                       "assets/shaders/terrain.frag");
    glUseProgram(terrainShader);
    glUniformMatrix4fv(glGetUniformLocation(terrainShader, "projMatrix"), 1,
                       GL_TRUE, cam->projectionMatrix.m);
    glUniform1i(glGetUniformLocation(terrainShader, "grass"), 0);
    glUniform1i(glGetUniformLocation(terrainShader, "dirt"), 1);

    GLuint skyShader =
        loadShaders("assets/shaders/sky.vert", "assets/shaders/sky.frag");
    glUseProgram(skyShader);
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "projMatrix"), 1,
                       GL_TRUE, cam->projectionMatrix.m);
    glUniform1i(glGetUniformLocation(skyShader, "sky"), 0);

    GLuint waterShader =
        loadShaders("assets/shaders/water.vert", "assets/shaders/water.frag");
    glUseProgram(waterShader);
    glUniformMatrix4fv(glGetUniformLocation(waterShader, "projMatrix"), 1,
                       GL_TRUE, cam->projectionMatrix.m);
    glUniform1i(glGetUniformLocation(waterShader, "reflection"), 0);
    glUniform1i(glGetUniformLocation(waterShader, "refraction"), 1);
    glUniform1i(glGetUniformLocation(waterShader, "depth"), 2);

    printError("ERROR: SETUP PROGRAMS");

    /* SETUP TEXTURES */
    GLuint grassTex, dirtTex, skyTex;
    LoadTGATextureSimple("assets/textures/grass.tga", &grassTex);
    LoadTGATextureSimple("assets/textures/dirt.tga", &dirtTex);
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);

    printError("ERROR: SETUP TEXTURES");

    /* LOAD MODELS */
    terrain = std::make_unique<Terrain>();
    terrain->generate("assets/textures/terrain.tga");
    terrain->setShader(terrainShader);
    terrain->addTexture(grassTex);
    terrain->addTexture(dirtTex);

    sky = std::make_unique<Skybox>();
    sky->setShader(skyShader);
    sky->loadModel("assets/models/skybox.obj");
    sky->addTexture(skyTex);

    water = std::make_unique<Water>();
    water->generate(50.0, waterPlane.w, 50.0, 20.0, 20.0);
    water->setShader(waterShader);
    water->addTexture(reflectionFBO->texture);
    water->addTexture(refractionFBO->texture);
    water->addTexture(refractionFBO->depth);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bindDefaultFramebuffer();

    // Refraction
    refractionFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    terrain->draw(*cam, waterPlane);

    Camera camCopy = *cam;

    camCopy.invertPitch();
    float distance = 2 * (camCopy.y() - waterPlane.w);
    camCopy.y() -= distance;
    camCopy.updateCamMatrix();

    // Reflection
    reflectionFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    terrain->draw(camCopy, waterPlane * -1);
    sky->draw(camCopy);

    // Scene
    bindDefaultFramebuffer();
    sky->draw(*cam);
    terrain->draw(*cam, vec4(0, 1, 0, 1e6));
    water->draw(*cam);

    glutSwapBuffers();

    printError("ERROR: DRAW");
}

void updateCam() {
    float mx = 0;
    float mz = 0;

    if (glutKeyIsDown('p')) cam->increase();
    if (glutKeyIsDown('o')) cam->decrease();
    if (glutKeyIsDown('a')) mx -= 1;
    if (glutKeyIsDown('d')) mx += 1;
    if (glutKeyIsDown('s')) mz += 1;
    if (glutKeyIsDown('w')) mz -= 1;

    if (glutKeyIsDown('r')) cam->up();
    if (glutKeyIsDown('e')) cam->down();
    if (glutKeyIsDown('i')) cam->invertPitch();

    cam->updatePos(mx, mz);

    // float h = terrain.height(cam.x(), cam.z());
    // cam.y() = h + 1.86;

    cam->updateCamMatrix();
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
        cam->rotateX(dx);
        cam->rotateY(dy);
    }

    mousex = x;
    mousey = y;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("WAWW");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(20, &timer, 0);
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();
    exit(0);
}