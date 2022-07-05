import java.io.BufferedReader;

public class CliUtils {
    public static int getNumber(BufferedReader stdIn) {
        boolean isNum = false;
        int num = 0;

        while (!isNum) {
            try {
                num = Integer.parseInt(stdIn.readLine().trim());
                isNum = true;
            } catch (NumberFormatException e) {
                System.out.println("Please try again");
                isNum = false;
            } catch (Exception e) {
                System.out.println("Something went wrong while reading the number from stdin, please try again");
                isNum = false;
            }
        }

        return num;
    }
}
