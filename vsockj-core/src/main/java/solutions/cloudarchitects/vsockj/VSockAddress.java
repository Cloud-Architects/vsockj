package solutions.cloudarchitects.vsockj;

import java.net.SocketAddress;
import java.util.Objects;

public class VSockAddress extends SocketAddress {
    public static final int VMADDR_CID_ANY = -1;
    public static final int VMADDR_CID_HYPERVISOR = 0;
    public static final int VMADDR_CID_RESERVED = 1;
    public static final int VMADDR_CID_HOST = 2;

    public static final int VMADDR_PORT_ANY = -1;
    final int cid;
    final int port;

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
