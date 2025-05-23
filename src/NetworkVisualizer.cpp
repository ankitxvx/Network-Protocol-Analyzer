// NetworkVisualizer.cpp - Implementation of visualization components
#include "NetworkVisualizer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

NetworkVisualizer::NetworkVisualizer() 
    : startTime(std::chrono::steady_clock::now()), totalBytesProcessed(0) {}

void NetworkVisualizer::updateStats(const PacketStats& stats, int totalBytes) {    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    
    int bytesPerSecond = (duration > 0) ? (int)(totalBytes / duration) : 0;
    
    TimeSeriesData dataPoint;
    dataPoint.timestamp = now;
    dataPoint.packetCount = stats.totalPackets;
    dataPoint.bytesPerSecond = bytesPerSecond;
    
    bandwidthHistory.push_back(dataPoint);
    
    // Keep only last 60 data points (1 minute of data)
    if (bandwidthHistory.size() > 60) {
        bandwidthHistory.erase(bandwidthHistory.begin());
    }
    
    lastStats = stats;
    totalBytesProcessed = totalBytes;
}

void NetworkVisualizer::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void NetworkVisualizer::displayRealTimeStats() {
    clear();
    
    std::cout << "╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   NETWORK PROTOCOL ANALYZER                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    
    std::cout << "║ Runtime: " << std::setfill(' ') << std::setw(8) << duration 
              << " seconds  |  Total Bytes: " << std::setw(12) << totalBytesProcessed << " ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    
    displayNetworkMetrics(lastStats);
    displayProtocolDistribution(lastStats);
    displayBandwidthGraph();
    
    std::cout << "╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Press Ctrl+C to stop capture...\n";
}

void NetworkVisualizer::displayNetworkMetrics(const PacketStats& stats) {
    std::cout << "║ NETWORK METRICS                                                  ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    
    double packetsPerSecond = (duration > 0) ? (double)stats.totalPackets / duration : 0;
    double avgPacketSize = (stats.totalPackets > 0) ? (double)totalBytesProcessed / stats.totalPackets : 0;
    
    std::cout << "║ Total Packets: " << std::setfill(' ') << std::setw(8) << stats.totalPackets 
              << "  |  Packets/sec: " << std::setw(8) << std::fixed << std::setprecision(1) << packetsPerSecond << " ║\n";
    std::cout << "║ Avg Packet Size: " << std::setw(6) << std::fixed << std::setprecision(0) << avgPacketSize 
              << " bytes  |  Bandwidth: " << std::setw(6) << (totalBytesProcessed * 8 / (duration > 0 ? duration : 1)) << " bps     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
}

void NetworkVisualizer::displayProtocolDistribution(const PacketStats& stats) {
    std::cout << "║ PROTOCOL DISTRIBUTION                                            ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    
    if (stats.totalPackets > 0) {
        drawProgressBar("TCP   ", stats.tcpPackets, stats.totalPackets, 40);
        drawProgressBar("UDP   ", stats.udpPackets, stats.totalPackets, 40);
        drawProgressBar("ICMP  ", stats.icmpPackets, stats.totalPackets, 40);
        drawProgressBar("Other ", stats.otherPackets, stats.totalPackets, 40);
    } else {
        std::cout << "║ No packets captured yet...                                       ║\n";
    }
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
}

void NetworkVisualizer::displayBandwidthGraph() {
    std::cout << "║ BANDWIDTH HISTORY (last 20 measurements)                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    
    if (bandwidthHistory.size() < 2) {
        std::cout << "║ Collecting data...                                               ║\n";
    } else {        // Extract last 20 bandwidth measurements
        std::vector<int> recentBandwidth;
        size_t start = (bandwidthHistory.size() > 20) ? bandwidthHistory.size() - 20 : 0;
        for (size_t i = start; i < bandwidthHistory.size(); i++) {
            recentBandwidth.push_back(bandwidthHistory[i].bytesPerSecond);
        }
        
        // Find max value for scaling
        int maxBandwidth = *std::max_element(recentBandwidth.begin(), recentBandwidth.end());
        if (maxBandwidth == 0) maxBandwidth = 1;
        
        // Draw simple ASCII chart
        std::cout << "║ ";
        for (int value : recentBandwidth) {
            int barHeight = (value * 10) / maxBandwidth;
            if (barHeight == 0 && value > 0) barHeight = 1;
            
            if (barHeight >= 8) std::cout << "█";
            else if (barHeight >= 6) std::cout << "▆";
            else if (barHeight >= 4) std::cout << "▄";
            else if (barHeight >= 2) std::cout << "▂";
            else if (barHeight >= 1) std::cout << "▁";
            else std::cout << " ";
        }
          // Fill remaining space
        for (size_t i = recentBandwidth.size(); i < 60; i++) {
            std::cout << " ";
        }
        std::cout << " ║\n";
        
        std::cout << "║ Max: " << std::setfill(' ') << std::setw(8) << maxBandwidth 
                  << " bytes/sec                                    ║\n";
    }
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
}

void NetworkVisualizer::drawProgressBar(const std::string& label, int current, int total, int width) {
    double percentage = (total > 0) ? (double)current / total * 100.0 : 0.0;
    int filled = (int)(percentage * width / 100.0);
    
    std::cout << "║ " << label << " [";
    
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    
    std::cout << "] " << std::setfill(' ') << std::setw(6) << std::fixed << std::setprecision(1) 
              << percentage << "% (" << current << ") ║\n";
}
