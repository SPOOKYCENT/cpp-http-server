#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <map>
#include <thread>
#include <fstream>
#include <filesystem>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

HttpServer::HttpServer(int port, const Router& router) : port(port), server_fd(-1), router(router) {}

void HttpServer::setupSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "🚀 Server listening on port " << port << std::endl;
}

void HttpServer::start() {
    setupSocket();

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

        std::thread(&HttpServer::handleClient, this, client_fd, std::string(client_ip)).detach();
    }
}

void HttpServer::handleClient(int client_fd, std::string client_ip) {
    char buffer[1024] = {0};
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read<=0) {
        close(client_fd);
        return;
    }
    std::string raw_request(buffer);

    HttpRequest request = HttpRequest::parse(raw_request, client_fd);
    HttpResponse response = router.handle(request);
    std::string response_str = response.toString();

    send(client_fd, response_str.c_str(), response_str.length(), 0);
    close(client_fd);
}
