#pragma once

#include "GL_utilities.h"

class FrameBuffer {
   public:
    FrameBuffer(int width, int height, bool needDepthTexture);
    ~FrameBuffer();

    void bind();

    GLuint texture;
    GLuint depth;

   private:
    int width;
    int height;
    GLuint frame;
};