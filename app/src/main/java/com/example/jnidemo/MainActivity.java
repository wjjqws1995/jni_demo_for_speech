package com.example.jnidemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private static final int a = 100;
    private  TextView tv;


    static {
        System.loadLibrary("native-lib");
        initData();

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());
        tv.setText(stringFromJNI());
//        callMethod();
        callSystemMethod(this);

//        callMethod();
//


    }

    @Override
    protected void onStart() {
        super.onStart();
        //在jni层创建一个线程
//        createNativeThread();

    }

    @Override
    protected void onStop() {
        super.onStop();
        //jni层线程退出释放资源
//        releaseNativeThread();
    }

    /**
     * java to c
     * @return
     */
    public native String stringFromJNI();


    /**
     * C to java
     * 系统方法
     * @return
     */
    public native void callSystemMethod(Context context);


    /**
     * C to java
     * 实例方法
     * @return
     */
    public native void callMethod();

    /**
     * C to java
     * 构造方法
     * @return
     */
    public native void callInitMethod();

    /**
     * C to java
     * 字段
     * @return
     */
    public native int callJavaField();


    /**
     * C to java
     * 数组
     * @return
     */
    private native int[][] initInt2DArray(int size);


    //在jni层创建一个线程
    public native void createNativeThread();

    //jni层线程退出释放资源
    public native void releaseNativeThread();

    public static native void initData();


    /**
     * 异常
     */
    public native void exceptionTest() throws IllegalArgumentException;



    private void toastJavaTest1(){
        Toast.makeText(MainActivity.this,"调用本地java代码",Toast.LENGTH_LONG).show();
    }

    private void showInt(){
        Toast.makeText(MainActivity.this,"in java a = " + a,Toast.LENGTH_LONG).show();
        Toast.makeText(MainActivity.this,"in C a = " + callJavaField(),Toast.LENGTH_LONG).show();
    }

    private void callArrayMethod(){
        int size = 5;
        int[][] i2arr = initInt2DArray(size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                System.out.print(i2arr[i][j]+"");
            }
            System.out.println();
        }
    }

    public void callbackFromJNI(int arg){
        final int i = arg;
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tv.setText("Native Update: "+ i);
            }
        });
    }

    private void exception() throws NullPointerException{
        throw new NullPointerException("抛出了异常");
    }

}
