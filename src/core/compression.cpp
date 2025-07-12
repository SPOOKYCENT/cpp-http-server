#include "compression.hpp"
#include <zlib.h>
#include <stdexcept>

bool acceptGzipCompression(const HttpRequest& request) {
    auto it = request.headers().find("Accept-Encoding");
    return (it!=request.headers().end() && it->second.find("gzip")!=std::string::npos);
}

std::string gzipCompress(const std::string& input) {
    z_stream zs{};
    if (deflateInit2(&zs, Z_BEST_COMPRESSION,Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY)!=Z_OK)
        throw std::runtime_error("deflateInit2 failed");

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    zs.avail_in = input.length();

    std::string output;
    char buffer[4096];
    do {
        zs.next_out = reinterpret_cast<Bytef*>(buffer);
        zs.avail_out = sizeof(buffer);

        int ret = deflate(&zs, Z_FINISH);
        if (ret!=Z_OK && ret!=Z_STREAM_END) break;

        output.append(buffer, sizeof(buffer)-zs.avail_out);
    } while (zs.avail_out==0);

    deflateEnd(&zs);
    return output;
}