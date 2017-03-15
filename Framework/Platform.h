//
// Created by lihua_000 on 3/1/2017.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include <android/log.h>
#include <GLES3/gl3.h>
#include <time.h>

#define LOG_TAG "libGame"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define GL_CHECK(x) x; CheckGLError(__FILE__, __LINE__)
#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

void CheckGLError(const char* filename, unsigned int line);
bool LoadFile(const char* filename, void** data, size_t* data_size);

class Timer {
public:
    Timer();
    void Reset();
    double GetDeltaTime();
    double GetTime();

private:
    timespec _start_time;
    timespec _prev_time;
};

#endif //PLATFORM_H
