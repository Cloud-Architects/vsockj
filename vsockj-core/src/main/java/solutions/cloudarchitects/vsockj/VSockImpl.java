package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.net.SocketException;

public final class VSockImpl {
    static {
        System.loadLibrary("vsockj-native-" + VSockJ.VERSION);
    }

    int fd = -1;

    void create() throws SocketException {
        socketCreate();
    }

    native void socketCreate() throws SocketException;
    native void connect(VSockAddress address) throws SocketException;
    native void close() throws IOException;
    native void write(byte[] b, int off, int len) throws IOException;
    native int read(byte[] b, int off, int len) throws IOException;
    native void bind(VSockAddress address) throws IOException;
    native void listen(int backlog) throws IOException;
    native void accept(VSockImpl peerVSock);
    native int getLocalCid();
}
