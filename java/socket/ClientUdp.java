import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ClientUdp {
	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.out.println("Usage: ClientUdp serverUdpIp serverUdpPort");
			System.exit(1);
		}

		InetAddress serverUdpAddress = InetAddress.getByName(args[0]);
		int serverUdpPort = Integer.parseInt(args[1]);

		if (serverUdpPort < 1024 || serverUdpPort > 65535) {
			System.out.println("Porta udp non valida");
			System.exit(1);
		}

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		System.out.println("input:");
		for (String input; (input = stdIn.readLine()) != null;) {
			byte[] buff = StreamUtils.stringUTFToBytes(input);
			DatagramSocket socket = new DatagramSocket();
			DatagramPacket packet = new DatagramPacket(buff, buff.length, serverUdpAddress, serverUdpPort);
			socket.send(packet);

			socket.receive(packet);

			int risp = StreamUtils.bytesToInt(packet.getData());

			System.out.println(risp);
			socket.close();
		}
	}
}
