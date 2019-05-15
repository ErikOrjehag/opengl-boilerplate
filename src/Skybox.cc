#include "Skybox.hh"

Skybox::Skybox() : Object() {}

void Skybox::prepareDraw(const Camera &cam) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glDisable(GL_CLIP_PLANE0);

    // toWorld = Ry(0.0001) * toWorld;
}