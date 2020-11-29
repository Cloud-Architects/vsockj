package solutions.cloudarchitects.vsockj;

import java.net.SocketAddress;
import java.util.Objects;

public class VSockAddress extends SocketAddress {
    private final int cid;
    private final int port;

    public VSockAddress(int cid, int port) {
        this.cid = cid;
        this.port = port;
    }

    public int getCid() {
        return cid;
    }

    public int getPort() {
        return port;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        VSockAddress that = (VSockAddress) o;
        return cid == that.cid &&
                port == that.port;
    }

    @Override
    public int hashCode() {
        return Objects.hash(cid, port);
    }

    @Override
    public String toString() {
        return "VSockAddress{" +
                "cid=" + cid +
                ", port=" + port +
                '}';
    }
}
