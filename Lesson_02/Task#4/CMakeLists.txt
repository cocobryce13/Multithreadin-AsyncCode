cmake_minimum_required(VERSION 3.12)
project(tcp_sockets_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tcp_server src/server.cpp)
add_executable(tcp_client src/client.cpp)

if (WIN32)
  target_link_libraries(tcp_server ws2_32)
  target_link_libraries(tcp_client ws2_32)
endif()