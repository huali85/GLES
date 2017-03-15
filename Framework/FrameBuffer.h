//
// Created by lihua_000 on 3/4/2017.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Platform.h"

namespace FrameBuffer
{
    struct FrameBuffer {
        GLuint fbo = 0;
        GLuint depth_texture = 0;
        GLuint* color_textures = nullptr;
        unsigned int num_color_textures = 0;
        //FrameBuffer() : fbo(0), depth_texture(0), color_textures(nullptr), num_color_textures(0) {}
    };

    bool CreateShadowMap(int width, int height, FrameBuffer& fb);
    bool CreateGBuffer(int width, int height, FrameBuffer& gb);
    void Destroy(FrameBuffer& fb);
}

typedef FrameBuffer::FrameBuffer FrameBuffer_t;

#endif //FRAMEBUFFER_H
