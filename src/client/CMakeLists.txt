cmake_minimum_required(VERSION 3.10) # Set a minimum CMake version

# Set the project name
project(Client)

# Add executables for client
add_executable(client client.cpp)

# Include directories for found packages
target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/asio
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
