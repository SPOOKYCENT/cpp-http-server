#include "server.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>

HttpServer::HttpServer(int port) : port(port), server_fd(-1) {}

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

    std::cout << "✅ Listening on port " << port << std::endl;
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

        char buffer[1024] = {0};
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read<=0) {
            close(client_fd);
            return;
        }
        std::string request(buffer);
        std::cout << "📥 Received:\n" << request << std::endl;

        size_t first_line_end = request.find("\r\n");
        std::string firstLine = request.substr(0, first_line_end);

        std::string method, path, http_version;
        std::istringstream iss(firstLine);
        iss >> method >> path >> http_version;

        std::cout << "🔍 Method: " << method << ", Path: " << path << ", HTTP Version: " << http_version << std::endl;

        std::map<std::string, std::string> headers;

        size_t pos = request.find("\r\n") + 2;
        while (pos<request.size()) {
            size_t line_end = request.find("\r\n", pos);
            if (line_end==std::string::npos || line_end==pos) break;

            std::string line = request.substr(pos, line_end-pos);
            size_t colon = line.find(":");
            if (colon!=std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon+1);
                value.erase(0, value.find_first_not_of(" \t"));
                headers[key] = value;
            }
            pos = line_end+2;
        }

        std::cout << "📋 Headers:" << std::endl;
        for (const auto& [key, value] : headers)
            std::cout << "  " << key << ": " << value << std::endl;

        std::string status_line_200 = "HTTP/1.1 200 OK\r\n";
        std::string status_line_404 = "HTTP/1.1 404 Not Found\r\n";

        std::string status_line, body;
        if (path=="/") {
            status_line = status_line_200;
            body = "Welcome to the root!";
        }
        else if (path=="/hello") {
            status_line = status_line_200;
            body = "Hello!";
        } 
        else if (path.rfind("/echo/", 0)==0) {
            status_line = status_line_200;
            std::string toEcho = path.substr(6);
            body = toEcho.empty() ? "Nothing to echo" : toEcho;
        }
        else {
            status_line = status_line_404;
            body = "Unknown path: " + path;
        }


        std::string response =
            status_line +
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        std::cout << "📤 Sending:\n" << response << std::endl;

        send(client_fd, response.c_str(), response.length(), 0);
        close(client_fd);
    }
}
