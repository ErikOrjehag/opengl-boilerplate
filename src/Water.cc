#include "Water.hh"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "MicroGlut.h"
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

void Water::prepareDraw(const Camera &cam) {
    float timeScale = 0.00001;

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    float moveOffset = std::fmod(timeScale * t, 1.0);

    shader.upload("waveOffset", moveOffset);
    shader.upload("inCamera", cam.camPos);
}