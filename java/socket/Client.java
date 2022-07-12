import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;

public class Client {
	public static void main(String[] args) throws Exception {
		if (args.length != 4) {
			System.out.println("Usage: Client serverTcpIp serverTcpPort serverUdpIp serverUdpPort");
			System.exit(1);
		}

		InetAddress serverTcpAddress = InetAddress.getByName(args[0]);
		int serverTcpPort = Integer.parseInt(args[1]);
		InetAddress serverUdpAddress = InetAddress.getByName(args[2]);
		int serverUdpPort = Integer.parseInt(args[3]);

		if (serverTcpPort < 1024 || serverTcpPort > 65535) {
			System.out.println("Porta tcp non valida");
			System.exit(1);
		}
		if (serverUdpPort < 1024 || serverUdpPort > 65535) {
			System.out.println("Porta udp non valida");
			System.exit(1);
		}

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		System.out.println("u: udp, t: tcp");
		for (String input; (input = stdIn.readLine()) != null;) {
			/* UDP */
			if (input.equals("u")) {
				System.out.println("input:");
				byte[] buff = StreamUtils.stringUTFToBytes(stdIn.readLine());

				DatagramSocket socket = new DatagramSocket();
				DatagramPacket packet = new DatagramPacket(buff, buff.length, serverUdpAddress, serverUdpPort);
				socket.send(packet);

				socket.receive(packet);

				int risp = StreamUtils.bytesToInt(packet.getData());

				System.out.println(risp);
				socket.close();
				/* TCP */
			} else if (input.equals("t")) {
				try {
					System.out.println("input:");
					String toSend = stdIn.readLine();
					Socket socket = new Socket(serverTcpAddress, serverTcpPort);
					socket.setSoTimeout(30000);
					DataInputStream inSock = new DataInputStream(socket.getInputStream());
					DataOutputStream outSock = new DataOutputStream(socket.getOutputStream());
					outSock.writeUTF(toSend);
					String serverResponse = inSock.readUTF();

					System.out.println(serverResponse);
					socket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			System.out.println("u: udp, t: tcp");
		}
	}
}
