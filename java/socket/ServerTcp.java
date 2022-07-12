import java.net.ServerSocket;
import java.net.Socket;

public class ServerTcp {
    public static void main(String[] args) {
        int port = -1;

        if (args.length == 1) {
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException e) {
                System.out.println("Usage: java Server [serverPort>1024]");
                System.out.println(args[0] + " is not a valid port number");
                System.exit(1);
            }
            if (port < 1024 || port > 65535) {
                System.out.println("Usage: java Server [serverPort>1024]");
                System.out.println(args[0] + " is not in range");
                System.exit(1);
            }
        } else if (args.length > 1) {
            System.out.println("Usage: java Server [serverPort>1024]");
            System.exit(1);
        }

        try {
            ServerSocket serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("Server: avviato");
            System.out.println("Server: creata la server socket: " + serverSocket);
            Socket clientSocket;

            while (true) {
                System.out.println("Server: in attesa di richieste...\n");

                try {
                    clientSocket = serverSocket.accept();
                    clientSocket.setSoTimeout(30000);
                    System.out.println("Server: connessione accettata: " + clientSocket);
                    new ServerThread(clientSocket).start();
                } catch (Exception e) {
                    System.err
                            .println("Server: problemi nella accettazione della connessione: "
                                    + e.getMessage());
                    e.printStackTrace();
                }
            }
        } catch (Exception e) {
            System.err
                    .println("Server: problemi nella creazione della server socket: "
                            + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }

    }
}
