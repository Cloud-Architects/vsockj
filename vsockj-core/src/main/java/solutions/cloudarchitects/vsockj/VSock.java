package solutions.cloudarchitects.vsockj;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.SocketException;

public final class VSock extends BaseVSock implements Closeable {
    private boolean connected = false;
    private VSockOutputStream outputStream;
    private VSockInputStream inputStream;

    public VSock() {
    }

    public VSock(VSockAddress address) {
        try {
            getImplementation().connect(address);
        } catch (Exception e) {
            try {
                close();
            } catch (Exception ce) {
                e.addSuppressed(ce);
            }
            throw new IllegalStateException(e.getMessage(), e);
        }
    }

    public void connect(VSockAddress address) throws SocketException {
        if (isClosed()) {
            throw new SocketException("Socket closed");
        }
        if (connected) {
            throw new SocketException("Socket already connected");
        }
        getImplementation().connect(address);
        connected = true;
        bound = true;
    }

    public synchronized OutputStream getOutputStream() throws IOException {
        if (isClosed()) {
            throw new SocketException("VSock is closed");
        }
        if (outputStream == null) {
            outputStream = new VSockOutputStream(getImplementation());
        }
        return outputStream;
    }

    public synchronized InputStream getInputStream() throws IOException {
        if (isClosed()) {
            throw new SocketException("VSock is closed");
        }
        if (inputStream == null) {
            inputStream = new VSockInputStream(getImplementation());
        }
        return inputStream;
    }

    void postAccept() {
        created = true;
        bound = true;
        connected = true;
    }
}
