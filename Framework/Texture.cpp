//
// Created by lihua_000 on 3/1/2017.
//
#include "Texture.h"
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLuint LoadSimpleTexture() {
    /* 3x3 image, RGBA raw format */
    GLubyte pixles[9 * 4] = {
            18,  140, 171, 255, /* Some Colour Bottom Left. */
            143, 143, 143, 255, /* Some Colour Bottom Middle. */
            255, 255, 255, 255, /* Some Colour Bottom Right. */
            255, 255, 0,   255, /* Yellow Middle Left. */
            0,   255, 255, 255, /* Some Colour Middle. */
            255, 0,   255, 255, /* Some Colour Middle Right. */
            255, 0,   0,   255, /* Red Top Left. */
            0,   255, 0,   255, /* Green Top Middle. */
            0,   0,   255, 255, /* Blue Top Right. */
    };
    /* Use tightly packed data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

bool Texture::CreateFromFile(const char *filename, GLuint &obj) {
    void* file_data = nullptr;
    size_t file_size;
    if (!LoadFile(filename, &file_data, &file_size)) {
        return false;
    }

    GLubyte* tex_data = nullptr;
    int width, height, components;
    //stbi_set_flip_vertically_on_load(true);
    tex_data = stbi_load_from_memory((unsigned char*)file_data, (int)file_size, &width, &height, &components, 0);
    if (!tex_data) {
        LOGE("Unknown texture: %s\n", filename);
        free(file_data);
        return false;
    }

    GLuint tex;
    GL_CHECK(glGenTextures(1, &tex));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GLenum format = GL_RGB;
    switch (components) {
        case 1: format = GL_LUMINANCE; break;
        case 2: format = GL_LUMINANCE_ALPHA; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: LOGE("Unknown data format: %s\n", filename); break;
    }
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, tex_data));
    GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(tex_data);
    free(file_data);
    obj = tex;
    return true;
}

bool Texture::CreateFromRawFile(const char *filename, int width, int height, GLuint &obj) {
    GLubyte* tex_data = nullptr;
    size_t tex_size;
    if (!LoadFile(filename, (void**)&tex_data, &tex_size)) {
        return false;
    }

    GLuint tex;
    GL_CHECK(glGenTextures(1, &tex));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data));
    free(tex_data);
    obj = tex;
    return true;
}

bool Texture::CreateFromMemory(GLubyte *data, int width, int height, GLuint &obj) {
    if (!data)
        return false;

    GLuint tex;
    GL_CHECK(glGenTextures(1, &tex));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    tex = obj;
    return true;
}

void Texture::Bind(GLenum texture_unit, GLuint texture_obj) {
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_obj);
}

void Texture::Destroy(GLuint& texture_obj) {
    if (texture_obj) {
        glDeleteTextures(1, &texture_obj);
        texture_obj = 0;
    }
}
