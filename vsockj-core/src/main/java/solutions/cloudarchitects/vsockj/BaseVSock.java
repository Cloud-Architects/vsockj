package solutions.cloudarchitects.vsockj;

import java.io.Closeable;
import java.io.IOException;
import java.net.SocketException;

abstract class BaseVSock implements Closeable {
    private static final int DEFAULT_BACKLOG = 42;

    protected final Object closeLock = new Object();

    protected boolean closed = false;
    protected boolean created = false;
    protected boolean bound = false;
    private VSockImpl implementation;

    private void createImplementation() throws SocketException {
        implementation = new VSockImpl();
        implementation.create();
        created = true;
    }

    protected VSockImpl getImplementation() throws SocketException {
        if (!created)
            createImplementation();
        return implementation;
    }

    protected VSockImpl setImplementation() throws SocketException {
        if(implementation == null) {
            implementation = new VSockImpl();
        }
        return implementation;
    }

    public void bind(VSockAddress address) throws IOException {
        bind(address, DEFAULT_BACKLOG);
    }

    public void bind(VSockAddress address, int backlog) throws IOException {
        if (isClosed()) {
            throw new SocketException("Socket closed");
        }
        if (bound) {
            throw new SocketException("Socket already bound");
        }
        if (backlog <= 0) {
            backlog = DEFAULT_BACKLOG;
        }
        getImplementation().bind(address);
        getImplementation().listen(backlog);
        bound = true;
    }

    @Override
    public synchronized void close() throws IOException {
        synchronized (closeLock) {
            if (isClosed())
                return;
            if (created)
                getImplementation().close();
            closed = true;
        }
    }

    protected boolean isClosed() {
        return closed;
    }
}
