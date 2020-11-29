#include <iostream>

#include <jni.h>
#include <solutions_cloudarchitects_vsockj_VSockSocketServerJNI.h>

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockSocketServerJNI_connect
  (JNIEnv *, jobject, jint, jint) {
    std::cout << "Hello from C++ !!" << std::endl;
}