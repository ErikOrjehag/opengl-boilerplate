#include "Render.hh"

void Render::bindDefaultFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Render::initCamera() {
    /* CREATE CAMERA */
    cam = std::make_unique<Camera>();
    printError("Init camera");
}

void Render::initGodrays() {
    Shader godShader { loadShaders("assets/shaders/debug.vert",
                                   "assets/shaders/godray.frag") };
    godShader.upload("light", 0);

    blackShader = Shader { loadShaders("assets/shaders/black.vert",
                                       "assets/shaders/black.frag") };

    blackShader.hasTextureCoords = false;
    blackShader.hasNormals = false;

    LoadTGATextureSimple("assets/textures/sky_black.tga", &blackSkyTex);

    sunFBO = std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, false);
    sunDebug = std::make_unique<ScreenFill>(0.0, 0.25, 0.25, 0.25);
    sunDebug->setShader(godShader);
    sunDebug->addTexture(sunFBO->texture);
    sunDebug->addTexture(depth);
}

void Render::initSkybox() {
    Shader skyShader { loadShaders("assets/shaders/sky.vert",
                                   "assets/shaders/sky.frag") };

    skyShader.hasNormals = false;

    skyShader.upload("sky", 0);

    GLuint skyTex;
    LoadTGATextureSimple("assets/textures/sky.tga", &skyTex);

    sky = std::make_unique<Skybox>();
    sky->setShader(skyShader);
    sky->loadModel("assets/models/skybox.obj");
    sky->addTexture(skyTex);
    printError("Init skybox");
}

void Render::initTerrain() {
    Shader terrainShader = Shader { loadShaders(
        "assets/shaders/terrain.vert", "assets/shaders/terrain.frag") };

    terrainShader.upload("grass", 0);
    terrainShader.upload("dirt", 1);

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

void Render::initWater() {
    reflectionDebug = std::make_unique<ScreenFill>(0.0, 0.0, 0.25, 0.25);
    refractionDebug = std::make_unique<ScreenFill>(0.25, 0.0, 0.25, 0.25);

    /* SETUP FRAME BUFFERS */
    reflectionFBO = std::make_unique<FrameBuffer>(SCREEN_HEIGHT / 2,
                                                  SCREEN_WIDTH / 2, false);
    refractionFBO =
        std::make_unique<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, true);

    Shader waterShader = Shader { loadShaders("assets/shaders/water.vert",
                                              "assets/shaders/water.frag") };

    waterShader.hasNormals = false;
    waterShader.hasTextureCoords = false;

    waterShader.upload("reflection", 0);
    waterShader.upload("refraction", 1);
    waterShader.upload("depth", 2);
    waterShader.upload("dudv", 3);

    /* FBO DEBUGGING */
    GLuint debugShader =
        loadShaders("assets/shaders/debug.vert", "assets/shaders/debug.frag");

    reflectionDebug->setShader(Shader { debugShader });
    reflectionDebug->addTexture(reflectionFBO->texture);

    refractionDebug->setShader(Shader { debugShader });
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

void Render::initObjects() {
    sphereObject = std::make_unique<Object>();
    Shader objectShader { loadShaders("assets/shaders/object.vert",
                                      "assets/shaders/object.frag") };

    objectShader.hasTextureCoords = false;

    sphereObject->setShader(objectShader);
    sphereObject->loadModel("assets/models/orb.obj");
    sphereObject->toWorld = S(10, 10, 10) * T(10, 10, 10);

    sphereObject->useTexCoord = false;
}

void Render::initDebug() {
    depthDebug = std::make_unique<ScreenFill>(0.5, 0.0, 0.25, 0.25);

    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCREEN_WIDTH,
                 SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    Shader debugDepthShader { loadShaders("assets/shaders/debug.vert",
                                          "assets/shaders/debug-depth.frag") };

    depthDebug->setShader(debugDepthShader);
    depthDebug->addTexture(depth);
    printError("Init debug");
}

void Render::init() {
    /* INIT GL */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    printError("GL inits");

    initCamera();
    initTerrain();
    initSkybox();
    initWater();
    initDebug();
    initObjects();
    initGodrays();
}

void Render::renderWaterFBO() {
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
}

void Render::renderGodRays() {
    // Sun position
    vec3 sunDir = { 1, -1.12, 0.58 };
    sunPosWorld = cam->camPos - sunDir * 50;
    vec4 sunPosScreen =
        cam->projectionMatrix * cam->camMatrix * vec3tovec4(sunPosWorld);
    sunPosScreen /= sunPosScreen.w;

    // Godrays
    sunDebug->shader.upload("source", sunPosScreen.x * 2 - 1,
                            sunPosScreen.y * 2 - 1);
    sunFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Skybox blackSky = *sky;
    blackSky.textures.clear();
    blackSky.textures.push_back(blackSkyTex);
    blackSky.draw(*cam);
    Shader tmp = terrain->shader;
    terrain->shader = blackShader;
    terrain->draw(*cam);
    terrain->shader = tmp;
}

void Render::renderEnvironment() {
    // Scene
    bindDefaultFramebuffer();
    sky->draw(*cam);
    terrain->draw(*cam, vec4(0, 1, 0, 1e6));
    water->draw(*cam);
}
void Render::renderObjects() {
    // Objects
    sphereObject->toWorld =
        T(sunPosWorld.x, sunPosWorld.y, sunPosWorld.z) * S(15, 15, 15);
    sphereObject->draw(*cam);
}
void Render::renderHUD() {
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
}

void Render::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderWaterFBO();
    renderGodRays();
    renderEnvironment();
    renderObjects();
    renderHUD();

    glutSwapBuffers();
    printError("ERROR: DRAW");
}