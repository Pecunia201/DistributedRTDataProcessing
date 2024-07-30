#include <iostream>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>

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
json generate_iot_data() {
    json data_to_send;
    data_to_send["timestamp"] = get_current_timestamp();
    data_to_send["sensor_id"] = "sensor_002";
    data_to_send["temperature"] = generate_random_float(20.0, 30.0);
    data_to_send["humidity"] = generate_random_float(40.0, 60.0);
    data_to_send["pressure"] = generate_random_float(900.0, 1100.0);
    data_to_send["aqi"] = generate_random_float(0.0, 50.0);
    return data_to_send;
}

bool connection_closed = false;

void send_json(tcp::socket& socket) {
    try {
        // Construct JSON data
        json data_to_send = generate_iot_data();
        std::string json_string = data_to_send.dump();

        // Print JSON string
        std::cout << "Sending JSON to server: " << json_string << std::endl;

        // Send JSON string to server
        asio::write(socket, asio::buffer(json_string));

        // Read confirmation message from server
        std::array<char, 128> confirmation_buf;
        std::error_code error;
        size_t confirmation_len = socket.read_some(asio::buffer(confirmation_buf), error);

        if (!error) {
            std::cout.write(confirmation_buf.data(), confirmation_len);
            std::cout << std::endl;
        }
        else {
            // Handle errors
            if (error == asio::error::connection_reset || error == asio::error::eof) {
                std::cerr << "Connection closed by server." << std::endl;
                connection_closed = true;
            }
            else {
                std::cerr << "Error: " << error.message() << std::endl;
            }
        }
    }
    catch (std::system_error& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "1234");

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        // Loop to send JSON every 2 seconds until the connection is closed
        while (!connection_closed) {
            send_json(socket);
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 2 seconds
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
