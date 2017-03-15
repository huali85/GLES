//
// Created by lihua_000 on 3/4/2017.
//

#include "FrameBuffer.h"

bool FrameBuffer::CreateShadowMap(int width, int height, FrameBuffer& fb) {
    // Create frame buffer object
    GL_CHECK(glGenFramebuffers(1, &fb.fbo));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo));
    // Create depth texture
    GL_CHECK(glGenTextures(1, &fb.depth_texture));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, fb.depth_texture));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    // Bind depth texture
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb.depth_texture, 0));
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Failed to create frame buffer: 0x%x\n", status);
        return false;
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return true;
}

bool FrameBuffer::CreateGBuffer(int width, int height, FrameBuffer& gb) {
    // Create frame buffer object
    GL_CHECK(glGenFramebuffers(1, &gb.fbo));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, gb.fbo));
    // Create gbuffer textures
    gb.color_textures = new GLuint[4];
    gb.num_color_textures = 4;
    GL_CHECK(glGenTextures(gb.num_color_textures, gb.color_textures));
    for (unsigned int i = 0; i < gb.num_color_textures; ++i) {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, gb.color_textures[i]));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0));
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gb.color_textures[i], 0));
    }
    // Create depth texture
    GL_CHECK(glGenTextures(1, &gb.depth_texture));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, gb.depth_texture));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0));
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gb.depth_texture, 0));

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Failed to create frame buffer: 0x%x\n", status);
        return false;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return true;
}

void FrameBuffer::Destroy(FrameBuffer &fb) {
    if (fb.fbo) {
        glDeleteTextures(fb.num_color_textures, fb.color_textures);
        glDeleteTextures(1, &fb.depth_texture);
        glDeleteFramebuffers(1, &fb.fbo);
        delete[] fb.color_textures;
        fb.color_textures = nullptr;
        fb.num_color_textures = 0;
        fb.fbo = 0;
    }
}