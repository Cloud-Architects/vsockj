package solutions.cloudarchitects.vsockj;

public class VSockSocketServerJNI {

    static {
        System.loadLibrary("vsockj-native-" + VSockJ.VERSION);
    }

    public native void connect(int cid, int port);
}
