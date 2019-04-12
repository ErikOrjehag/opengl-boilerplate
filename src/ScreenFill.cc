#include "ScreenFill.hh"

ScreenFill::ScreenFill(float x, float y, float width, float height) 
        : x{x}, y{y}, width{width}, height{height} {

    transform = T(x*2, -y*2, 0) * T(width, -height, 0) * T(-1, 1, 0.0) * S(width, height, 1.0);

    int vertexCount = 6;
    int triangleCount = 2;

    GLfloat vertexArray[] = { // First triangle
                              -1, -1, 0,
                              -1,  1, 0,
                               1, -1, 0,
                              // Second triangle
                               1, -1, 0,
                              -1,  1, 0,
                               1,  1, 0,};

    GLuint indexArray[] { 5, 4, 3, 2, 1, 0 };

    // Create Model and upload to GPU:
    model = *LoadDataToModel(vertexArray, NULL, NULL, NULL, indexArray,
                             vertexCount, triangleCount * 3);
}

void ScreenFill::addTexture(GLuint texture) {
    textures.push_back(texture);
}

void ScreenFill::setShader(GLuint shader) {
    this->shader = shader;
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(this->shader, "tex0"), 0);
}


void ScreenFill::draw() {

    glUseProgram(shader);

    for (size_t i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }


    glUniformMatrix4fv(glGetUniformLocation(shader, "transform"), 1, GL_TRUE,
                       transform.m);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    DrawModel(&model, shader, "inPosition", NULL, NULL);

}