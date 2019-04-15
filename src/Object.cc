#include "Object.hh"

Object::Object() : toWorld { IdentityMatrix() } {}

void Object::setShader(Shader shader) { this->shader = shader; }

void Object::addTexture(GLuint texture) { textures.push_back(texture); }

void Object::draw(const Camera &cam, std::optional<vec4> plane) {
    shader.activate();

    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    if (plane) {
        shader.upload("plane", plane.value());
    }

    shader.upload("projection", cam.projectionMatrix);
    shader.upload("modelToWorld", toWorld);
    shader.upload("worldToView", cam.camMatrix);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    prepareDraw(cam);

    int ok = DrawModel(&model, shader.program, "inPosition",
                       shader.hasNormals ? "inNormal" : NULL,
                       shader.hasTextureCoords ? "inTexCoord" : NULL);

    if (ok == 0) {
        std::cerr << "Error in model " << this->modelname << std::endl;
    }
}

void Object::prepareDraw(const Camera &cam) {}

void Object::loadModel(const std::string &modelName) {
    model = *LoadModelPlus(modelName.c_str());
    this->modelname = modelName;
}