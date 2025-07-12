#include "router.hpp"
#include "compression.hpp"
#include <fstream>
#include <filesystem>

void Router::mount(const std::string& path, std::unique_ptr<View> view) {
    routes[path] = std::move(view);
}

HttpResponse Router::handle(const HttpRequest& request) const {
    HttpResponse response;
    for (const auto& [prefix, view] : routes) {
        if (request.path().rfind(prefix, 0)==0) {
            const std::string& method = request.method();

            if (method=="GET") response = view->get(request);
            else if (method=="POST") response = view->post(request);
            else if (method=="PUT") response = view->put(request);
            else if (method=="PATCH") response = view->patch(request);
            else if (method=="DELETE") response = view->del(request);
            else {
                response.setStatus(405);
                response.setBody("405 Method Not Allowed");
                return response;
            }
            if (acceptGzipCompression(request)) {
                std::string compressed = gzipCompress(response.body());
                response.setBody(compressed);
                response.setHeader("Content-Encoding", "gzip");
            } 
            response.setHeader("Content-Length", std::to_string(response.body().size()));
            return response;
        }
    }

    response.setStatus(404);
    response.setBody("404 Not Found");
    return response;
}