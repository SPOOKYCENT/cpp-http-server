#pragma once

#include "request.hpp"
#include "response.hpp"

class View {
public:
    virtual HttpResponse get(const HttpRequest&) const {
        return methodNotAllowed("GET");
    }
    virtual HttpResponse post(const HttpRequest&) const {
        return methodNotAllowed("POST");
    }
    virtual HttpResponse put(const HttpRequest&) const {
        return methodNotAllowed("PUT");
    }
    virtual HttpResponse patch(const HttpRequest&) const {
        return methodNotAllowed("PATCH");
    }
    virtual HttpResponse del(const HttpRequest&) const {
        return methodNotAllowed("DELETE");
    }
    virtual ~View() = default;
    

protected:
    HttpResponse methodNotAllowed(const std::string& method) const {
        HttpResponse response;
        response.setStatus(405);
        response.setHeader("Content-Lenght", "text/plain");
        response.setBody("405 Method not allowed: " + method);
        return response;      
    }
};