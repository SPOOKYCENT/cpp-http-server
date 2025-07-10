#pragma once

#include <string>

class HttpServer {
public:
    HttpServer(int port);
    void start();
    void handleClient(int client_fd, std::string client_ip);

private:
    int server_fd;
    int port;

    void setupSocket();
};
