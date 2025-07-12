#pragma once

#include "request.hpp"
#include <string>

bool acceptGzipCompression(const HttpRequest& request);
std::string gzipCompress(const std::string& input);