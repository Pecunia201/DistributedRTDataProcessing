# src/kafka/producer/CMakeLists.txt
# Find the installed packages
find_package(PkgConfig REQUIRED)

# Create a library for server
add_library(kafka_producer_server STATIC producer.cpp)
target_include_directories(kafka_producer_server PRIVATE
    ${RDKAFKA_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(kafka_producer_server
    ${RDKAFKA_LIBRARIES}
    ${GLIB_LIBRARIES}
)