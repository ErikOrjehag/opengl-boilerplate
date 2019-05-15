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
    // LoadTGATextureSimple("assets/textures/waterDUDV.tga", &dudvTex);
    LoadTGATextureSimple("assets/textures/dudv.tga", &dudvTex);

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

    auto add_sphere = [&](float x, float z) {
        vec3 pos { x, terrain->height(x, z), z };
        std::shared_ptr<Object> sphere = std::make_shared<Object>();
        sphere->setPosition(pos);
        spheres.push_back(sphere);
    };

    Shader objectShaderTextured { loadShaders(
        "assets/shaders/object_textured.vert",
        "assets/shaders/object_textured.frag") };

    // add_sphere(100, 220);
    // add_sphere(100, 210);
    // add_sphere(110, 220);
    // add_sphere(110, 230);
    // add_sphere(110, 240);
    // add_sphere(110, 250);

    float start_x = 90;
    float start_z = 190;
    float dist = 6;
    int nr_x = 4;
    int nr_z = 4;

    // for (int i = 0; i < nr_z; ++i) {
    //     for (int j = 0; j < nr_x; ++j) {
    //         if (j == 0 || j == nr_z - 1 || i == 0 || i == nr_x - 1) {
    //             add_sphere(start_x + j * dist, start_z + i * dist);
    //         }
    //     }
    // }

    for (int i = 0; i < nr_z; ++i) {
        for (int j = 0; j < nr_x; ++j) {
            add_sphere(start_x + j * dist, start_z + i * dist);
        }
    }

    // Sphere in water
    add_sphere(137, 136);

    GLuint sphereTexture;
    LoadTGATextureSimple("assets/textures/waterDUDV.tga", &sphereTexture);

    for (auto &obj : spheres) {
        obj->setShader(objectShaderTextured);
        obj->loadModel("assets/models/orb.obj");
        obj->addTexture(sphereTexture);
        // obj->useTexCoord = false;
        obj->setScaling({ 10, 10, 10 });
    }
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
/*************
 * RENDERING *
 *************/

void Render::renderWaterFBO() {
    // Refraction
    refractionFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    terrain->draw(*cam, waterPlane);
    vec4 offsetWaterPlane = waterPlane;
    offsetWaterPlane.w += 0.0;
    for (auto &obj : spheres) obj->draw(*cam, offsetWaterPlane);

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
    for (auto &obj : spheres) obj->draw(camCopy, waterPlane * -1);
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

void Render::checkSphereCollisions() {
    for (uint i = 0; i < spheres.size() - 1; ++i) {
        for (uint j = i + 1; j < spheres.size(); ++j) {
            std::shared_ptr<Object> first = spheres[i];
            std::shared_ptr<Object> second = spheres[j];

            if (first->colliding(*second)) {
                vec3 new_vel_first = first->forceVector(*second) *
                                     second->getSpeed() *
                                     Object::elasticity_constant;

                vec3 new_vel_second = second->forceVector(*first) *
                                      first->getSpeed() *
                                      Object::elasticity_constant;

                new_vel_first.y = 0;
                new_vel_second.y = 0;

                first->setVelocity(new_vel_first);
                second->setVelocity(new_vel_second);
            }
        }
    }

    for (auto &obj : spheres) {
        if (obj->colliding(*cam)) {
            vec3 new_vel = obj->forceVector(*cam) * cam->camSpeed *
                           Object::elasticity_constant;
            new_vel.y = 0;
            obj->setVelocity(new_vel);
            obj->placeAtCamEdge(*cam);
        }

        obj->position.y =
            terrain->height(obj->position.x, obj->position.z) + 1.75;
        obj->updatePostion();
    }
}

void Render::renderObjects() {
    // Sphere at sun
    sphereObject->toWorld =
        T(sunPosWorld.x, sunPosWorld.y, sunPosWorld.z) * S(15, 15, 15);
    sphereObject->draw(*cam);

    // Colliding spheres
    checkSphereCollisions();
    for (auto &obj : spheres) obj->draw(*cam, vec4(0, 1, 0, 1e6));
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
    if (shouldRenderHUD) renderHUD();

    glutSwapBuffers();
    printError("ERROR: DRAW");
}