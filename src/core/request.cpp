#include "request.hpp"
#include <sstream>
#include <unistd.h>

HttpRequest HttpRequest::parse(const std::string& raw, int client_fd) {
    HttpRequest request;

    // parse request line
    size_t first_line_end = raw.find("\r\n");
    std::string firstLine = raw.substr(0, first_line_end);
    std::istringstream line_stream(firstLine);
    line_stream >> request.m_method >> request.m_path >> request.m_http_version;

    // parse request headers
    size_t pos = raw.find("\r\n") + 2;
    while (pos<raw.size()) {
        size_t line_end = raw.find("\r\n", pos);
        if (line_end==std::string::npos || line_end==pos) break;

        std::string line = raw.substr(pos, line_end-pos);
        size_t colon = line.find(":");
        if (colon!=std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon+1);
            value.erase(0, value.find_first_not_of(" \t"));
            request.m_headers[key] = value;
        }
        pos = line_end+2;
    }

    // parse request body
    size_t content_length = 0;   
    if (request.m_headers.find("Content-Length")!=request.m_headers.end()) content_length = std::stoi(request.m_headers["Content-Length"]);

    size_t header_end = raw.find("\r\n\r\n");
    std::string initial_body;
    if (header_end!=std::string::npos) initial_body = raw.substr(header_end+4);

    while (initial_body.length()<content_length) {
        char temp[1024];
        ssize_t n = read(client_fd, temp, sizeof(temp));
        if (n<=0) break;
        initial_body.append(temp, n);
    }

    request.m_body = std::move(initial_body);
    
    return request;
}

const std::string& HttpRequest::method() const       { return m_method; }
const std::string& HttpRequest::path() const         { return m_path; }
const std::string& HttpRequest::httpVersion() const  { return m_http_version; }
const std::map<std::string, std::string>& HttpRequest::headers() const { return m_headers; }
const std::string& HttpRequest::body() const         { return m_body; }
