#include <client.h>
#include <iostream>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

using asio::ip::tcp;
using json = nlohmann::json;

// Function to generate random float within a range
float generate_random_float(float min, float max) {
    static std::default_random_engine rng(std::time(nullptr));
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// Function to generate the current timestamp in ISO 8601 format
std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::gmtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

// Function to generate IoT data with random float values
json generate_iot_data(int sensor_id) {
    json data_to_send;
    data_to_send["timestamp"] = get_current_timestamp();
    data_to_send["sensor_id"] = "sensor_" + std::to_string(sensor_id);
    data_to_send["temperature"] = generate_random_float(20.0, 30.0);
    data_to_send["humidity"] = generate_random_float(40.0, 60.0);
    data_to_send["pressure"] = generate_random_float(900.0, 1100.0);
    data_to_send["aqi"] = generate_random_float(0.0, 50.0);
    return data_to_send;
}

bool connection_closed = false;

void send_json(tcp::socket& socket, int sensor_id) {
    try {
        // Construct JSON data
        json data_to_send = generate_iot_data(sensor_id);
        std::string json_string = data_to_send.dump();

        // Print JSON string with PREFIX
        std::cout << "\033[36m[IoT Device " << sensor_id << "]\033[0m Sending JSON to server: " << json_string << std::endl;

        // Send JSON string to server
        asio::write(socket, asio::buffer(json_string));

        // Read confirmation message from server
        std::array<char, 128> confirmation_buf;
        std::error_code error;
        size_t confirmation_len = socket.read_some(asio::buffer(confirmation_buf), error);

        // Print confirmation message
        if (!error) {
            //std::cout << PREFIX << " ";
            //std::cout.write(confirmation_buf.data(), confirmation_len);
            //std::cout << std::endl;
        }
        else {
            // Handle errors
            if (error == asio::error::connection_reset || error == asio::error::eof) {
                std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Connection closed by server." << std::endl;
                connection_closed = true;
            }
            else {
                std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Error: " << error.message() << std::endl;
            }
        }
    }
    catch (std::system_error& e) {
        std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Exception: " << e.what() << std::endl;
    }
}

void setupIoTDeviceSimulator(int sensor_id) {
    asio::io_context io_context;
    tcp::resolver resolver(io_context);

    while (true) {
        try {
            // Resolve and attempt to connect to the server
            tcp::resolver::results_type endpoints = resolver.resolve("localhost", "1234");
            tcp::socket socket(io_context);
            asio::connect(socket, endpoints);

            // Print message indicating connection established
            std::cout << "\033[36m[IoT Device " << sensor_id << "]\033[0m Connection established with server." << std::endl;

            // Connection successful, enter the sending loop
            while (!connection_closed) {
                send_json(socket, sensor_id);
                //std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds
            }

            // If the connection was closed, log it and attempt to reconnect
            std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Connection closed by server. Reconnecting..." << std::endl;

        }
        catch (const std::exception& e) {
            // If there is an error during connection, log it and wait before retrying
            std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Connection error: " << e.what() << std::endl;
            std::cerr << "\033[36m[IoT Device " << sensor_id << "]\033[0m Retrying in 2 seconds..." << std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait before retrying
        }
    }
}