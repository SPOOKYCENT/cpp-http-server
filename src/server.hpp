#pragma once

class HttpServer {
public:
    HttpServer(int port);
    void start();

private:
    int server_fd;
    int port;

    void setupSocket();
};
