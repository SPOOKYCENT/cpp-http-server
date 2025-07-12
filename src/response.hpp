#pragma once

#include <string>
#include <map>

class HttpResponse {
public:
    void setStatus(int code, const std::string& message);
    void setStatus(int code);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    int statusCode() const;
    std::string toString() const;

private:
    int m_status_code = 200;
    std::string m_status_message = "OK";
    std::map<std::string, std::string> m_headers;
    std::string m_body;
};