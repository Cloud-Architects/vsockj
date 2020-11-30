package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.io.OutputStream;

public class VSockOutputStream extends OutputStream {
    private final VSockImpl vSock;
    private final byte[] temp = new byte[1];

    VSockOutputStream(VSockImpl vSock) {
        this.vSock = vSock;
    }

    @Override
    public void write(int b) {
        temp[0] = (byte) b;
        this.write(temp, 0, 1);
    }

    @Override
    public void write(byte[] b, int off, int len) {
        vSock.write(b, off, len);
    }

    @Override
    public void close() throws IOException {
        vSock.close();
        super.close();
    }
}
