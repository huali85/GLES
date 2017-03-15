//
// Created by lihua_000 on 3/1/2017.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Platform.h"

namespace Texture
{
    bool CreateFromFile(const char* filename, GLuint& obj);
    bool CreateFromRawFile(const char* filename, int width, int height, GLuint& obj);
    bool CreateFromMemory(GLubyte* data, int width, int height, GLuint& obj);
    void Bind(GLenum texture_unit, GLuint texture_obj);
    void Destroy(GLuint& texture_obj);
}

#endif //TEXTURE_H
