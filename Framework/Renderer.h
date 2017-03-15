//
// Created by lihua_000 on 3/8/2017.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include <glm/glm.hpp>

class Renderer {
private:
    int _width;
    int _height;

    GLuint _fullscreen_program;
    GLuint _fullscreen_quad_buffers[2];

    FrameBuffer_t _framebuffer;

    glm::mat4 _proj;
    glm::mat4 _view;



};

#endif //RENDERER_H
