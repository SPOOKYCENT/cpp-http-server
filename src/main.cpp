#include "server.hpp"
#include "core/router.hpp"
#include "routes.hpp"

int main() {
    Router router = setupRouter();
    HttpServer server(8080, router);
    server.start();
    return 0;
}
