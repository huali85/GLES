//
// Created by lihua_000 on 3/1/2017.
//
#include <jni.h>
#include <android/asset_manager_jni.h>
#include "Game.h"

extern AAssetManager* g_asset_manager;
extern Game* g_game;

extern "C" {
    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Init(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_InitAssetManager(JNIEnv *env, jobject obj, jobject asset_manager);
    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Resize(JNIEnv *env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Frame(JNIEnv *env, jobject obj);
    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Shutdown(JNIEnv *env, jobject obj);

    JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_TouchMove(JNIEnv *env, jclass type, jfloat x, jfloat y);
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Init(JNIEnv *env, jobject obj) {
    if (g_game) {
        delete g_game;
    }
    g_game = CreateGame();
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_InitAssetManager(JNIEnv *env, jobject obj, jobject asset_manager) {
    g_asset_manager = AAssetManager_fromJava(env, asset_manager);
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Resize(JNIEnv *env, jobject obj, jint width, jint height) {
    if (g_game) {
        g_game->Resize(width, height);
    }
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Frame(JNIEnv *env, jobject obj) {
    if (g_game) {
        g_game->Render();
    }
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_Shutdown(JNIEnv *env, jobject obj) {
    if (g_game) {
        delete g_game;
        g_game = nullptr;
    }
}

JNIEXPORT void JNICALL Java_com_huali_basiclighting_JNILib_TouchMove(JNIEnv *env, jclass type, jfloat x, jfloat y) {
    if (!g_game)
        return;
    g_game->OnTouchMove(x, y, false);
}
