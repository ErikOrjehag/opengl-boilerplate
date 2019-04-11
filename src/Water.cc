#include "Water.hh"
#include <algorithm>
#include <iostream>
#include "VectorUtils3.h"

Water::Water() {}

void Water::generate(float x, float y, float z, float width, float height) {
    toWorld = T(x, y, z) * S(width, 0, height);

    int vertexCount = 6;
    int triangleCount = 2;

    GLfloat vertexArray[] = { -1, 0, -1, -1, 0, 1, 1, 0, -1,
                              1,  0, -1, -1, 0, 1, 1, 0, 1 };

    GLuint indexArray[] { 0, 1, 2, 3, 4, 5 };

    // Create Model and upload to GPU:

    model = *LoadDataToModel(vertexArray, NULL, NULL, NULL, indexArray,
                             vertexCount, triangleCount * 3);
}

void Water::_draw(const Camera &cam) {
    // glUniform3fv(glGetUniformLocation(shader, "inCamera"), 1,
    // &(cam.camPos.x));
    DrawModel(&model, shader, "inPosition", NULL, NULL);
}