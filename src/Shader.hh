#pragma once
#include <string>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

class Shader {
   public:
    Shader() = delete;
    Shader(GLuint shader);

    void activate();

    void upload(const std::string &uniform, float value);
    void upload(const std::string &uniform, const vec3 &vector);
    void upload(const std::string &uniform, const mat4 &matrix);

    GLuint shader;
    bool hasNormals { true };
    bool hasTextureCoords { true };
};