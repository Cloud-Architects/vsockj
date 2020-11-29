package solutions.cloudarchitects.vsockj;

public class Demo {

    public static void main(String[] args) {
        VSock sock = new VSock(new VSockAddress(10, 5000));
        System.out.println(sock.getImplementation().fd);
        sock.close();
    }
}
