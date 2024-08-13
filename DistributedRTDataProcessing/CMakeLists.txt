cmake_minimum_required(VERSION 3.8)

# Set the project name
project(RTDataProcessing)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src/iot_device)
add_subdirectory(src/data_ingestion)