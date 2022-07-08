import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RMISecurityManager;

class RMIClientTemplate {
	final static int REGISTRYPORT = 1099;

	public static void main(String[] args) {
		// where the rmiregistry is running
		String registryHost = null;
		String serviceName = "";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP ServiceName");
			System.exit(1);
		}

		registryHost = args[0];
		serviceName = args[1];

		if (System.getSecurityManager() == null) {
			System.setSecurityManager(new RMISecurityManager());
		}

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		// Connessione al servizio RMI remoto
		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RMIServerTemplateInterface serverRMI = (RMIServerTemplateInterface) Naming.lookup(completeName);
			System.out.println("RMIClientTemplate: Servizio \"" + serviceName + "\" connesso");

			System.out.println("\nRichieste di servizio fino a fine file");

			String service;
			System.out.print("Servizio (R=Registrazione, P=Programma del congresso): ");

			// users request until EOF
			while ((service = stdIn.readLine()) != null) {

				// choose some service and do something..
				// REMEMBER TO CHECK THE INPUTS
				// if (service.equals("R")) {

				// }

				// else if (service.equals("P")) {

				// }

				// else {
				// System.out.println("Servizio non disponibile");
				// }

				System.out.print("Servizio (R=Registrazione, P=Programma del congresso): ");
			} // while (!EOF), fine richieste utente

		} catch (NotBoundException nbe) {
			System.err.println("RMIClientTemplate: il nome fornito non risulta registrato; " + nbe.getMessage());
			nbe.printStackTrace();
			System.exit(1);
		} catch (Exception e) {
			System.err.println("RMIClientTemplate: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
	}
}