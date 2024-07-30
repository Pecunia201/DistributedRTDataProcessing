# Distributed Real-Time Data Processing System

## Introduction
This project is a C++ implementation of a distributed real-time data processing system. It consists of a server component that listens for incoming JSON data from clients and a client component that sends simulated IoT data to the server.

## Setup Instructions
1. **Add Asio standalone:** Download `asio_1_30_2` from [their website](https://think-async.com/Asio/Download.html). Place the `asio.hpp` file and `asio` folder in a `/src/include` directory of this project.
2. **nlohmann's JSON:** Download `include.zip` from [GitHub releases](https://github.com/nlohmann/json). Place the `nlohmann` folder containing header.hpp into a `/src/include` directory of this project.

## Usage
1. **Server:** Compile and run `server.cpp` to start the server. It listens for incoming JSON data on port `1234`.
2. **Client:** Compile and run `client.cpp` to simulate IoT data and send it to the server.

## Project Roadmap (WIP)
![Alt Text](images/flowchart.jpg)

### Features implemented
- [x] Client
- [x] TCP Server
- [ ] Kafka Producer
- [ ] Real-Time Data Processing Logic
- [ ] InfluxDB
- [ ] Performance Metrics (Prometheus/Grafana)

## Credits
- [Boost C++ Libraries](https://www.boost.org/)
- [nlohmann/json](https://github.com/nlohmann/json) for JSON handling
