// demo.cpp - Demo version to showcase visualization features
#include <iostream>
#include "NetworkVisualizer.h"
#include "PacketCapture.h"
#include <thread>
#include <chrono>
#include <random>

void demonstrateVisualization() {
    NetworkVisualizer visualizer;
    PacketStats stats;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> packetDist(5, 25);
    std::uniform_int_distribution<> byteDist(64, 1500);
    std::uniform_int_distribution<> protocolDist(1, 4);
    
    std::cout << "Network Protocol Analyzer - Visualization Demo\n";
    std::cout << "Generating sample network data...\n";
    std::cout << "Press Ctrl+C to stop\n\n";
    
    int totalBytes = 0;
    
    for (int i = 0; i < 100; i++) {
        // Simulate packet capture
        int newPackets = packetDist(gen);
        int packetBytes = byteDist(gen);
        
        for (int j = 0; j < newPackets; j++) {
            stats.totalPackets++;
            totalBytes += packetBytes;
            
            int protocol = protocolDist(gen);
            switch (protocol) {
                case 1: stats.tcpPackets++; break;
                case 2: stats.udpPackets++; break;
                case 3: stats.icmpPackets++; break;
                default: stats.otherPackets++; break;
            }
        }
        
        stats.totalBytes = totalBytes;
        
        // Update visualization
        visualizer.updateStats(stats, totalBytes);
        visualizer.displayRealTimeStats();
        
        // Wait a bit to simulate real-time capture
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "\nDemo completed!\n";
}

int main() {
    demonstrateVisualization();
    return 0;
}
