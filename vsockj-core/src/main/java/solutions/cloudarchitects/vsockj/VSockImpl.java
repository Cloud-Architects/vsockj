package solutions.cloudarchitects.vsockj;

public class VSockImpl {
    static {
        System.loadLibrary("vsockj-native-" + VSockJ.VERSION);
    }

    int fd = -1;

    protected void create() {
        socketCreate();
    }

    native void socketCreate();
    native void connect(VSockAddress address);
    native void close();
    native void write(byte[] b, int off, int len);
}
