#include "Object.hh"

Object::Object() : toWorld { IdentityMatrix() } {}

void Object::setShader(GLuint shader) { this->shader = shader; }

void Object::addTexture(GLuint texture) { textures.push_back(texture); }

void Object::draw(const Camera &cam, std::optional<vec4> plane) {
    glUseProgram(shader);

    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    if (plane) {
        glUniform4fv(glGetUniformLocation(shader, "plane"), 1,
                     &(plane.value().x));
    }

    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_TRUE,
                       cam.projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "modelToWorld"), 1, GL_TRUE,
                       toWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "worldToView"), 1, GL_TRUE,
                       cam.camMatrix.m);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    _draw(cam);
}

void Object::_draw(const Camera &cam) {
    DrawModel(&model, shader, "inPosition", "inNormal", "inTexCoord");
}

void Object::loadModel(const std::string &modelName) {
    model = *LoadModelPlus(modelName.c_str());
    modelname = modelName;
}