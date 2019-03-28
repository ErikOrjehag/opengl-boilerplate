#pragma once
#include "loadobj.h"
#include "VectorUtils3.h"
#include <vector>

class Object {

    public:
    Object(bool depthTest = true);
    virtual ~Object() = default;

    void setShader(GLuint shader);
    void addTexture(GLuint texture);
    void draw(mat4 cam);

    protected:
    bool depthTest;
    Model model;
    mat4 toWorld;
    GLuint shader;
    std::vector<GLuint> textures;

};