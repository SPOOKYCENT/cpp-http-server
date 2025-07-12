#pragma once

#include "../core/view.hpp"
#include "../core/request.hpp"
#include "../core/response.hpp"

class RootView : public View {
public:
    HttpResponse get(const HttpRequest&) const override;
};

class HelloView : public View {
public: 
    HttpResponse get(const HttpRequest&) const override;
};

class EchoView : public View {
public: 
    HttpResponse get(const HttpRequest&) const override;
    HttpResponse post(const HttpRequest&) const override;
};

class FileView : public View {
public: 
    HttpResponse get(const HttpRequest&) const override;
    HttpResponse post(const HttpRequest&) const override;
};