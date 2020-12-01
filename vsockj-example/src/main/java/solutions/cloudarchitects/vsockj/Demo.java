package solutions.cloudarchitects.vsockj;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class Demo {

    public static void main(String[] args) throws IOException {
        VSock sock = new VSock(new VSockAddress(10, 5000));
        sock.getOutputStream().write("Hello world".getBytes(StandardCharsets.UTF_8));
        sock.close();
    }
}
