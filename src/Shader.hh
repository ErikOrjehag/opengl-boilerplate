#pragma once
#include <iostream>
#include <string>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

class Shader {
   public:
    Shader() = default;
    Shader(GLuint shader);

    void activate();

    void upload(const std::string &uniform, int value);
    void upload(const std::string &uniform, float value);
    void upload(const std::string &uniform, const vec3 &vector);
    void upload(const std::string &uniform, const vec4 &vector);
    void upload(const std::string &uniform, const mat4 &matrix);

    bool initialized { false };

    GLuint program;
    bool hasNormals { true };
    bool hasTextureCoords { true };
};