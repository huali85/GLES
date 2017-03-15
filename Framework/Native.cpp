#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Texture.h"

#define LOG_TAG "libGame"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

/* [Vertex source] */
static const char glVertexShader[] =
        "attribute vec3 vPosition;\n"
        "attribute vec2 vTexCoord;\n"
        "varying vec2 texCoord;\n"
        "uniform mat4 MVP;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = MVP * vec4(vPosition, 1.0);\n"
        "  texCoord = vTexCoord;\n"
        "}\n";

/* [Fragment source] */
static const char glFragmentShader[] =
        "precision mediump float;\n"
        "varying vec2 texCoord;\n"
        "uniform sampler2D texture;\n"
        "void main()\n"
        "{\n"
         "  gl_FragColor = texture2D(texture, texCoord);\n"
         "}\n";

GLuint LoadShader(GLenum shader_type, const char* shader_source) {
    GLuint shader = glCreateShader(shader_type);
    if (shader) {
        glShaderSource(shader, 1, &shader_source, NULL);
        glCompileShader(shader);

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (!result) {
            GLint info_len = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
            if (info_len) {
                char* buf = (char*)malloc(info_len);
                if (buf) {
                    glGetShaderInfoLog(shader, info_len, NULL, buf);
                    LOGE("Failed to compile shader %d:\n%s\n", shader_type, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint CreateProgram(const char* vert_source, const char* frag_source) {
    GLuint vert_shader = LoadShader(GL_VERTEX_SHADER, vert_source);
    if (!vert_shader) {
        return 0;
    }

    GLuint frag_shader = LoadShader(GL_FRAGMENT_SHADER, frag_source);
    if (!frag_shader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);
        glLinkProgram(program);
        GLint result;
        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result != GL_TRUE) {
            GLint info_len = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
            if (info_len) {
                char* buf = (char*)malloc(info_len);
                if (buf) {
                    glGetProgramInfoLog(program, info_len, NULL, buf);
                    LOGE("Failed to link program: %s\n", buf);
                    free(buf);
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
    }
    return program;
}

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;
};

GLuint g_program;
GLuint g_uniform_MVP;
GLuint g_texture;

glm::mat4 g_proj;
float g_angle = 0.0f;

bool SetupGraphics(int width, int height) {
    g_program = CreateProgram(glVertexShader, glFragmentShader);
    if (!g_program) {
        LOGE("Failed to create program");
        return false;
    }

    g_uniform_MVP = glGetUniformLocation(g_program, "MVP");
    GLuint sampler = glGetUniformLocation(g_program, "texture");
    glUniform1i(sampler, 0);
    g_proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // vertices data
    Vertex vertices[] = {
            /* Back */
            { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
            /* Front */
            { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
            /* Left */
            { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
            /* Right */
            { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
            /* Top */
            { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
            /* Bottom */
            { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
            { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 0.0f) }
    } ;
    // cretae vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLushort indices[] = { 0, 2, 3, 0, 1, 3, 4, 6, 7, 4, 5, 7, 8, 9, 10, 11, 8, 10, 12, 13, 14, 15, 12, 14, 16, 17, 18, 16, 19, 18, 20, 21, 22, 20, 23, 22 };
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLfloat*)(offsetof(Vertex, texCoord)));

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    // Load texture
    g_texture = LoadSimpleTexture();
    if (g_texture == 0)
        return false;

    return true;
}

void RenderFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -10.0f));
    model = glm::rotate(model, glm::radians(g_angle), glm::vec3(0.0f, 1.0f, 0.0f));

    glUseProgram(g_program);
    glUniformMatrix4fv(g_uniform_MVP, 1, GL_FALSE, glm::value_ptr(g_proj * model));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    if (++g_angle > 360.0f) {
        g_angle -= 360.0f;
    }
}

extern "C" {
JNIEXPORT void JNICALL Java_com_huali_glsample_JNILib_Init(JNIEnv *env, jobject obj, jint width, jint height) {
        SetupGraphics(width, height);
    }

    JNIEXPORT void JNICALL Java_com_huali_glsample_JNILib_Frame(JNIEnv *env, jobject obj) {
        RenderFrame();
    }
}
