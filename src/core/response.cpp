#include "response.hpp"
#include <sstream>
#include <unordered_map>

void HttpResponse::setStatus(int code, const std::string& message) {
    m_status_code = code;
    m_status_message = message;
    m_headers["Content-Type"] = "text/plain";
}

void HttpResponse::setStatus(int code) {
    static const std::unordered_map<int, std::string> statusText = {
        {200, "OK"},
        {201, "Created"},
        {204, "No Content"},
        {400, "Bad Request"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {503, "Service Unavailable"},
    };

    m_status_code = code;
    auto it = statusText.find(code);
    m_status_message = (it != statusText.end()) ? it->second : "Unknown";
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    m_headers[key] = value;
}

void  HttpResponse::setBody(const std::string& body) {
    m_body = body;
    m_headers["Content-Length"] = std::to_string(m_body.size());
}

int HttpResponse::statusCode() const { return m_status_code; }

std::string HttpResponse::toString() const {
    std::ostringstream res;
    res << "HTTP/1.1 " << m_status_code << " " << m_status_message << "\r\n";
    for (const auto& [key, value] : m_headers) {
        res << key << ": " << value << "\r\n";
    }
    res << "\r\n" << m_body;
    return res.str();
}

const std::string HttpResponse::body() const { return m_body; }
