package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class ClientDemo {

    public static void main(String[] args) throws IOException {
        VSock sock = new VSock(new VSockAddress(3, 5000));
        sock.getOutputStream()
                .write("Hello world\n".getBytes(StandardCharsets.UTF_8));
        byte[] b = new byte[4096];
        sock.getInputStream().read(b, 0, 4096);
        System.out.println(new String(b, StandardCharsets.UTF_8));
        sock.close();
    }
}
