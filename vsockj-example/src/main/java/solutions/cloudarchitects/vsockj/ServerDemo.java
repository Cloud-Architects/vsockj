package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class ServerDemo {

    public static void main(String[] args) throws IOException {
        ServerVSock server = new ServerVSock();
        server.bind(new VSockAddress(VSockAddress.VMADDR_CID_ANY, 5000));
        System.out.println("Bound on Cid: " + server.getLocalCid());

        try {
            try (VSock peerVSock = server.accept()) {
                byte[] b = new byte[4096];
                peerVSock.getInputStream().read(b, 0, 4096);
                peerVSock.getOutputStream()
                        .write(("Got: " + new String(b, StandardCharsets.UTF_8)).getBytes(StandardCharsets.UTF_8));
            } catch (Exception ex) {
                System.out.println("Error: " + ex.getMessage());
            }
        } finally {
            server.close();
        }
    }
}
