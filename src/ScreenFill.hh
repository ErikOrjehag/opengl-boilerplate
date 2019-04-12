

#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "MicroGlut.h"



class ScreenFill {

    public:

    ScreenFill(float x, float y, float width, float height);

    void setShader(GLuint shader);
    void addTexture(GLuint texture);
    void draw();


    mat4 transform{};


    private:
    float x, y, width, height;
    Model model;
    std::vector<GLuint> textures{};
    GLuint shader;
};