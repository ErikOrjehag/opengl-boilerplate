#include "Skybox.hh"

Skybox::Skybox() : Object() {}

void Skybox::_draw(const Camera &cam) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // toWorld = Ry(0.0001) * toWorld;

    DrawModel(&model, shader, "inPosition", NULL, "inTexCoord");
}