cmake_minimum_required(VERSION 3.10)
project(WebSocketServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/game)
add_subdirectory(src/infra)

# Add executable
add_executable(websocket_server main.cpp)

target_link_libraries(websocket_server PRIVATE game)
target_link_libraries(websocket_server PRIVATE infra)