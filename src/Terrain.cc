#include "Terrain.hh"
#include <algorithm>
#include <iostream>
#include "VectorUtils3.h"

void Terrain::generate(std::string terrain_file) {
    LoadTGATextureData(terrain_file.c_str(), &tex);

    int vertexCount = tex.width * tex.height;
    int triangleCount = (tex.width - 1) * (tex.height - 1) * 2;
    int x, z;

    GLfloat* vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat* normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
    GLfloat* texCoordArray =
        (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
    GLuint* indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount * 3);

    for (x = 0; x < tex.width; x++) {
        for (z = 0; z < tex.height; z++) {
            float s11 = texHeight(x, z);
            float s01 = texHeight(x - 1, z);
            float s21 = texHeight(x + 1, z);
            float s10 = texHeight(x, z - 1);
            float s12 = texHeight(x, z + 1);

            vec3 va = Normalize(SetVector(2.0, 0.0, -(s21 - s01)));
            vec3 vb = Normalize(SetVector(0.0, 2.0, (s12 - s10)));
            vec3 normal = Normalize(CrossProduct(va, vb));

            // Vertex array. You need to scale this properly
            vertexArray[(x + z * tex.width) * 3 + 0] = x / 1.0;
            vertexArray[(x + z * tex.width) * 3 + 1] = s11;
            vertexArray[(x + z * tex.width) * 3 + 2] = z / 1.0;
            // Normal vectors. You need to calculate these.
            normalArray[(x + z * tex.width) * 3 + 0] = normal.x;
            normalArray[(x + z * tex.width) * 3 + 1] = normal.y;
            normalArray[(x + z * tex.width) * 3 + 2] = normal.z;
            // Texture coordinates. You may want to scale them.
            texCoordArray[(x + z * tex.width) * 2 + 0] =
                x;  // (float)x / tex.width;
            texCoordArray[(x + z * tex.width) * 2 + 1] =
                z;  // (float)z / tex.height;
        }
    }
    for (x = 0; x < tex.width - 1; x++) {
        for (z = 0; z < tex.height - 1; z++) {
            // Triangle 1
            indexArray[(x + z * (tex.width - 1)) * 6 + 0] = x + z * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 1] =
                x + (z + 1) * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 2] =
                x + 1 + z * tex.width;
            // Triangle 2
            indexArray[(x + z * (tex.width - 1)) * 6 + 3] =
                x + 1 + z * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 4] =
                x + (z + 1) * tex.width;
            indexArray[(x + z * (tex.width - 1)) * 6 + 5] =
                x + 1 + (z + 1) * tex.width;
        }
    }

    // End of terrain generation

    // Create Model and upload to GPU:

    model = *LoadDataToModel(vertexArray, normalArray, texCoordArray, NULL,
                             indexArray, vertexCount, triangleCount * 3);
}

float Terrain::texHeight(int x, int z) {
    return tex.imageData[(std::max(0, std::min<int>(tex.width - 1, x)) +
                          std::max(0, std::min<int>(tex.height - 1, z)) *
                              tex.width) *
                         (tex.bpp / 8)] /
           10.0;
}

float Terrain::height(float xx, float zz) {
    int x = xx;
    int z = zz;

    // First triangle
    float v1x = x, v1z = z;
    float v2x = x, v2z = z + 1;
    float v3x = x + 1, v3z = z;

    // Second triangle
    if (1 - (xx - x) < (zz - z)) {
        v1x = x + 1;
        v1z = z;
        // x2 same
        v3x = x + 1;
        v3z = z + 1;
    }

    float h1 = texHeight(v1x, v1z);
    float h2 = texHeight(v2x, v2z);
    float h3 = texHeight(v3x, v3z);

    // Barycentric coordinates
    // https://codeplea.com/triangular-interpolation
    float d = ((v2z - v3z) * (v1x - v3x) + (v3x - v2x) * (v1z - v3z));
    float w1 = ((v2z - v3z) * (xx - v3x) + (v3x - v2x) * (zz - v3z)) / d;
    float w2 = ((v3z - v1z) * (xx - v3x) + (v1x - v3x) * (zz - v3z)) / d;
    float w3 = 1 - w1 - w2;

    float h = (w1 * h1 + w2 * h2 + w3 * h3) / (w1 + w2 + w3);

    return h;
}