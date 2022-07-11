import java.rmi.Naming;
import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMIServerTemplate extends UnicastRemoteObject implements RMIServerTemplateInterface {
    final static int REGISTRYPORT = 1099;

    // additional variables static to the class
    // static Object[] objects;

    public RMIServerTemplate() throws RemoteException {
        super();
    }

    public static void main(String[] args) {
        String registryHost = "localhost";
        String serviceName = "RMIServerTemplate";

        // where the server is located in the rmi registry
        String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;

        try {
            RMIServerTemplate serverRMI = new RMIServerTemplate();
            Naming.rebind(completeName, serverRMI);
            System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
        } catch (Exception e) {
            System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
}