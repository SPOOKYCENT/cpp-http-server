#pragma once

#include "core/router.hpp"
#include <string>

class HttpServer {
public:
    HttpServer(int port, const Router&);
    void start();
    void handleClient(int client_fd, std::string client_ip);

private:
    int server_fd;
    int port;
    const Router& router;

    void setupSocket();
};
