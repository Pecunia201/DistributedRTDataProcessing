# Distributed Real-Time Data Processing System

## Introduction
This project is a C++ implementation of a distributed real-time data processing system. It consists of a server component that listens for incoming JSON data from clients and a client component that sends simulated IoT data to the server.

## Setup Instructions
1. **Boost Installation:** Download and build Boost version 1.84.0 from [the official Boost website](https://www.boost.org/users/history/).
2. **Add Boost:** Place the built `boost_1_84_0` into the `/src/include` directory of this project.
3. **nlohmann's JSON:** The project already includes nlohmann's `json.hpp` library for JSON handling.

## Usage
1. **Server:** Compile and run `server.cpp` to start the server. It listens for incoming JSON data on port `1234`.
2. **Client:** Compile and run `client.cpp` to simulate IoT data and send it to the server.

## Project Roadmap (WIP)
![Alt Text](images/flowchart.jpg)

## Credits
- [Boost C++ Libraries](https://www.boost.org/)
- [nlohmann/json](https://github.com/nlohmann/json) for JSON handling
