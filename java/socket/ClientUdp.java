import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ClientUdp {
	public static void main(String[] args) throws Exception {
		// check args
		if (args.length != 2) {
			System.out.println("Usage: ClientUdp serverUdpIp serverUdpPort");
			System.exit(1);
		}

		// get the server address & port
		InetAddress serverUdpAddress = InetAddress.getByName(args[0]);
		int serverUdpPort = Integer.parseInt(args[1]);

		// check if the port is in the correct range
		if (serverUdpPort < 1024 || serverUdpPort > 65535) {
			System.out.println("Porta udp non valida");
			System.exit(1);
		}

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		String userInput;

		System.out.println("input:");
		while ((userInput = stdIn.readLine()) != null) {
			byte[] buff = StreamUtils.stringUTFToBytes(userInput);
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
