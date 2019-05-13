#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

#include "Camera.hh"
#include "FrameBuffer.hh"
#include "ScreenFill.hh"
#include "Shader.hh"
#include "Skybox.hh"
#include "Terrain.hh"
#include "Water.hh"

struct Render {
    const int SCREEN_WIDTH = 1800;
    const int SCREEN_HEIGHT = 900;

    void bindDefaultFramebuffer();

    /* INIT SCENE */
    void initCamera();
    void initGodrays();
    void initSkybox();
    void initTerrain();
    void initWater();
    void initObjects();
    void initDebug();
    void init();

    /* RENDER FUNCTIONS */
    void renderWaterFBO();
    void renderGodRays();
    void renderEnvironment();
    void renderObjects();
    void renderHUD();
    void display();

    /* COLLISION */
    void checkSphereCollisions();

    std::unique_ptr<Camera> cam;
    std::unique_ptr<Terrain> terrain;
    std::unique_ptr<Skybox> sky;
    std::unique_ptr<Water> water;
    std::unique_ptr<FrameBuffer> reflectionFBO;
    std::unique_ptr<FrameBuffer> refractionFBO;
    std::unique_ptr<FrameBuffer> sunFBO;
    std::unique_ptr<ScreenFill> reflectionDebug;
    std::unique_ptr<ScreenFill> refractionDebug;
    std::unique_ptr<ScreenFill> depthDebug;
    std::unique_ptr<ScreenFill> sunDebug;
    std::unique_ptr<Object> sphereObject;

    std::vector<std::shared_ptr<Object>> spheres {};

    const vec4 waterPlane { 0, -1, 0, 5 };

    mat4 camMatrix;
    GLuint depth;
    Shader blackShader {};
    GLuint blackSkyTex;

    // TODO place somewhere nice :)
    vec3 sunPosWorld;
};