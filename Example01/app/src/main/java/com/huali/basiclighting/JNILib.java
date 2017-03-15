package com.huali.basiclighting;

import android.content.res.AssetManager;

class JNILib {
    static {
        System.loadLibrary("game");
    }

    public static native void Init();
    public static native void InitAssetManager(AssetManager asset_manager);
    public static native void Resize(int width, int height);
    public static native void Frame();
    public static native void Shutdown();

    public static native void TouchMove(float x, float y);
}
