#include "Object.hh"

Object::Object(bool depthTest) :
    depthTest{depthTest}, 
    toWorld{IdentityMatrix()}
{

}

void Object::setShader(GLuint shader) {
    this->shader = shader;
}

void Object::addTexture(GLuint texture) {
    textures.push_back(texture);
}

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

    glUniformMatrix4fv(glGetUniformLocation(shader, "modelToWorld"), 1, GL_TRUE, toWorld.m);
    glUniformMatrix4fv(glGetUniformLocation(shader, "worldToView"), 1, GL_TRUE, camMatrix.m);

    bool hasNormals = model.normalArray != NULL;
    bool hasTexture = model.texCoordArray != NULL;

    DrawModel(&model, shader, "inPosition", (hasNormals ? "inNormal" : NULL), (hasTexture ? "inTexCoord" : NULL));

    // Be a good guy
    glEnable(GL_DEPTH_TEST);
}