
#include <algorithm>
#include <memory>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

#include "Camera.hh"
#include "FrameBuffer.hh"
#include "ScreenFill.hh"
#include "Skybox.hh"
#include "Terrain.hh"
#include "Water.hh"

const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 900;

const vec4 waterPlane(0, -1, 0, 5);

std::unique_ptr<Terrain> terrain;
std::unique_ptr<Skybox> sky;
std::unique_ptr<Water> water;
std::unique_ptr<Camera> cam;
std::unique_ptr<FrameBuffer> reflectionFBO;
std::unique_ptr<FrameBuffer> refractionFBO;
std::unique_ptr<ScreenFill> reflectionDebug;
std::unique_ptr<ScreenFill> refractionDebug;

mat4 camMatrix;

bool onGround { false };

bool mouseInit { false };
int mousex;
int mousey;
int buttonState = GLUT_UP;

void bindDefaultFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void init() {
    /* INIT GL */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    printError("GL inits");

    /* CREATE CAMERA */
    cam = std::make_unique<Camera>();

    // Create screen fill quad
    reflectionDebug = std::make_unique<ScreenFill>(0.0, 0.0, 0.25, 0.25);
    refractionDebug = std::make_unique<ScreenFill>(0.25, 0.0, 0.25, 0.25);

    /* SETUP FRAME BUFFERS */
    reflectionFBO = std::make_unique<FrameBuffer>(SCREEN_HEIGHT / 2,
                                                  SCREEN_WIDTH / 2, false);
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
    glUniform1i(glGetUniformLocation(waterShader, "dudv"), 3);

    GLuint debugShader = loadShaders("assets/shaders/debug.vert", "assets/shaders/debug.frag");

    reflectionDebug->setShader(debugShader);
    refractionDebug->setShader(debugShader);
    reflectionDebug->addTexture(reflectionFBO->texture);
    refractionDebug->addTexture(refractionFBO->texture);

    reflectionDebug->setShader(debugShader);
    refractionDebug->setShader(debugShader);
    reflectionDebug->addTexture(reflectionFBO->texture);
    refractionDebug->addTexture(refractionFBO->texture);

    printError("ERROR: SETUP PROGRAMS");

    /* SETUP TEXTURES */
    GLuint grassTex, dirtTex, skyTex, dudvTex;
    LoadTGATextureSimple("assets/textures/grass2_1024.tga", &grassTex);
    LoadTGATextureSimple("assets/textures/dirt2_1024.tga", &dirtTex);
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);
    LoadTGATextureSimple("assets/textures/waterDUDV.tga", &dudvTex);

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
    water->generate(100.0, waterPlane.w, 100.0, 75.0, 75.0);
    water->setShader(waterShader);
    water->addTexture(reflectionFBO->texture);
    water->addTexture(refractionFBO->texture);
    water->addTexture(refractionFBO->depth);
    water->addTexture(dudvTex);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // bindDefaultFramebuffer();
    sky->draw(camCopy);
    terrain->draw(camCopy, waterPlane * -1);

    // Scene
    bindDefaultFramebuffer();
    sky->draw(*cam);
    terrain->draw(*cam, vec4(0, 1, 0, 1e6));
    water->draw(*cam);

    // reflectionDebug->draw();
    // refractionDebug->draw();

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
    if (glutKeyIsDown('l')) onGround = !onGround;

    cam->updatePos(mx, mz);

    if (onGround) {
        float h = terrain->height(cam->x(), cam->z());
        cam->y() = h + 1.86;
    }

    cam->updateCamMatrix();
}

void timer(int i) {
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
    updateCam();
}

void mouseButton(int button, int state, int x, int y) { buttonState = state; }

void mouseMove(int x, int y) {
    if (buttonState == GLUT_DOWN && mouseInit) {
        float dx = (x - mousex) / 200.f;
        float dy = (y - mousey) / 200.f;
        cam->rotateX(dx);
        cam->rotateY(dy);
    }

    mousex = x;
    mousey = y;
    mouseInit = true;
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