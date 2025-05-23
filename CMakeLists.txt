cmake_minimum_required(VERSION 3.10)
project(NetworkProtocolAnalyzer)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)
include_directories("C:/npcap-sdk-1.15/Include")

# Link directories
link_directories("C:/npcap-sdk-1.15/Lib/x64")

# Add source files to executable
add_executable(NetworkProtocolAnalyzer 
    src/main.cpp 
    src/PacketCapture.cpp
    src/NetworkVisualizer.cpp
)

# Add demo executable
add_executable(Demo
    src/demo.cpp
    src/NetworkVisualizer.cpp
)

# Link Npcap libraries
target_link_libraries(NetworkProtocolAnalyzer wpcap Packet ws2_32)
target_link_libraries(Demo ws2_32)
