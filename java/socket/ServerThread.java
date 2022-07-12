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

		try {
			inSock = new DataInputStream(this.clientSocket.getInputStream());
			outSock = new DataOutputStream(this.clientSocket.getOutputStream());

			// garbage collection is not resource cleanup (destructors)
			try {
				String test = inSock.readUTF();
				outSock.writeUTF(test + "yolo");
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
