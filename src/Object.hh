#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "Camera.hh"
#include "VectorUtils3.h"
#include "loadobj.h"

class Object {
   public:
    Object();
    virtual ~Object() = default;

    void setShader(GLuint shader);
    void addTexture(GLuint texture);
    void draw(const Camera &cam, std::optional<vec4> plane = std::nullopt);

    void loadModel(const std::string &modelName);

   protected:
    bool depthTest { true };
    bool cullFace { true };
    bool hasNormals { true };
    bool hasTexture { true };
    Model model;
    mat4 toWorld;
    GLuint shader;
    std::vector<GLuint> textures;

    // DEBUG
    std::string modelname { "UNNAMED OBJECT" };
};