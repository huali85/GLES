//
// Created by lihua_000 on 3/1/2017.
//

#ifndef SHADER_H
#define SHADER_H

#include "Platform.h"

namespace Shader
{
    bool CreateFromFile(const char* vs_filename, const char* fs_filename, GLuint& program);
    bool CreateFromString(const char* vs_source, const char* fs_source, GLuint& program);
    void Destroy(GLuint& shader);
}

#endif //SHADER_H
