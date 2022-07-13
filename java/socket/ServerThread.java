import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;

public class ServerThread extends Thread {
	private final Socket clientSocket;

	public ServerThread(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	public void run() {
		DataInputStream inSock;
		DataOutputStream outSock;
		String userInput;

		try {
			inSock = new DataInputStream(this.clientSocket.getInputStream());
			outSock = new DataOutputStream(this.clientSocket.getOutputStream());
			// NOTE: in this way the server stays alive for one user request then exits
			// if you want to keep the server alive for multiple requests, you need to use a
			// put the logic inside a while loop and read until the inSock.read != null!
			// while ((userInput = inSock.readUTF()) != null) {
			// try { ..
			// ..

			// garbage collection is not resource cleanup (destructors)
			try {
				userInput = inSock.readUTF();
				outSock.writeUTF(userInput + "yolo");
			} catch (EOFException ignored) {
				System.out.println("EOF ricevuto\n Terminata connessione con " + clientSocket);
				clientSocket.close();
			} catch (SocketException e) {
				System.out.println("timeout scattato");
				e.printStackTrace();
				clientSocket.close();
			}
			clientSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
