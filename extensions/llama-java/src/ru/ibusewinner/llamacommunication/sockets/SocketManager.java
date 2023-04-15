package ru.ibusewinner.llamacommunication.sockets;

import org.json.JSONObject;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class SocketManager {

    private ServerSocket serverSocket;
    private int SERVER_PORT;
    private int JEFF_PORT;
    private int N;
    private double RP;
    private int C;
    private int T;
    private String MODEL_FILE_NAME;

    public SocketManager(int SERVER_PORT, int JEFF_PORT, int N, double RP, int C, int T, String MODEL_FILE_NAME) {
        this.SERVER_PORT = SERVER_PORT;
        this.JEFF_PORT = JEFF_PORT;
        this.N = N;
        this.RP = RP;
        this.C = C;
        this.T = T;
        this.MODEL_FILE_NAME = MODEL_FILE_NAME;
    }

    public void start() {
        try {
            serverSocket = new ServerSocket(SERVER_PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                new ServerThread(clientSocket, this).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void send(String message) {
        try {
            Socket clientSocket = new Socket("localhost", JEFF_PORT);
            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
            JSONObject jsonObject = new JSONObject();
            jsonObject.put("send", message);
            out.write(jsonObject.toString());
            out.flush();
            clientSocket.close();
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void stop() {
        try {
            if (serverSocket != null && serverSocket.isClosed()) serverSocket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public int getN() {
        return N;
    }
    public double getRP() {
        return RP;
    }
    public int getC() {
        return C;
    }
    public int getT() {
        return T;
    }
    public String getModelFileName() {
        return MODEL_FILE_NAME;
    }
}
