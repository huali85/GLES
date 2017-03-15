//
// Created by lihua_000 on 3/1/2017.
//

#include "Platform.h"
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <android/asset_manager.h>

AAssetManager* g_asset_manager = nullptr;

bool LoadFile(const char* filename, void** data, size_t* data_size) {
    AAsset* file = AAssetManager_open(g_asset_manager, filename, AASSET_MODE_UNKNOWN);
    if (file) {
        off_t file_size = AAsset_getLength(file);
        *data = malloc(file_size);
        *data_size = file_size;
        assert(*data_size);
        AAsset_read(file, *data, file_size);
        AAsset_close(file);
    } else {
        LOGI("Cannot read file: %s\n", filename);
        return false;
    }
    return true;
}

void CheckGLError(const char* filename, unsigned int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::string info;
        switch (error) {
            case GL_INVALID_ENUM:       info = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:      info = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: info = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:      info = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: info = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        LOGE("GL error at file:%s line: %d\n%s\n", filename, line, info.c_str());
    }
}

void Timer::Reset() {
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    _prev_time = _start_time = time;
}

double Timer::GetDeltaTime() {
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    timespec diff;
    diff.tv_sec = time.tv_sec - _prev_time.tv_sec;
    diff.tv_nsec = time.tv_nsec - _prev_time.tv_nsec;
    _prev_time = time;
    return diff.tv_sec + diff.tv_nsec / 1000000000.0;
}

double Timer::GetTime() {
    timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    timespec diff;
    diff.tv_sec = time.tv_sec - _start_time.tv_sec;
    diff.tv_nsec = time.tv_nsec - _start_time.tv_nsec;
    return diff.tv_sec + diff.tv_nsec / 1000000000.0;
}

Timer::Timer() {
    Reset();
}
