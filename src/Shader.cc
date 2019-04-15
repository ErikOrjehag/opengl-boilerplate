#include "Shader.hh"

Shader::Shader(GLuint shader) { this->shader = shader; }

void Shader::activate() { glUseProgram(this->shader); }

void Shader::upload(const std::string &uniform, float value) {
    activate();
    glUniform1f(glGetUniformLocation(shader, uniform.c_str()), value);
}
void Shader::upload(const std::string &uniform, const vec3 &vector) {
    activate();
    glUniform3fv(glGetUniformLocation(shader, uniform.c_str()), 1, &(vector.x));
}

void Shader::upload(const std::string &uniform, const mat4 &matrix) {
    activate();
    glUniformMatrix4fv(glGetUniformLocation(shader, uniform.c_str()), 1,
                       GL_TRUE, matrix.m);
}