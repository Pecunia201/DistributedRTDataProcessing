cmake_minimum_required(VERSION 3.10) # Set a minimum CMake version

# Set the project name
project(Server)

# Find the installed packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(RDKAFKA REQUIRED rdkafka)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# Add executables for server
add_executable(server server.cpp)

# Include directories for found packages
target_include_directories(server PRIVATE
    ${RDKAFKA_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/asio
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link the libraries to the executable
target_link_libraries(server
    ${RDKAFKA_LIBRARIES}
    ${GLIB_LIBRARIES}
)
