#include <iostream>
#include <cstring>
#include <stdio.h>

#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <unistd.h>
#include <errno.h>

#include <jni.h>
#include <solutions_cloudarchitects_vsockj_VSockImpl.h>

#define JVM_IO_INTR (-2)
#define BUFFER_LEN 65536
#define min(a, b) ((a) < (b) ? (a) : (b))

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_socketCreate
  (JNIEnv *env, jobject thisObject) {
    int s = socket(AF_VSOCK, SOCK_STREAM, 0);

    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    env->SetIntField(thisObject, fdField, s);
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_connect
  (JNIEnv *env, jobject thisObject, jobject addr) {

    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    jclass VSockAddressClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockAddress");
    jfieldID cidField = env->GetFieldID(VSockAddressClass, "cid", "I");
    jfieldID portField = env->GetFieldID(VSockAddressClass, "port", "I");


    struct sockaddr_vm sock_addr;
    std::memset(&sock_addr, 0, sizeof(struct sockaddr_vm));
    sock_addr.svm_family = AF_VSOCK;
    sock_addr.svm_port = (int)env->GetIntField(addr, portField);
    sock_addr.svm_cid = (int)env->GetIntField(addr, cidField);
    connect(s, (struct sockaddr *) &sock_addr, sizeof(struct sockaddr_vm));
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_close
  (JNIEnv *env, jobject thisObject) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    close((int)env->GetIntField(thisObject, fdField));
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_write
  (JNIEnv * env, jobject thisObject, jbyteArray b, jint offset, jint len) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    char BUF[BUFFER_LEN];
    while(len > 0) {
        int chunk_offset = 0;
        int chunkLen = min(BUFFER_LEN, len);
        int chunkWorkingLen = chunkLen;

        env->GetByteArrayRegion(b, offset, chunkLen, (jbyte *)BUF);

        while(chunkWorkingLen > 0) {
            int n = (int)send(s, BUF + chunk_offset, len, 0);
            if (n > 0) {
                chunkWorkingLen -= n;
                chunk_offset += n;
                continue;
            }
            if (n == JVM_IO_INTR) {
                env->ThrowNew(env->FindClass("java/io/InterruptedIOException"), 0);
            } else {
                if (errno == ECONNRESET) {
                    env->ThrowNew(env->FindClass("sun/net/ConnectionResetException"), "Connection reset");
                } else {
                    env->ThrowNew(env->FindClass("java/net/SocketException"), "Write failed");
                }
            }
            return;
        }
        len -= chunkLen;
        offset += chunkLen;
    }
}