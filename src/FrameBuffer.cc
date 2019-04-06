#include "FrameBuffer.hh"

FrameBuffer::FrameBuffer(int width, int height, bool needDepthTexture) : width(width), height(height) {
    // Create frame buffer
    glGenFramebuffers(1, &frame);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // Create texture attachment
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    if (needDepthTexture) {
        // Create depth texture attachment
        glGenTextures(1, &depth);
        glBindTexture(GL_TEXTURE_2D, depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth, 0);
    } else {
        // Create depth buffer attachment
        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    }
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &frame);
}

void FrameBuffer::bind() {
    glBindTexture(GL_TEXTURE_2D, 0); // To make sure the texture isnt bound
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glViewport(0, 0, width, height);
}
