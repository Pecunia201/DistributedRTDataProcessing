#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include <librdkafka/rdkafkacpp.h>

using asio::ip::tcp;
using json = nlohmann::json;

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
                std::cout << "Received: " << received_json.dump() << std::endl;

                // Produce the received JSON message to Kafka
                //

                // Send confirmation message back to client
                std::string confirmation_message = "JSON received successfully!";
                asio::write(socket, asio::buffer(confirmation_message), error);
                if (error) throw asio::system_error(error);
                std::cout << "Sent confirmation message to client." << std::endl;
            }
            catch (const std::exception& e) {
                // If parsing fails, send an error message back to the client
                std::string error_message = "Error: Invalid JSON received.";
                asio::write(socket, asio::buffer(error_message), error);
                if (error) throw asio::system_error(error);
                std::cerr << "Error: Invalid JSON received" << std::endl;
            }
        }
    }
    catch (const asio::system_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    try {
        // Kafka configuration

        // Boost configuration
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

        // Loop forever to accept incoming connections
        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::thread(handle_client, std::move(socket)).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}