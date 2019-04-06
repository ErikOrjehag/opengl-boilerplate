#include "Water.hh"
#include <algorithm>
#include <iostream>
#include "VectorUtils3.h"

Water::Water() {
    hasNormals = false;
    hasTexture = false;
}

void Water::generate(float x, float y, float z, float width, float height) {
    toWorld = T(x, y, z);

    int vertexCount = 4;
    int triangleCount = 2;

    GLfloat* vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLuint* indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount * 3);

    // Upper left
    vertexArray[0 * 3 + 0] = -width / 2.0;
    vertexArray[0 * 3 + 1] = 0.0;
    vertexArray[0 * 3 + 2] = height / 2.0;

    // Upper right
    vertexArray[1 * 3 + 0] = width / 2.0;
    vertexArray[1 * 3 + 1] = 0.0;
    vertexArray[1 * 3 + 2] = height / 2.0;

    // Lower left
    vertexArray[2 * 3 + 0] = -width / 2.0;
    vertexArray[2 * 3 + 1] = 0.0;
    vertexArray[2 * 3 + 2] = -height / 2.0;

    // Lower right
    vertexArray[3 * 3 + 0] = width / 2.0;
    vertexArray[3 * 3 + 1] = 0.0;
    vertexArray[3 * 3 + 2] = -height / 2.0;

    // Triangle 1
    indexArray[0] = 0;
    indexArray[1] = 1;
    indexArray[2] = 3;
    // Triangle 2
    indexArray[3] = 3;
    indexArray[4] = 2;
    indexArray[5] = 0;

    // Create Model and upload to GPU:

    model = *LoadDataToModel(vertexArray, NULL, NULL, NULL, indexArray,
                             vertexCount, triangleCount * 3);
}