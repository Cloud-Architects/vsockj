package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.net.SocketAddress;

public class VSock {
    private boolean closed = false;
    private boolean created = false;
    private final Object closeLock = new Object();
    private VSockImpl implementation;

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

    public void connect(VSockAddress address) {
        implementation.connect(address);
    }

    public synchronized void close() {
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
