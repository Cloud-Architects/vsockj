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

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return;
    }

    jclass VSockAddressClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockAddress");
    jfieldID cidField = env->GetFieldID(VSockAddressClass, "cid", "I");
    jfieldID portField = env->GetFieldID(VSockAddressClass, "port", "I");


    struct sockaddr_vm sock_addr;
    std::memset(&sock_addr, 0, sizeof(struct sockaddr_vm));
    sock_addr.svm_family = AF_VSOCK;
    sock_addr.svm_port = (int)env->GetIntField(addr, portField);
    sock_addr.svm_cid = (int)env->GetIntField(addr, cidField);
    int status = connect(s, (struct sockaddr *) &sock_addr, sizeof(struct sockaddr_vm));
    if (status != 0) {
        if (errno == EALREADY) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Previous connection in progress");
        } else if (errno == ECONNREFUSED) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Connection refused");
        } else if (errno == EINPROGRESS) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Connection cannot be completed now");
        } else if (errno == EINTR) {
            env->ThrowNew(env->FindClass("java/io/InterruptedIOException"), 0);
        } else if (errno == EISCONN) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Socket is already connected");
        } else if (errno == ENOTSOCK) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "File descriptor doesn't point to socket");
        } else if (errno == EPROTOTYPE) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Protocol not supported");
        } else if (errno == ETIMEDOUT) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Timeout");
        } else if (errno == ECONNRESET) {
            env->ThrowNew(env->FindClass("java/net/ConnectException"), "Connection reset by peer");
        } else  {
            env->ThrowNew(env->FindClass("java/net/ConnectException"),
                ("Connect failed with error no: " + std::to_string(errno)).c_str());
        }
    }
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_close
  (JNIEnv *env, jobject thisObject) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        return;
    }

    int status = close(s);

    env->SetIntField(thisObject, fdField, -1);
    if (status != 0) {
        if (errno == EBADF) {
            env->ThrowNew(env->FindClass("java/net/SocketException"), "Not valid file descriptor");
        } else if (errno == EINTR) {
            env->ThrowNew(env->FindClass("java/io/InterruptedIOException"), 0);
        } else  {
            env->ThrowNew(env->FindClass("java/net/ConnectException"),
                ("Close failed with error no: " + std::to_string(errno)).c_str());
        }
    }
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_write
  (JNIEnv * env, jobject thisObject, jbyteArray b, jint offset, jint len) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return;
    }

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

JNIEXPORT jint JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_read
  (JNIEnv * env, jobject thisObject, jbyteArray b, jint off, jint len) {
    jint nread;

    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return -1;
    }

    char *bufP = (char *)malloc((size_t)len);

    nread = (jint) recv(s, bufP, len, 0);

    if (nread <= 0) {
        if (nread < 0) {
            if (errno == ECONNRESET || errno == EPIPE) {
                env->ThrowNew(env->FindClass("sun/net/ConnectionResetException"), "Connection reset");
            }  else if (errno == EBADF) {
                env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
            } else if (errno == EINTR) {
                env->ThrowNew(env->FindClass("java/io/InterruptedIOException"), 0);
            } else  {
                env->ThrowNew(env->FindClass("java/net/SocketException"),
                    ("Read failed with error no: " + std::to_string(errno)).c_str());
            }
        }
    } else {
        env->SetByteArrayRegion(b, off, nread, (jbyte *)bufP);
    }

    free(bufP);
    return nread;
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_bind
  (JNIEnv *env, jobject thisObject, jobject addr) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return;
    }

    jclass VSockAddressClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockAddress");
    jfieldID cidField = env->GetFieldID(VSockAddressClass, "cid", "I");
    jfieldID portField = env->GetFieldID(VSockAddressClass, "port", "I");


    struct sockaddr_vm sock_addr;
    std::memset(&sock_addr, 0, sizeof(struct sockaddr_vm));
    sock_addr.svm_family = AF_VSOCK;
    sock_addr.svm_port = (int)env->GetIntField(addr, portField);
    sock_addr.svm_cid = (int)env->GetIntField(addr, cidField);

    int status = bind(s, (struct sockaddr *) &sock_addr, sizeof(struct sockaddr_vm));

    if (status != 0) {
        if (errno == EACCES) {
            env->ThrowNew(env->FindClass("java/net/BindException"), "Insufficient access");
        } else if (errno == EADDRINUSE) {
            env->ThrowNew(env->FindClass("java/net/BindException"), "Address already in use");
        } else if (errno == EBADF) {
            env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        } else if (errno == EINTR) {
            env->ThrowNew(env->FindClass("java/io/InterruptedIOException"), 0);
        } else if (errno == EADDRNOTAVAIL) {
            env->ThrowNew(env->FindClass("java/net/BindException"), "Nonexistent interface requested");
        } else  {
            env->ThrowNew(env->FindClass("java/net/SocketException"),
                ("Bind failed with error no: " + std::to_string(errno)).c_str());
        }
    }
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_listen
  (JNIEnv *env, jobject thisObject, jint backlog) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return;
    }

    int status = listen(s, (int) backlog);
    if (status != 0) {
        env->ThrowNew(env->FindClass("java/net/SocketException"),
                        ("Listen exception " + std::to_string(errno)).c_str());
    }
}

JNIEXPORT void JNICALL Java_solutions_cloudarchitects_vsockj_VSockImpl_accept
  (JNIEnv *env, jobject thisObject, jobject connectionVSock) {
    jclass VSockImplClass = env->FindClass("solutions/cloudarchitects/vsockj/VSockImpl");
    jfieldID fdField = env->GetFieldID(VSockImplClass, "fd", "I");
    int s = (int)env->GetIntField(thisObject, fdField);

    if (s == -1) {
        env->ThrowNew(env->FindClass("java/net/SocketException"), "Socket is closed");
        return;
    }

	struct sockaddr_vm peer_addr; // TODO: set in socket
	socklen_t peer_addr_size = sizeof(struct sockaddr_vm);
	int peer_fd = accept(s, (struct sockaddr *) &peer_addr, &peer_addr_size);

    if (peer_fd == -1) {
        if (errno == EAGAIN) {
            env->ThrowNew(env->FindClass("java/net/SocketException"), "Try again error"); // TODO: implement retry several times
        } else  {
            env->ThrowNew(env->FindClass("java/net/SocketException"),
                ("Bind failed with error no: " + std::to_string(errno)).c_str());
        }
        return;
    }

	env->SetIntField(connectionVSock, fdField, peer_fd);
}