#include "router.hpp"
#include <fstream>
#include <filesystem>

void Router::mount(const std::string& path, std::unique_ptr<View> view) {
    routes[path] = std::move(view);
}

HttpResponse Router::handle(const HttpRequest& request) const {
    
    for (const auto& [prefix, view] : routes) {
        if (request.path().rfind(prefix, 0)==0) {
            const std::string& method = request.method();

            if (method=="GET") return view->get(request);
            if (method=="POST") return view->post(request);
            if (method=="PUT") return view->put(request);
            if (method=="PATCH") return view->patch(request);
            if (method=="DELETE") return view->del(request);

            HttpResponse response;
            response.setStatus(405);
            response.setBody("405 Method Not Allowed");
            return response;
        }
    }

    HttpResponse response;
    response.setStatus(404);
    response.setBody("404 Not Found");
    return response;
}