//
// Created by lihua_000 on 3/15/2017.
//

#ifndef UTIL_H
#define UTIL_H

#include "Platform.h"

namespace Util
{
    class OverlayTexture {
    public:
        ~OverlayTexture();

        static OverlayTexture* CreateOverlayTexture(float sx, float sy, float width, float height);
        void Draw(GLuint texture);

    private:
        OverlayTexture();

        GLuint _shader;
        GLint  _screeSize;
        GLint  _texture;

        GLuint _vao;
        GLuint _vbo;

        float _screen_width;
        float _screen_height;
    };
}

#endif //UTIL_H
