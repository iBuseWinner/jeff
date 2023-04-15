package ru.ibusewinner.llamacommunication.sockets;

import org.json.JSONObject;

import java.io.*;
import java.net.Socket;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

public class ServerThread extends Thread {
    private Socket socket;
    private SocketManager socketManager;

    public ServerThread(Socket socket, SocketManager socketManager) {
        this.socket = socket;
        this.socketManager = socketManager;
    }

    @Override
    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            String message = in.lines().collect(Collectors.joining(System.lineSeparator()));
            if (message != null) {
                JSONObject jsonObject = new JSONObject(message);
                int contentType = jsonObject.getInt("content_type");
                if (contentType == 1 || contentType == 2) {
                    String content = jsonObject.getString("content");

                    ProcessBuilder processBuilder = new ProcessBuilder(
                            "llama/main.exe",
                            "-m", "llama/models/"+socketManager.getModelFileName(),
                            "-t", String.valueOf(socketManager.getT()),
                            "-n", String.valueOf(socketManager.getN()),
                            "-c", String.valueOf(socketManager.getC()),
                            "--repeat_penalty", String.valueOf(socketManager.getRP()),
                            "-i",
                            "-r", "\"### Human:\"",
                            "-p", "\"### Human: "+content+" ### Assistant:\"");

                    File outputFile = new File("output.txt");
                    if (outputFile.exists()) {
                        outputFile.delete();
                    }
                    outputFile.createNewFile();

                    processBuilder.redirectOutput(outputFile);
//                    processBuilder.redirectError(ProcessBuilder.Redirect.INHERIT);
                    Process process = processBuilder.start();

                    ScheduledExecutorService scheduler = Executors.newScheduledThreadPool(1);
                    Runnable runnable = () -> {
                        String response = readLine(process);
                        boolean exit = response.endsWith("### Human:");
                        if (exit) {
                            socketManager.send(response
                                    .replace("### Human:", "")
                                    .replace("### Assistant:", "")
                                    .replace(content, ""));
                            process.destroy();
                            scheduler.shutdown();
                        }
                    };
                    scheduler.scheduleAtFixedRate(runnable, 0, 1, TimeUnit.SECONDS);

                }
            }

            socket.close();
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private String readLine(Process process) {
        File outputFile = new File("output.txt");
        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(outputFile));
            String output = bufferedReader.lines().collect(Collectors.joining(System.lineSeparator()));
            bufferedReader.close();
            return output;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return "";
    }
}
