#include <iostream>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <vector>

using asio::ip::tcp;
using json = nlohmann::json;

constexpr char SERVER_HOST[] = "localhost";
constexpr char SERVER_PORT[] = "1234";

// Define a constant log prefix
constexpr char PREFIX[] = "[IoT Device] ";

// Generate a random float within a range
float generateRandomFloat(float min, float max) {
    static std::default_random_engine rng(std::time(nullptr));
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// Generate the current timestamp in ISO 8601 format
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::gmtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

// Generate IoT data with random float values
json generateIoTData(int sensor_id) {
    return {
        {"timestamp", getCurrentTimestamp()},
        {"sensor_id", "sensor_" + std::to_string(sensor_id)},
        {"temperature", generateRandomFloat(20.0, 30.0)},
        {"humidity", generateRandomFloat(40.0, 60.0)},
        {"pressure", generateRandomFloat(900.0, 1100.0)},
        {"aqi", generateRandomFloat(0.0, 50.0)}
    };
}

// Function to handle the IoT device communication
void handleIoTDevice(int sensor_id) {
    asio::io_context io_context;
    tcp::resolver resolver(io_context);

    while (true) {
        try {
            // Resolve and connect to the server
            auto endpoints = resolver.resolve(SERVER_HOST, SERVER_PORT);
            tcp::socket socket(io_context);
            asio::connect(socket, endpoints);

            std::cout << PREFIX << sensor_id << ": Connection established with server." << std::endl;

            while (true) {
                try {
                    // Generate and send JSON data
                    json data_to_send = generateIoTData(sensor_id);
                    std::string json_string = data_to_send.dump();

                    std::cout << PREFIX << sensor_id << ": Sending JSON: " << json_string << std::endl;
                    asio::write(socket, asio::buffer(json_string));

                    // Read confirmation from server
                    std::array<char, 128> confirmation_buf;
                    std::error_code error;
                    size_t confirmation_len = socket.read_some(asio::buffer(confirmation_buf), error);

                    if (error == asio::error::connection_reset || error == asio::error::eof) {
                        std::cerr << PREFIX << sensor_id << ": Connection closed by server." << std::endl;
                        break;  // Exit inner loop to reconnect
                    }

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                catch (const std::system_error& e) {
                    std::cerr << PREFIX << sensor_id << ": Error: " << e.what() << std::endl;
                    break;  // Exit inner loop to reconnect
                }
            }

            std::cerr << PREFIX << sensor_id << ": Reconnecting..." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << PREFIX << sensor_id << ": Connection error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));  // Wait before retrying
        }
    }
}

int main() {
    // Create and start threads for each sensor
    std::vector<std::thread> threads;
    for (int sensor_id = 1; sensor_id <= 3; ++sensor_id) {
        threads.emplace_back(handleIoTDevice, sensor_id);
    }

    // Join the threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}