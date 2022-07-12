import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class ClientTcp {
	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.out.println("Usage: Client serverTcpIp serverTcpPort");
			System.exit(1);
		}

		InetAddress serverTcpAddress = InetAddress.getByName(args[0]);
		int serverTcpPort = Integer.parseInt(args[1]);

		if (serverTcpPort < 1024 || serverTcpPort > 65535) {
			System.out.println("Porta tcp non valida");
			System.exit(1);
		}

		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		System.out.println("input:");
		for (String input; (input = stdIn.readLine()) != null;) {
			Socket socket = new Socket(serverTcpAddress, serverTcpPort);
			socket.setSoTimeout(30000);
			DataInputStream inSock = new DataInputStream(socket.getInputStream());
			DataOutputStream outSock = new DataOutputStream(socket.getOutputStream());
			outSock.writeUTF(input);
			String serverResponse = inSock.readUTF();

			System.out.println(serverResponse);
			socket.close();
			System.out.println("input:");
		}
	}
}
