package solutions.cloudarchitects.vsockj;

public class Demo {

    public static void main(String[] args) {
        VSockSocketServerJNI serverJNI = new VSockSocketServerJNI();
        serverJNI.connect(1, 2);
    }
}
