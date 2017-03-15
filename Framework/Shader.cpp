//
// Created by lihua_000 on 3/1/2017.
//
#include "Shader.h"
#include <stdlib.h>

bool CompileShaderFromString(const char* source, GLuint handle) {
    glShaderSource(handle, 1, &source, NULL);
    glCompileShader(handle);
    // Check for compile success
    GLint result = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
    if (!result) {
        char info[512];
        glGetShaderInfoLog(handle, 512, NULL, info);
        LOGE("Compile shader failed: %s\n", info);
        return false;
    }
    return true;
}

bool CompileShaderFromFile(const char* filename, GLuint handle) {
    char* shader_source = nullptr;
    size_t shader_size;
    if (!LoadFile(filename, (void**)&shader_source, &shader_size)) {
        return false;
    }
    bool result = CompileShaderFromString(shader_source, handle);
    free(shader_source);
    return result;
}

bool LinkProgram(GLuint program) {
    glLinkProgram(program);
    GLint result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        LOGE("Link shaders failed: %s\n", info);
        return false;
    }
    return true;
}

bool Shader::CreateFromString(const char *vs_source, const char *fs_source, GLuint &obj) {
    // Create object handles
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile shaders
    if (!CompileShaderFromString(vs_source, vs)) {
        LOGE("Shader type: VERTEX_SHADER\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }
    if (!CompileShaderFromString(fs_source, fs)) {
        LOGE("Shader type: FRAGMENT_SHADER\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }
    // Attach shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    if (!LinkProgram(program)) {
        glDeleteProgram(program);
        return false;
    }
    obj = program;
    return true;
}

bool Shader::CreateFromFile(const char *vs_filename, const char *fs_filename, GLuint &obj) {
    // Create object handles
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile shaders
    if (!CompileShaderFromFile(vs_filename, vs)) {
        LOGE("Shader file: %s\n", vs_filename);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }
    if (!CompileShaderFromFile(fs_filename, fs)) {
        LOGE("Shader file: %s\n", fs_filename);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }
    // Attach shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    if (!LinkProgram(program)) {
        glDeleteProgram(program);
        return false;
    }
    obj = program;
    return true;
}

void Shader::Destroy(GLuint& shader) {
    if (shader) {
        glDeleteProgram(shader);
        shader = 0;
    }
}