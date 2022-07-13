import java.net.ServerSocket;
import java.net.Socket;

public class ServerTcp {
    public static void main(String[] args) {
        int port = -1;

        // checking of arguments
        if (args.length == 1) {

            // checking if the port is a number
            try {
                port = Integer.parseInt(args[0]);
            } catch (NumberFormatException e) {
                System.out.println("Usage: java Server [serverPort>1024]");
                System.out.println(args[0] + " is not a valid port number");
                System.exit(1);
            }

            // checking if the port is in the correct range
            if (port < 1024 || port > 65535) {
                System.out.println("Usage: java Server [serverPort>1024]");
                System.out.println(args[0] + " is not in range");
                System.exit(1);
            }
            // too many arguments, wrong usage
        } else if (args.length > 1) {
            System.out.println("Usage: java Server [serverPort>1024]");
            System.exit(1);
        }

        ServerSocket serverSocket = null;
        Socket clientSocket = null;

        try {
            // initialize the server socket to listen on the given port
            serverSocket = new ServerSocket(port);
            serverSocket.setReuseAddress(true);
            System.out.println("Server: avviato");
            System.out.println("Server: creata la server socket: " + serverSocket);

            // listening for connections, daemon running
            while (true) {
                System.out.println("Server: in attesa di richieste...\n");

                // when a connection arrives, spawn a new thread to handle it and continue
                // listening to new connections
                try {
                    clientSocket = serverSocket.accept();
                    clientSocket.setSoTimeout(30000);
                    System.out.println("Server: connessione accettata: " + clientSocket);
                } catch (Exception e) {
                    System.err
                            .println("Server: problemi nella accettazione della connessione: "
                                    + e.getMessage());
                    e.printStackTrace();
                }

                try {
                    new ServerThread(clientSocket).start();
                } catch (Exception e) {
                    System.err.println("Server: problemi nell'avvio del thread: " + e.getMessage());
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
