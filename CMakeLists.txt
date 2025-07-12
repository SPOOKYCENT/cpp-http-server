cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)

find_package(ZLIB REQUIRED)

add_executable(http_server
  src/main.cpp
  src/server.cpp
  src/core/request.cpp
  src/core/response.cpp
  src/core/routes.cpp
  src/core/router.cpp
  src/core/compression.cpp
  src/views/views.cpp
)

target_link_libraries(http_server PRIVATE ZLIB::ZLIB)