#include <iostream>

#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <unistd.h>

#include <jni.h>
#include <solutions_cloudarchitects_vsockj_VSockImpl.h>

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_socketCreate
  (JNIEnv *env, jobject thisObject) {
    int s = socket(AF_VSOCK, SOCK_STREAM, 0);

    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    env->SetIntField(thisObject, fdField, s);
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_connect
  (JNIEnv *env, jobject thisObject, jobject) {
//    struct sockaddr_vm addr;
//    memset(&addr, 0, sizeof(struct sockaddr_vm));
//    addr.svm_family = AF_VSOCK;
//    addr.svm_port = 9999;
//    addr.svm_cid = VMADDR_CID_HOST;
//    connect(s, &addr, sizeof(struct sockaddr_vm));
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_close
  (JNIEnv *env, jobject thisObject) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    close((int)env->GetIntField(thisObject, fdField));
}