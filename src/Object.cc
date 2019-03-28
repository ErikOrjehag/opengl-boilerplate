#include "Object.hh"

Object::Object(bool depthTest)
    : depthTest { depthTest }, toWorld { IdentityMatrix() } {}

void Object::setShader(GLuint shader) { this->shader = shader; }

void Object::addTexture(GLuint texture) { textures.push_back(texture); }

void Object::draw(mat4 camMatrix) {
    glUseProgram(shader);

    if (depthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    glUniformMatrix4fv(glGetUniformLocation(shader, "modelToWorld"), 1, GL_TRUE,
                       toWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "worldToView"), 1, GL_TRUE,
                       camMatrix.m);

    DrawModel(&model, shader, "inPosition", (hasNormals ? "inNormal" : NULL),
              (hasTexture ? "inTexCoord" : NULL));

    std::cout << "Name: " << modelname << " hasNormals: " << hasNormals
              << " hasTexture: " << hasTexture << " depthTest: " << depthTest
              << std::endl;

    // Be a good guy
    glEnable(GL_DEPTH_TEST);
}

void Object::loadModel(const std::string &modelName) {
    model = *LoadModelPlus(modelName.c_str());
    modelname = modelName;
}