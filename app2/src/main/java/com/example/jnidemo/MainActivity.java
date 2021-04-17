package com.example.jnidemo;

import android.widget.TextView;

public class MainActivity {

    private TextView mTextView;
    private MainActivity1 mActivity1;
    static {
        System.loadLibrary("native-lib");
    }

    public MainActivity(MainActivity1 mainActivity1, TextView tv) {
        mActivity1 = mainActivity1;

        mTextView = tv;
    }


    /**
     * java to c
     * @return
     */
    public native String stringFromJNI();


    //在jni层创建一个线程
    public native void createNativeThread();

    //jni层线程退出释放资源
    public native void releaseNativeThread();

    public void callbackFromJNI(int arg){
        final int i = arg;
        mActivity1.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mTextView.setText("Native Update: "+ i);
            }
        });
    }
}
