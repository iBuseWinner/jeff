package ru.ibusewinner.llamacommunication;

import ru.ibusewinner.llamacommunication.sockets.SocketManager;

public class LlamaCommunication {

    public static void main(String[] args) {
        //Extension args
        int SERVER_PORT = Integer.parseInt(args[0]);
        int JEFF_PORT = Integer.parseInt(args[1]);

        //Llama args
        int n = Integer.parseInt(args[2]); //n??
        double RP = Double.parseDouble(args[3]); //Repeat penalty
        int c = Integer.parseInt(args[4]); //c??
        int t = Integer.parseInt(args[5]); //t??
        String MODEL_FILE_NAME = args[6]; //Model file name

        //Sockets
        SocketManager socketManager = new SocketManager(SERVER_PORT, JEFF_PORT, n, RP, c, t, MODEL_FILE_NAME);
        socketManager.start();
    }

}
