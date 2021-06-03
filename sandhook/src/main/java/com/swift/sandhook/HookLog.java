package com.swift.sandhook;

import android.util.Log;


public class HookLog {

    public static final String TAG = "SandHook";

    public static boolean DEBUG = false;

    public static int v(String s) {
        if(!DEBUG)
            return 0;
        return Log.v(TAG, s);
    }

    public static int i(String s) {
        if(!DEBUG)
            return 0;
        return Log.i(TAG, s);
    }

    public static int d(String s) {
        if(!DEBUG)
            return 0;
        return Log.d(TAG, s);
    }

    public static int w(String s) {
        if(!DEBUG)
            return 0;
        return Log.w(TAG, s);
    }

    public static int e(String s) {
        if(!DEBUG)
            return 0;
        return Log.e(TAG, s);
    }

    public static int e(String s, Throwable t) {
        if(!DEBUG)
            return 0;
        return Log.e(TAG, s, t);
    }


}
