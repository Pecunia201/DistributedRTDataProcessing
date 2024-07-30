# src/CMakeLists.txt
# Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(iot_device)
add_subdirectory(asio_server)
add_subdirectory(kafka/producer)
add_subdirectory(kafka/consumer)

# Add the main executable
add_executable(main main.cpp)

# Link libraries
target_link_libraries(main iot_device_client asio_server kafka_producer_server kafka_consumer_server)