package com.example.jnidemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity1 extends AppCompatActivity {
    MainActivity mainActivity;
    private static final int a = 100;
    TextView tv;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        tv = findViewById(R.id.tv_hello_world);
        mainActivity = new MainActivity(this,tv);
        tv.setText(mainActivity.stringFromJNI());
    }


    @Override
    protected void onPause() {
        super.onPause();
        mainActivity.releaseNativeThread();
    }

    @Override
    protected void onStart() {
        super.onStart();
        mainActivity.createNativeThread();
    }


}