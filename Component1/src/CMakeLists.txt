# src/CMakeLists.txt
# Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(iot_device)
add_subdirectory(data_ingestion/asio_receiver)
add_subdirectory(data_ingestion/kafka_producer)

# Add the main executable
add_executable(main main.cpp)

# Link libraries
target_link_libraries(main iot_device_client asio_server kafka_producer_server)