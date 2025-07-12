#include "views.hpp"
#include <fstream>
#include <filesystem>

// RootView
HttpResponse RootView::get(const HttpRequest& request) const {
    HttpResponse response;
    response.setStatus(200);
    response.setBody("Welcome to the root!");
    return response;
}

// HelloView
HttpResponse HelloView::get(const HttpRequest& request) const {
    HttpResponse response;
    response.setStatus(200);
    response.setBody("Hello!");
    return response;
}

// EchoView
HttpResponse EchoView::get(const HttpRequest& request) const {
    HttpResponse response;
    response.setStatus(200);
    std::string to_echo = request.path().substr(6);
    response.setBody(to_echo.empty() ? "Nothing to echo" : to_echo);
    return response;
}
HttpResponse EchoView::post(const HttpRequest& request) const {
    HttpResponse response;
    response.setStatus(200);
    response.setBody(request.body());
    return response;
}

// FileView
HttpResponse FileView::get(const HttpRequest& request) const {
    HttpResponse response;
    std::string file_name = request.path().substr(6);
    std::string file_path = "public/" + file_name;
    std::string body;
    if (std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path)) {
        response.setStatus(200);
        std::ifstream file(file_path);
        body.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    else {
        response.setStatus(404);
        body = "404 Not Found: " + file_name;
    }
    response.setBody(body);
    return response;
}
HttpResponse FileView::post(const HttpRequest& request) const {
    HttpResponse response;
    response.setStatus(201);
    std::string file_name = request.path().substr(6);
    std::string file_path = "public/" + file_name;
    std::ofstream outfile(file_path);
    std::string body;
    if (!outfile.is_open()) {
        response.setStatus(500);
        body = "500 Internal Server Error: Failed to open file.";
    }
    else {
        outfile << request.body();
        if (outfile.fail()) {
            response.setStatus(500);
            body = "500 Internal Server Error: Failed to write file.";
        }
        else {
            response.setStatus(201);
            body = "Uploaded to: " + file_name;
        }
        outfile.close();
    }
    response.setBody(body);
    return response;
}



