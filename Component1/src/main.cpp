#include <iostream>
#include <thread>
#include <mutex>
#include <exception>
#include "iot_device/client.h"
#include "data_ingestion/asio_receiver/server.h"

// Mutex for synchronising console output
std::mutex console_mutex;

// Function to print messages safely
void printSafe(const std::string& message) {
    std::lock_guard<std::mutex> lock(console_mutex);
    std::cout << message << std::endl;
}

// Function to run the ASIO server
void runASIOServer() {
    try {
        setupASIOServer();
    }
    catch (const std::exception& e) {
        printSafe("ASIO server error: " + std::string(e.what()));
    }
}

// Function to run the IoT device simulator with a given sensor ID
void runIoTDeviceSimulator(int sensor_id) {
    try {
        setupIoTDeviceSimulator(sensor_id);
    }
    catch (const std::exception& e) {
        printSafe("IoT device simulator error for sensor " + std::to_string(sensor_id) + ": " + std::string(e.what()));
    }
}

int main() {
    try {
        // Create threads to run the server and Kafka consumer
        std::thread asioThread(runASIOServer);

        // Create and pass unique integer parameters to the IoT device simulator threads
        std::thread iotThread1([]() { runIoTDeviceSimulator(1); });
        std::thread iotThread2([]() { runIoTDeviceSimulator(2); });
        std::thread iotThread3([]() { runIoTDeviceSimulator(3); });

        // Wait for all threads to complete
        asioThread.join();
        iotThread1.join();
        iotThread2.join();
        iotThread3.join();

        printSafe("System initialised. Running...");
    }
    catch (const std::exception& e) {
        printSafe("An error occurred: " + std::string(e.what()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}