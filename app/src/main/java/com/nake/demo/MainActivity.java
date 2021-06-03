package com.nake.demo;

import androidx.annotation.Keep;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.swift.sandhook.SandHook;
import com.swift.sandhook.SandHookConfig;
import com.swift.sandhook.annotation.HookClass;
import com.swift.sandhook.annotation.HookMethod;
import com.swift.sandhook.annotation.HookMethodBackup;
import com.swift.sandhook.annotation.MethodParams;
import com.swift.sandhook.annotation.ThisObject;
import com.swift.sandhook.wrapper.HookErrorException;
import com.swift.sandhook.wrapper.HookWrapper;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            SandHook.addHookClass(TestHooker.class);
        } catch (HookErrorException e) {
            e.printStackTrace();
        }

        test("test!");
    }

    @Keep
    private void test(String str){
        Log.e(MainActivity.class.getCanonicalName(), str);
    }

    @HookClass(MainActivity.class)
    public static class TestHooker {

        @HookMethodBackup("test")
        @MethodParams(String.class)
        public static HookWrapper.HookEntity testBackup;

        @HookMethod("test")
        @MethodParams(String.class)
        public static void test(@ThisObject Activity thiz, String str) throws Throwable {
            Log.e(MainActivity.class.getCanonicalName(), "-------------------------hooked test call start-------------------------");
            testBackup.callOrigin(thiz, str);
            Log.e(MainActivity.class.getCanonicalName(), "-------------------------hooked test call over--------------------------");
        }

    }
}