#include "routes.hpp"
#include "views/views.hpp"
#include <memory>

Router setupRouter() {
    Router r;
    // r.mount("/", std::make_unique<RootView>());
    r.mount("/hello", std::make_unique<HelloView>());
    r.mount("/echo/", std::make_unique<EchoView>());
    r.mount("/file/", std::make_unique<FileView>());
    return r;
}