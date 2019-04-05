#pragma once

#include "GL_utilities.h"

class FrameBuffer {
   public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void bind();
    void unbind();

   private:
    int createFrameBuffer();
    int createTextureAttachement();
    int createDepthTextureAttachment();
    int createDepthBufferAttachment();

    int width;
    int height;
    int frameBuffer;
    int depthBuffer;
    int texture;
};