#include "Shader.hh"

Shader::Shader(GLuint shader) : initialized { true } { this->program = shader; }

void Shader::activate() {
    if (!this->initialized) {
        std::cerr << "Calling activate on un-initialized shader" << std::endl;
        throw 1;
    }
    glUseProgram(this->program);
}

void Shader::upload(const std::string &uniform, float value) {
    activate();
    glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
}

void Shader::upload(const std::string &uniform, int value) {
    activate();
    glUniform1i(glGetUniformLocation(program, uniform.c_str()), value);
}

void Shader::upload(const std::string &uniform, float a, float b) {
    activate();
    float tmp[2] = { a, b };
    glUniform2fv(glGetUniformLocation(program, uniform.c_str()), 1, tmp);
}

void Shader::upload(const std::string &uniform, const vec3 &vector) {
    activate();
    glUniform3fv(glGetUniformLocation(program, uniform.c_str()), 1,
                 &(vector.x));
}

void Shader::upload(const std::string &uniform, const vec4 &vector) {
    activate();
    glUniform4fv(glGetUniformLocation(program, uniform.c_str()), 1,
                 &(vector.x));
}

void Shader::upload(const std::string &uniform, const mat4 &matrix) {
    activate();
    glUniformMatrix4fv(glGetUniformLocation(program, uniform.c_str()), 1,
                       GL_TRUE, matrix.m);
}