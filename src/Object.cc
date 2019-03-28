#include "Object.hh"

Object::Object() : 
    toWorld(IdentityMatrix()) 
{

}

void Object::setProgram(GLuint program) {
    this->program = program;
}

void Object::draw(mat4 camMatrix) {
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE, toWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, camMatrix.m);
    DrawModel(&model, program, "inPosition", "inNormal", "inTexCoord");
}