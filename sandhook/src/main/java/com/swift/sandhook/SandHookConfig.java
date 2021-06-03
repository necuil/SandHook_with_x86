package com.swift.sandhook;

import android.annotation.SuppressLint;
import android.os.Build;

import androidx.annotation.Keep;

import java.lang.reflect.Method;

public class SandHookConfig {
    public static final String LIB_NAME = "sandhook";
    public static final String LIB_NAME_64 = LIB_NAME + "E";

    public volatile static int SDK_INT = Build.VERSION.SDK_INT;
    //Debug status of hook target process
    public volatile static boolean DEBUG = true;
    //Enable compile with jit
    @Keep
    public volatile static boolean compiler = SDK_INT < 29;
    public volatile static ClassLoader initClassLoader;
    public volatile static int curUser = 0;
    public volatile static boolean delayHook = true;

    public volatile static String libSandHookPath;
    public volatile static LibLoader libLoader = new LibLoader() {
        @SuppressLint("UnsafeDynamicallyLoadedCode")
        @Override
        public void loadLib() {
            if (SandHookConfig.libSandHookPath == null) {
                System.loadLibrary(is64Bit() ? LIB_NAME_64 : LIB_NAME);
            } else {
                System.load(SandHookConfig.libSandHookPath);
            }
        }
    };

    private static boolean is64Bit() {
        try {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                return false;
            }
            Class<?> clzVMRuntime = Class.forName("dalvik.system.VMRuntime");
            Method mthVMRuntimeGet = clzVMRuntime.getDeclaredMethod("getRuntime");
            Object objVMRuntime = mthVMRuntimeGet.invoke(null);
            if (objVMRuntime == null) {
                return false;
            }
            Method sVMRuntimeIs64BitMethod = clzVMRuntime.getDeclaredMethod("is64Bit");
            Object objIs64Bit = sVMRuntimeIs64BitMethod.invoke(objVMRuntime);
            if (objIs64Bit instanceof Boolean) {
                return (boolean) objIs64Bit;
            }
        } catch (Throwable ignored) {
        }
        return false;
    }

    public interface LibLoader {
        void loadLib();
    }
}
