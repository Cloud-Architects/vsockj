package solutions.cloudarchitects.vsockj;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class Demo {

    public static void main(String[] args) throws IOException {
        VSock sock = new VSock(new VSockAddress(10, 5000));
        sock.getOutputStream()
                .write("Hello world\n".getBytes(StandardCharsets.UTF_8));
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(sock.getInputStream()))) {
            String line = reader.readLine();
            System.out.println(line);
        }
        sock.close();
    }
}
