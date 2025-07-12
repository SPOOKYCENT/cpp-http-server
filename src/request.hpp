#pragma once

#include <string>
#include <map>

class HttpRequest {
public:
    static HttpRequest parse(const std::string& raw, int client_fd);
    
    const std::string& method() const;
    const std::string& path() const;
    const std::string& httpVersion() const;
    const std::map<std::string, std::string>& headers() const;
    const std::string& body() const;

private:
    std::string m_method;
    std::string m_path;
    std::string m_http_version;
    std::map<std::string, std::string> m_headers;
    std::string m_body;
};