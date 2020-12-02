package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.SocketException;

public class VSock {
    private boolean closed = false;
    private boolean created = false;
    private final Object closeLock = new Object();
    private VSockImpl implementation;
    private VSockOutputStream outputStream;
    private VSockInputStream inputStream;

    public VSock() {
    }

    public VSock(VSockAddress address) {
        try {
            setImplementation();

            implementation.create();
            implementation.connect(address);
        } catch (Exception e) {
            try {
                close();
            } catch (Exception ce) {
                e.addSuppressed(ce);
            }
            throw new IllegalStateException(e.getMessage(), e);
        }
    }

    private void setImplementation() {
        implementation = new VSockImpl();
        created = true;
    }

    VSockImpl getImplementation() {
        if (!created)
            setImplementation();
        return implementation;
    }

    public void connect(VSockAddress address) throws SocketException {
        implementation.connect(address);
    }

    public synchronized OutputStream getOutputStream() throws IOException {
        if (isClosed()) {
            throw new SocketException("VSock is closed");
        }
        if (outputStream == null) {
            outputStream = new VSockOutputStream(implementation);
        }
        return outputStream;
    }

    public synchronized InputStream getInputStream() throws IOException {
        if (isClosed()) {
            throw new SocketException("VSock is closed");
        }
        if (inputStream == null) {
            inputStream = new VSockInputStream(implementation);
        }
        return inputStream;
    }

    public synchronized void close() throws IOException {
        synchronized (closeLock) {
            if (isClosed())
                return;
            if (created)
                implementation.close();
            closed = true;
        }
    }

    private boolean isClosed() {
        return closed;
    }
}
