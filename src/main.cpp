// main.cpp - Entry point for Network Protocol Analyzer
#include <iostream>
#include "PacketCapture.h"
#include "NetworkVisualizer.h"
#include <vector>
#include <pcap.h>

std::vector<std::string> listDevices() {
    pcap_if_t* alldevs;
    std::vector<std::string> deviceNames;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return deviceNames;
    }
    
    int i = 0;
    for (pcap_if_t* d = alldevs; d; d = d->next) {
        std::cout << i << ": " << (d->description ? d->description : d->name) << std::endl;
        deviceNames.push_back(d->name);
        ++i;
    }
    
    pcap_freealldevs(alldevs);
    return deviceNames;
}

int main() {
    std::cout << "Network Protocol Analyzer started." << std::endl;
    std::cout << "Available network devices:" << std::endl;
    
    auto deviceNames = listDevices();
    if (deviceNames.empty()) {
        std::cerr << "No devices found!" << std::endl;
        return 1;
    }
    
    std::cout << "\nEnter device number to capture (or -1 to exit): ";
    int choice;
    std::cin >> choice;
    
    if (choice == -1 || choice < 0 || choice >= deviceNames.size()) {
        std::cout << "Exiting..." << std::endl;
        return 0;
    }
      std::cout << "Starting capture on: " << deviceNames[choice] << std::endl;
    
    // Create visualizer
    NetworkVisualizer visualizer;
    
    PacketCapture capture(deviceNames[choice]);
    
    // Set up visualization callback
    capture.setVisualizationCallback([&visualizer](const PacketStats& stats, int totalBytes) {
        visualizer.updateStats(stats, totalBytes);
        visualizer.displayRealTimeStats();
    });
    
    if (!capture.startCapture()) {
        std::cerr << "Failed to start packet capture!" << std::endl;
        return 1;
    }
    
    return 0;
}
