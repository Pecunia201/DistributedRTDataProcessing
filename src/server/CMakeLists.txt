# Set the project name
project(Server)

# Add the directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/boost_1_84_0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Add executables for server
add_executable(server server.cpp)