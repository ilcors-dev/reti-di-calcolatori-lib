import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ServerUdp {

    public static void main(String[] args) {

        if (args.length != 1) {
            System.out.println("Usage: java me.ServerUdp port");
            System.exit(1);
        }

        try {
            DatagramSocket socket = new DatagramSocket(Integer.parseInt(args[0]));
            DatagramPacket packet;
            int clientPort;
            InetAddress clientAddress;

            while (true) {
                try {
                    byte[] data = new byte[256];
                    packet = new DatagramPacket(data, data.length);
                    socket.receive(packet);

                    String richiesta = StreamUtils.bytesToStringUTF(packet.getData());

                    clientPort = packet.getPort();
                    clientAddress = packet.getAddress();
                    data = StreamUtils.intToBytes(richiesta.length());

                    packet.setPort(clientPort);
                    packet.setAddress(clientAddress);
                    packet.setData(data);
                    socket.send(packet);
                } catch (IOException e) {
                    e.printStackTrace();
                    System.out.println("problema con un pacchetto, continuo...");
                }
            }
        } catch (IOException | NumberFormatException e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}
