#pragma once

#include "../request.hpp"
#include "../response.hpp"
#include "view.hpp"
#include <memory>

class Router {
public:
    void mount(const std::string& path, std::unique_ptr<View> view);
    HttpResponse handle(const HttpRequest& request) const;

private:
    std::map<std::string, std::unique_ptr<View>> routes;
};