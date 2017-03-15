//
// Created by lihua_000 on 3/15/2017.
//

#include "Util.h"
#include "Shader.h"
#include <math.h>
#include <glm/vec4.hpp>

namespace Util
{
    OverlayTexture::OverlayTexture() :
        _shader(0),
        _vao(0)
    { }

    OverlayTexture::~OverlayTexture() {
        if (_vao) {
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
        }
        Shader::Destroy(_shader);
    }

    OverlayTexture *
    OverlayTexture::CreateOverlayTexture(float sx, float sy, float width, float height) {
        GLuint shader;
        if (!Shader::CreateFromFile("Shaders/Overlay.vert", "Shaders/Overlay.frag", shader))
            return nullptr;

        OverlayTexture* texture = new OverlayTexture();
        texture->_shader    = shader;
        texture->_screeSize = glGetUniformLocation(shader, "u_ScreenSize");
        texture->_texture   = glGetUniformLocation(shader, "u_Texture");

        // Create mesh
        struct { GLint x, y, width, height; } viewport;
        glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);
        float texture_width  = width;
        float texture_height = height;
        texture->_screen_width   = viewport.width;
        texture->_screen_height  = viewport.height;
        if (sx < 0.0f) sx += texture->_screen_width - texture_width;
        if (sy < 0.0f) sy += texture->_screen_height - texture_height;
        sx = floorf(sx);
        sy = floorf(sy);
        // Set the geometry
        glm::vec4 quad[] = {
                //        screenspace x       screen space y       tu    tv
                glm::vec4(sx,                 sy,                  0.0f, 1.0f),
                glm::vec4(sx,                 sy + texture_height, 0.0f, 0.0f),
                glm::vec4(sx + texture_width, sy + texture_height, 1.0f, 0.0f),
                glm::vec4(sx + texture_width, sy,                  1.0f, 1.0f),
        };
        glGenVertexArrays(1, &texture->_vao);
        glBindVertexArray(texture->_vao);
        glGenBuffers(1, &texture->_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texture->_vbo);
        size_t buffer_size = sizeof(quad);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        return texture;
    }

    void OverlayTexture::Draw(GLuint texture) {
        glUseProgram(_shader);
        glUniform2f(_screeSize, _screen_width, _screen_height);
        glUniform1i(_texture, 0);
        // Set the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        // Draw the quad
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}
