package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.net.SocketException;

public final class ServerVSock extends BaseVSock {

    public VSock accept() throws IOException {
        if (isClosed())
            throw new SocketException("Socket closed");
        if (!bound)
            throw new SocketException("Socket not bound");
        VSock socket = new VSock();
        socket.setImplementation();
        getImplementation().accept(socket.getImplementation());
        socket.postAccept();
        return socket;
    }
}
