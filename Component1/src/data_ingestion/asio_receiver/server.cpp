#include <server.h>
#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "../kafka_producer/producer.h"

using asio::ip::tcp;
using json = nlohmann::json;

const std::string PREFIX = "\033[32m[Asio Server]\033[0m ";

void handle_client(tcp::socket socket) {
    try {
        std::array<char, 256> buf;
        asio::error_code error;

        for (;;) {
            // Read JSON message from client
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof)
                break; // Connection closed cleanly by peer
            else if (error)
                throw asio::system_error(error); // Some other error

            // Parse received JSON
            std::string received_data(buf.data(), len);
            try {
                json received_json = json::parse(received_data);

                // Print received JSON
                std::cout << PREFIX << "Received: " << received_json.dump() << std::endl;

                // Produce message to Kafka
                std::string topic = "iot_data";
                std::string key = received_json["sensor_id"];
                std::string value = received_json.dump();

                produce_message(topic, key, value);

                // Send confirmation message back to client
                std::string confirmation_message = "JSON received successfully!";
                asio::write(socket, asio::buffer(confirmation_message), error);
                if (error) throw asio::system_error(error);
                //std::cout << PREFIX << "Sent confirmation message back to IoT Device." << std::endl;
            }
            catch (const std::exception& e) {
                // If parsing fails, send an error message back to the client
                std::string error_message = "Error: Invalid JSON received.";
                asio::write(socket, asio::buffer(error_message), error);
                if (error) throw asio::system_error(error);
                std::cout << PREFIX << "Error: Invalid JSON received" << std::endl;
            }
        }
    }
    catch (const asio::system_error& e) {
        std::cout << PREFIX << e.what() << std::endl;
    }
}

void setupASIOServer() {
    try {
        // Initialise Kafka
        initialise_kafka();

        // Asio configuration
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

        // Loop forever to accept incoming connections
        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::thread(handle_client, std::move(socket)).detach();
        }

        // Clean up Kafka
        cleanup_kafka();
    }
    catch (std::exception& e) {
        std::cout << PREFIX << e.what() << std::endl;
    }
}