-keepclasseswithmembers class * { native <methods>; }

-keepclasseswithmembers class * {
    @androidx.annotation.Keep <methods>;
}
-keepclasseswithmembers class * {
    @androidx.annotation.Keep <fields>;
}

-keep @androidx.annotation.Keep class * { *; }