cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(http_server
  src/main.cpp
  src/server.cpp
  src/request.cpp
  src/response.cpp
  src/routes.cpp
  src/core/router.cpp
  src/views/views.cpp
)
