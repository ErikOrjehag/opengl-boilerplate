#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Camera.hh"
#include "VectorUtils3.h"
#include "loadobj.h"

class Object {
   public:
    Object(bool depthTest = true);
    virtual ~Object() = default;

    void setShader(GLuint shader);
    void addTexture(GLuint texture);
    void draw(const Camera &cam);

    void loadModel(const std::string &modelName);

   protected:
    bool depthTest;
    bool hasNormals { true };
    bool hasTexture { true };
    Model model;
    mat4 toWorld;
    GLuint shader;
    std::vector<GLuint> textures;

    // DEBUG
    std::string modelname { "UNNAMED OBJECT" };
};