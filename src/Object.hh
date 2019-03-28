#pragma once
#include "loadobj.h"
#include "VectorUtils3.h"
#include <vector>

class Object {

    public:
    Object();
    virtual ~Object() = default;

    void setProgram(GLuint program);
    void addTexture(GLuint texture);
    void draw(mat4 cam);

    protected:
    Model model;
    mat4 toWorld;
    GLuint program;
    std::vector<GLuint> textures;

};