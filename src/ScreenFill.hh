

#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "MicroGlut.h"
#include "Shader.hh"
#include "VectorUtils3.h"
#include "loadobj.h"

class ScreenFill {
   public:
    ScreenFill(float x, float y, float width, float height);

    void setShader(Shader shader);
    void addTexture(GLuint texture);
    void draw();

    mat4 transform {};

    Shader shader;

   private:
    float x, y, width, height;
    Model model;
    std::vector<GLuint> textures {};
};