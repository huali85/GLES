package com.huali.basiclighting;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {

    private MainView _view;

    // Virutal joystick
    private float _startX, _startY;
    private float _max_distance = 0;
    private float _min_distance = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        _view = new MainView(this);
        setContentView(_view);

        AssetManager _asset_manager = getAssets();
        JNILib.InitAssetManager(_asset_manager);
    }

    @Override
    protected void onPause() {
        super.onPause();
        _view.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        _view.onResume();
    }

    // Single touch only
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (_max_distance == 0) {
            _max_distance = getWindow().getDecorView().getHeight() * 0.25f;
            float _threshold = 0.2f;
            _min_distance = _max_distance * _threshold;
        }
        float _dx = 0;
        float _dy = 0;
        int action = event.getActionMasked();
        if (action == MotionEvent.ACTION_DOWN) {
            _startX = event.getX();
            _startY = event.getY();
        }
        else if (action == MotionEvent.ACTION_UP) {
            _dx = _dy = 0;
            JNILib.TouchMove(_dx, _dy);
        }
        else if (action == MotionEvent.ACTION_MOVE) {
            float temp_dx = event.getX() - _startX;
            if (temp_dx > _min_distance || temp_dx < -_min_distance) {
                _dx = temp_dx / _max_distance;
                if (_dx > 1) { _dx = 1; }
                else if (_dx < -1) { _dx = -1; }
            }

            float temp_dy = event.getY() - _startY;
            if (temp_dy > _min_distance || temp_dy < -_min_distance) {
                _dy = temp_dy / _max_distance;
                if (_dy > 1) { _dy = 1; }
                else if (_dy < -1) { _dy = -1; }
            }
            //Log.i("Joystick", String.format("move: %f, %f", _dx, _dy));
            JNILib.TouchMove(_dx, _dy);
        }
        return true;
    }

    private class MainView extends GLSurfaceView {

        public MainView(Context context) {
            super(context);
            setEGLContextClientVersion(3);
            setEGLConfigChooser(8, 8, 8, 8, 16, 0);
            setRenderer(new Renderer());
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            JNILib.Shutdown();
            super.surfaceDestroyed(holder);
        }

        private class Renderer implements GLSurfaceView.Renderer {
            @Override
            public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
                JNILib.Init();
            }

            @Override
            public void onSurfaceChanged(GL10 gl10, int width, int height) {
                JNILib.Resize(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl10) {
                JNILib.Frame();
            }
        }
    }

}
