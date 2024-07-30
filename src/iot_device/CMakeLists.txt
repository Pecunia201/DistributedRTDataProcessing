# src/iot_device/CMakeLists.txt
# Create a library for client
add_library(iot_device_client STATIC client.cpp)
target_include_directories(iot_device_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})