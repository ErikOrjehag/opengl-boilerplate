#pragma once
#include "loadobj.h"
#include "VectorUtils3.h"

class Object {

    public:
    Object();
    virtual ~Object() = default;

    void setProgram(GLuint program);
    void draw();

    protected:
    Model* model = NULL;
    mat4 toWorld;
    GLuint program;

};