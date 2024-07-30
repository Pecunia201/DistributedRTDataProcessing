# src/asio_server/CMakeLists.txt
# Create a library for client
add_library(asio_server STATIC server.cpp)
target_include_directories(asio_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})