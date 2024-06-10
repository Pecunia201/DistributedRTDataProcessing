# Set the project name
project(Client)

# Add the directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/boost_1_84_0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Add executables for client
add_executable(client client.cpp)