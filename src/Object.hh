#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Camera.hh"
#include "Shader.hh"
#include "VectorUtils3.h"
#include "loadobj.h"

class Object {
   public:
    Object();
    virtual ~Object() = default;

    void setShader(Shader shader);
    void addTexture(GLuint texture);
    void draw(const Camera &cam, std::optional<vec4> plane = std::nullopt);
    void loadModel(const std::string &modelName);
    mat4 toWorld;
    bool useTexCoord { true };
    bool useNormals { true };

    std::vector<GLuint> textures {};

    Shader shader;

   protected:
    virtual void prepareDraw(const Camera &cam);

    Model model;

    // DEBUG
    std::string modelname { "UNNAMED OBJECT" };
};