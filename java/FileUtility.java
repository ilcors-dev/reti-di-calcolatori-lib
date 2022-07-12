import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.RemoteException;

public class FileUtility {
    /**
     * Swaps rows of a file at the given line numbers
     * 
     * @param filePath  the file path to swap rows in
     * @param firstRow  the first row to swap
     * @param secondRow the second row to swap
     * @return 0 if all ok, -1 if error
     */
    public static int swapRows(String filePath, int firstRow, int secondRow) {
        BufferedReader br = null;
        BufferedWriter bw = null;
        String line;
        String firstLineCache = null;
        String secondLineCache = null;
        int count = 0;

        System.out.println("Swapping rows...");

        try {
            br = new BufferedReader(new FileReader(filePath));
            bw = new BufferedWriter(new FileWriter("temp"));
        } catch (IOException e) {
            System.out.println("File not dound ");
            e.printStackTrace();
            return -1;
        }

        // getting the lines...
        try {
            while ((line = br.readLine()) != null) {
                count++;
                if (count == firstRow) {
                    firstLineCache = line;
                } else if (count == secondRow) {
                    secondLineCache = line;
                }
            }
            br.close();
        } catch (IOException e) {
            System.out.println("Could not read swap file " + filePath);
            e.printStackTrace();
            try {
                bw.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            return -1;
        }

        // swapping the lines in a new file
        try {
            br = new BufferedReader(new FileReader(filePath));
            count = 0;

            while ((line = br.readLine()) != null) {
                count++;

                if (count == firstRow) {
                    line = secondLineCache;
                } else if (count == secondRow) {
                    line = firstLineCache;
                }

                bw.write(line + "\n");
            }
            bw.close();
            br.close();
        } catch (IOException e) {
            System.out.println("Could not read swap file " + filePath);
            e.printStackTrace();
            return -1;
        }

        // renaming file...
        File file = new File(filePath);
        File tempFile = new File("temp");

        if (!tempFile.renameTo(file)) {
            System.out.println("Error renaming " + filePath);
            return -1;
        }

        if (!tempFile.delete()) {
            System.out.println("Error deleting " + filePath);
            return -1;
        }

        return 0;
    }

    /**
     * Writes the data read from the `src` stream into the `dest` stream
     * 
     * NOTE: Source and destination streams MUST be opened and closed by the caller.
     */
    static protected void transferBinaryFile(DataInputStream src,
            DataOutputStream dest) throws IOException {

        // ciclo di lettura da sorgente e scrittura su destinazione
        int buffer;
        try {
            // esco dal ciclo all lettura di un valore negativo -> EOF
            // N.B.: la funzione consuma l'EOF
            while ((buffer = src.read()) >= 0) {
                dest.write(buffer);
            }
            dest.flush();
        } catch (EOFException e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
        }
    }

    /**
     * Writes the data read from the `src` stream into the `dest` stream using the
     * given `fileSize`
     * 
     * NOTE: Source and destination streams MUST be opened and closed by the caller.
     */
    static protected void transferBinaryFile(DataInputStream src,
            DataOutputStream dest, long fileSize) throws IOException {

        // ciclo di lettura da sorgente e scrittura su destinazione
        int buffer;
        try {
            // esco dal ciclo all lettura di un valore negativo -> EOF
            // N.B.: la funzione consuma l'EOF
            for (buffer = 0; buffer < fileSize; buffer++) {
                dest.write(src.read());
            }
            dest.flush();
        } catch (EOFException e) {
            System.out.println("Problemi, i seguenti: ");
            e.printStackTrace();
        }
    }

    /**
     * Returns the line corresponding to the `lineIndex` parameter
     * 
     * @param filename
     * @param lineIndex
     * 
     * @return line letta o "Linea non trovata..."
     */
    static String getLine(String filename, int lineIndex) {
        String line = null;
        BufferedReader in = null;

        if (lineIndex <= 0)
            return line = "Linea non trovata: numero line maggiore di 0.";
        // associazione di uno stream di input al file da cui estrarre le linee
        try {
            in = new BufferedReader(new FileReader(filename));
            System.out.println("File aperto: " + filename);
        } catch (FileNotFoundException e) {
            System.out.println("File non trovato: ");
            e.printStackTrace();
            return line = "File non trovato";
        }

        try {
            for (int i = 1; i <= lineIndex; i++) {
                line = in.readLine();
                if (line == null) {
                    line = "Linea non trovata";
                    in.close();
                    return line;
                }
            }
        } catch (IOException e) {
            System.out.println("Linea non trovata: ");
            e.printStackTrace();
            return line = "Linea non trovata";
        }

        System.out.println("Linea selezionata: " + line);

        try {
            in.close();
        } catch (IOException e) {
            System.out.println("Errore nella chiusura del reader");
            e.printStackTrace();
        }

        return line;
    }

    /**
     * Recovers the next line of a file previously opened
     * 
     * @param in
     * @return line
     */
    static String getNextLine(BufferedReader in) {
        String line = null;
        try {
            if ((line = in.readLine()) == null) {
                in.close();
                line = "Nessuna line disponibile";
            }
        } catch (FileNotFoundException e) {
            System.out.println("File non trovato: ");
            e.printStackTrace();
            return line = "File non trovato";
        } catch (IOException e) {
            System.out.println("Problemi nell'estrazione della line: ");
            e.printStackTrace();
            line = "Nessuna line disponibile";
        }
        return line;
    }

    /**
     * Counts the rows which contain at least > `minWords` words.
     * 
     * @param filepath remote filepath
     * @param minWords words threshold
     * @throws RemoteException FileNotFound, IOException
     **/
    public int countRows(String filepath, int minWords) throws RemoteException {
        BufferedReader br = null;
        int wordCount = 0, result = 0;

        System.out.println("[count rows] Counting..");

        try {
            br = new BufferedReader(new FileReader(filepath));
        } catch (IOException e) {
            System.out.println("[count rows] File not found");
            throw new RemoteException(e.getMessage());
        }

        int r;

        try {
            while ((r = br.read()) != -1) {
                char c = (char) r;
                if (Character.isWhitespace(c) || c == '\n' || c == '\r') {
                    wordCount++;

                    if (wordCount > minWords) {
                        result++;
                        wordCount = 0;

                        // skip current line since we already have the number of words that make the
                        // condition true
                        br.readLine();
                    }
                }
            }
            br.close();
        } catch (IOException e) {
            System.out.println("[count rows] Something went wrong while reading the file");
            throw new RemoteException(e.getMessage());
        }

        System.out.println("[count rows] Done!");

        return result;
    }

    /**
     * Deletes a row from the file.
     * 
     * @param filepath remote filepath
     * @param rowNum   row to delete
     * @throws RemoteException FileNotFound, IOException, numero riga troppo
     *                         grande
     * @return status of the operation
     **/
    public int deleteRow(String filepath, int rowNum) throws RemoteException {
        BufferedReader br = null;
        BufferedWriter bw = null;
        String line;
        int count = 0;

        System.out.println("[delete row] Deleting row..");

        try {
            bw = new BufferedWriter(new FileWriter("temp"));
        } catch (IOException e) {
            System.out.println("[delete row] File not found or error while creating temp file");
            throw new RemoteException(e.getMessage());
        }

        // swapping the lines in a new file
        try {
            br = new BufferedReader(new FileReader(filepath));
            count = 0;

            while ((line = br.readLine()) != null) {
                count++;

                if (count == rowNum) {
                    continue;
                }

                bw.write(line + "\n");
            }

            bw.close();
            br.close();
        } catch (IOException e) {
            throw new RemoteException(e.getMessage());
        }

        if (rowNum > count) {
            new File("temp").delete();
            System.out.println("[delete row] Row number is greater than the number of lines in the file");
            throw new RemoteException("Row number is greater than the number of lines in the file");
        }

        // renaming file...
        File file = new File(filepath);
        File tempFile = new File("temp");

        if (!tempFile.renameTo(new File(file.getName() + "_deleted_row"))) {
            System.out.println("[delete row] Error renaming " + filepath);
            throw new RemoteException("Error renaming " + filepath);
        }

        return count;
    }

    /**
     * Lists files in a given directory (and sub-dir) which name has at least 3
     * consonants.
     * 
     * @param filepath
     * @return the list of files in the directory that match the pattern
     * @throws RemoteException
     */
    public String listFilesWithConsonantNames(String filepath) throws RemoteException {
        File f = new File(filepath);
        File[] files = f.listFiles();
        String output = "";

        for (int i = 0; i < files.length; i++) {
            if (files[i].isDirectory()) {
                output += "\t +" + files[i].getAbsolutePath() + "\n";
                output += "\t" + listFilesWithConsonantNames(files[i].getAbsolutePath()) + "\n";
            } else {
                int consonantCount = 0;
                String toLowerCaseFileName = files[i].getName().toLowerCase();

                for (int j = 0; j < (toLowerCaseFileName.length()) && consonantCount < 3; j++) {
                    if (toLowerCaseFileName.charAt(j) != 'a' && toLowerCaseFileName.charAt(j) != 'e'
                            && toLowerCaseFileName.charAt(j) != 'i' && toLowerCaseFileName.charAt(j) != 'o'
                            && toLowerCaseFileName.charAt(j) != 'u') {
                        consonantCount++;
                    }
                }

                if (consonantCount >= 3) {
                    output += "\t -" + files[i].getName() + "\n";
                }
            }
        }

        return output;
    }

    /**
     * Adds a progressive number in front of each odd row of a given file identified
     * by filepath.
     * 
     * @param filepath
     * @return execution status 1=success, 0=failure
     * @throws RemoteException
     */
    public int addNumbersToOddRows(String filepath) throws RemoteException {
        BufferedReader br = null;
        BufferedWriter bw = null;
        int lineNumber = 0;
        String line;

        try {
            br = new BufferedReader(new FileReader(filepath));
            bw = new BufferedWriter(new FileWriter("__temp"));
        } catch (IOException e) {
            System.out.println("File not dound ");
            e.printStackTrace();
            throw new RemoteException(e.getMessage());
        }

        try {
            while ((line = br.readLine()) != null) {
                if (lineNumber % 2 == 0) {
                    bw.write(line + "\n");
                } else {
                    bw.write(lineNumber + " " + line + "\n");
                }

                lineNumber++;
            }
        } catch (Exception e) {
            try {
                br.close();
                bw.close();
            } catch (Exception ee) {
                throw new RemoteException(ee.getMessage());
            }
            throw new RemoteException(e.getMessage());
        }

        try {
            br.close();
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
            throw new RemoteException(e.getMessage());
        }

        // renaming file...
        File file = new File(filepath);
        File tempFile = new File("__temp");

        if (!file.delete()) {
            System.out.println("Error deleting " + filepath);
            throw new RemoteException("Could not delete old file");
        }

        if (!tempFile.renameTo(file)) {
            System.out.println("Error renaming " + filepath);
            throw new RemoteException("Could not rename file");
        }

        return lineNumber;
    }
}