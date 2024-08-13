#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "kafka_producer/producer.h"

using asio::ip::tcp;
using json = nlohmann::json;

const std::string PREFIX = "[Asio Server] ";

void handleClient(tcp::socket socket) {
    try {
        std::array<char, 256> buf;
        asio::error_code error;

        while (true) {
            // Read JSON message from client
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof) break; // Connection closed cleanly by peer
            else if (error) throw asio::system_error(error); // other error

            // Parse received JSON
            std::string received_data(buf.data(), len);
            try {
                json received_json = json::parse(received_data);

                // Log received JSON
                std::cout << PREFIX << "Received: " << received_json.dump() << std::endl;

                // Produce message to Kafka
                std::string topic = "iot_data";
                std::string key = received_json["sensor_id"];
                std::string value = received_json.dump();

                produce_message(topic, key, value);

                // Send confirmation back to the client
                std::string confirmation_message = "JSON received successfully!";
                asio::write(socket, asio::buffer(confirmation_message), error);
                if (error) throw asio::system_error(error);
            }
            catch (const json::exception& e) {
                // Handle JSON parsing errors
                std::string error_message = "Error: Invalid JSON received.";
                asio::write(socket, asio::buffer(error_message), error);
                if (error) throw asio::system_error(error);
                std::cout << PREFIX << "Error: Invalid JSON received (" << e.what() << ")" << std::endl;
            }
        }
    }
    catch (const asio::system_error& e) {
        std::cout << PREFIX << "Connection error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        // Initialise Kafka outside the main loop
        initialise_kafka();

        // Asio configuration
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

        // Loop forever to accept incoming connections
        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::thread(handleClient, std::move(socket)).detach();
        }
    }
    catch (const std::exception& e) {
        std::cout << PREFIX << "Server error: " << e.what() << std::endl;
        return 1; // Return a non-zero value to indicate an error
    }

    // Kafka cleanup
    cleanup_kafka();
    return 0; // Return 0 to indicate successful completion
}