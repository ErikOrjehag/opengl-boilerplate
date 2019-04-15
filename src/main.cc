
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
std::unique_ptr<FrameBuffer> sunFBO;
std::unique_ptr<ScreenFill> reflectionDebug;
std::unique_ptr<ScreenFill> refractionDebug;
std::unique_ptr<ScreenFill> depthDebug;
std::unique_ptr<ScreenFill> sunDebug;
std::unique_ptr<Object> sphereObject;

mat4 camMatrix;

bool onGround { false };

bool mouseInit { false };
int mousex;
int mousey;
int buttonState = GLUT_UP;

GLuint depth;

GLuint blackShader;
GLuint blackSkyTex;

void bindDefaultFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void initCamera() {
    /* CREATE CAMERA */
    cam = std::make_unique<Camera>();
    printError("Init camera");
}

void initGodrays() {
    blackShader =
        loadShaders("assets/shaders/black.vert", "assets/shaders/black.frag");
    LoadTGATextureSimple("assets/textures/sky_black.tga", &blackSkyTex);
    GLuint godShader =
        loadShaders("assets/shaders/debug.vert", "assets/shaders/godray.frag");
    sunFBO = std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, false);
    sunDebug = std::make_unique<ScreenFill>(0.0, 0.25, 0.25, 0.25);
    sunDebug->setShader(godShader);
    sunDebug->addTexture(sunFBO->texture);
}

void initSkybox() {
    GLuint skyShader =
        loadShaders("assets/shaders/sky.vert", "assets/shaders/sky.frag");
    glUseProgram(skyShader);
    glUniform1i(glGetUniformLocation(skyShader, "sky"), 0);

    GLuint skyTex;
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);

    sky = std::make_unique<Skybox>();
    sky->setShader(skyShader);
    sky->loadModel("assets/models/skybox.obj");
    sky->addTexture(skyTex);
    printError("Init skybox");
}

void initTerrain() {
    GLuint terrainShader = loadShaders("assets/shaders/terrain.vert",
                                       "assets/shaders/terrain.frag");
    glUseProgram(terrainShader);
    glUniform1i(glGetUniformLocation(terrainShader, "grass"), 0);
    glUniform1i(glGetUniformLocation(terrainShader, "dirt"), 1);

    GLuint grassTex, dirtTex;
    LoadTGATextureSimple("assets/textures/grass2_1024.tga", &grassTex);
    LoadTGATextureSimple("assets/textures/dirt2_1024.tga", &dirtTex);

    terrain = std::make_unique<Terrain>();
    terrain->generate("assets/textures/terrain.tga");
    terrain->setShader(terrainShader);
    terrain->addTexture(grassTex);
    terrain->addTexture(dirtTex);
    printError("Init terrain");
}

void initWater() {
    reflectionDebug = std::make_unique<ScreenFill>(0.0, 0.0, 0.25, 0.25);
    refractionDebug = std::make_unique<ScreenFill>(0.25, 0.0, 0.25, 0.25);

    /* SETUP FRAME BUFFERS */
    reflectionFBO = std::make_unique<FrameBuffer>(SCREEN_HEIGHT / 2,
                                                  SCREEN_WIDTH / 2, false);
    refractionFBO =
        std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, true);

    GLuint waterShader =
        loadShaders("assets/shaders/water.vert", "assets/shaders/water.frag");
    glUseProgram(waterShader);
    glUniform1i(glGetUniformLocation(waterShader, "reflection"), 0);
    glUniform1i(glGetUniformLocation(waterShader, "refraction"), 1);
    glUniform1i(glGetUniformLocation(waterShader, "depth"), 2);
    glUniform1i(glGetUniformLocation(waterShader, "dudv"), 3);

    /* FBO DEBUGGING */
    GLuint debugShader =
        loadShaders("assets/shaders/debug.vert", "assets/shaders/debug.frag");

    reflectionDebug->setShader(debugShader);
    reflectionDebug->addTexture(reflectionFBO->texture);

    refractionDebug->setShader(debugShader);
    refractionDebug->addTexture(refractionFBO->texture);

    GLuint dudvTex;
    LoadTGATextureSimple("assets/textures/waterDUDV.tga", &dudvTex);

    water = std::make_unique<Water>();
    water->generate(100.0, waterPlane.w, 100.0, 75.0, 75.0);
    water->setShader(waterShader);
    water->addTexture(reflectionFBO->texture);
    water->addTexture(refractionFBO->texture);
    water->addTexture(refractionFBO->depth);
    water->addTexture(dudvTex);
    printError("Init water");
}

void initObjects() {
    sphereObject = std::make_unique<Object>();
    GLuint objectShader =
        loadShaders("assets/shaders/object.vert", "assets/shaders/object.frag");
    sphereObject->setShader(objectShader);
    sphereObject->loadModel("assets/models/orb.obj");
    sphereObject->toWorld = S(10, 10, 10) * T(10, 10, 10);

    sphereObject->useTexCoord = false;
}

void initDebug() {
    depthDebug = std::make_unique<ScreenFill>(0.5, 0.0, 0.25, 0.25);

    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCREEN_WIDTH,
                 SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLuint debugDepthShader = loadShaders("assets/shaders/debug.vert",
                                          "assets/shaders/debug-depth.frag");

    depthDebug->setShader(debugDepthShader);
    depthDebug->addTexture(depth);
    printError("Init debug");
}

void init() {
    /* INIT GL */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    printError("GL inits");

    initCamera();
    initGodrays();
    initTerrain();
    initSkybox();
    initWater();
    initDebug();
    initObjects();
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
    sky->draw(camCopy);
    terrain->draw(camCopy, waterPlane * -1);

    // Godrays
    vec3 sunDir = { 1, -1.12, 0.58 };
    vec3 sunPosWorld = cam->camPos - sunDir * 50;
    vec4 sunPosScreen =
        cam->projectionMatrix * cam->camMatrix * vec3tovec4(sunPosWorld);
    sunPosScreen /= sunPosScreen.w;

    sunFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Skybox blackSky = *sky;
    blackSky.textures.clear();
    blackSky.textures.push_back(blackSkyTex);
    Terrain blackTerrain = *terrain;
    blackTerrain.textures.clear();
    blackTerrain.setShader(blackShader);
    blackSky.draw(*cam);
    blackTerrain.draw(*cam);

    // Scene
    bindDefaultFramebuffer();
    sky->draw(*cam);
    terrain->draw(*cam, vec4(0, 1, 0, 1e6));
    water->draw(*cam);

    // Objects
    sphereObject->toWorld =
        T(sunPosWorld.x, sunPosWorld.y, sunPosWorld.z) * S(15, 15, 15);
    sphereObject->draw(*cam);

    // Depth
    glReadBuffer(GL_BACK);
    glBindTexture(GL_TEXTURE_2D, depth);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, SCREEN_WIDTH,
                     SCREEN_HEIGHT, 0);

    // Debug HUD
    reflectionDebug->draw();
    refractionDebug->draw();
    depthDebug->draw();
    sunDebug->draw();

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