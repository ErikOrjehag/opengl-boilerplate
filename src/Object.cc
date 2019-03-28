#include "Object.hh"

Object::Object() : 
    toWorld(IdentityMatrix()) 
{

}

void Object::setProgram(GLuint program) {
    this->program = program;
}

void Object::addTexture(GLuint texture) {
    textures.push_back(texture);
}

void Object::draw(mat4 camMatrix) {
    glUseProgram(program);

    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    glUniformMatrix4fv(glGetUniformLocation(program, "modelToWorld"), 1, GL_TRUE, toWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "worldToView"), 1, GL_TRUE, camMatrix.m);
    DrawModel(&model, program, "inPosition", "inNormal", "inTexCoord");
}