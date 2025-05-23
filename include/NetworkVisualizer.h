// NetworkVisualizer.h - Visualization components for network analysis
#pragma once
#include "PacketCapture.h"
#include <string>
#include <chrono>
#include <vector>

struct TimeSeriesData {
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    int packetCount;
    int bytesPerSecond;
};

class NetworkVisualizer {
public:
    NetworkVisualizer();
    void updateStats(const PacketStats& stats, int totalBytes);
    void displayRealTimeStats();
    void displayProtocolDistribution(const PacketStats& stats);
    void displayBandwidthGraph();
    void displayNetworkMetrics(const PacketStats& stats);
    void clear();
    
private:
    void drawProgressBar(const std::string& label, int current, int total, int width = 30);
    void drawSimpleChart(const std::vector<int>& data, const std::string& title);
    
    std::vector<TimeSeriesData> bandwidthHistory;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    PacketStats lastStats;
    int totalBytesProcessed;
};
