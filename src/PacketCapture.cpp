// PacketCapture.cpp - Implementation of packet capture
#include "PacketCapture.h"
#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>

// Ethernet header
struct EthernetHeader {
    u_char dest[6];
    u_char source[6];
    u_short type;
};

// IP header
struct IpHeader {
    u_char version_ihl;
    u_char tos;
    u_short total_length;
    u_short identification;
    u_short flags_fragment;
    u_char ttl;
    u_char protocol;
    u_short checksum;
    u_int source_ip;
    u_int dest_ip;
};

PacketCapture::PacketCapture(const std::string& deviceName)
    : handle(nullptr), deviceName(deviceName), capturing(false) {}

PacketCapture::~PacketCapture() {
    stopCapture();
}

bool PacketCapture::startCapture() {
    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_live(deviceName.c_str(), 65536, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Failed to open device: " << errbuf << std::endl;
        return false;
    }
    
    std::cout << "Started packet capture on device: " << deviceName << std::endl;
    std::cout << "Press Ctrl+C to stop capture..." << std::endl;
      capturing = true;
    
    // Start packet capture loop with visualization
    pcap_loop(handle, 0, packetHandler, (u_char*)this);  // 0 = infinite loop
    
    // Print final statistics
    std::cout << "\n=== Final Capture Statistics ===" << std::endl;
    std::cout << "Total packets: " << stats.totalPackets << std::endl;
    std::cout << "TCP packets: " << stats.tcpPackets << std::endl;
    std::cout << "UDP packets: " << stats.udpPackets << std::endl;
    std::cout << "ICMP packets: " << stats.icmpPackets << std::endl;
    std::cout << "Other packets: " << stats.otherPackets << std::endl;
    
    return true;
}

void PacketCapture::stopCapture() {
    if (handle) {
        pcap_close(handle);
        handle = nullptr;
    }
    capturing = false;
}

void PacketCapture::packetHandler(u_char* userData, const struct pcap_pkthdr* header, const u_char* packet) {
    PacketCapture* capture = reinterpret_cast<PacketCapture*>(userData);
    capture->analyzePacket(header, packet);
}

void PacketCapture::analyzePacket(const struct pcap_pkthdr* header, const u_char* packet) {
    stats.totalPackets++;
    stats.totalBytes += header->len;
    
    // Parse Ethernet header
    EthernetHeader* ethHeader = (EthernetHeader*)packet;
    u_short etherType = ntohs(ethHeader->type);
    
    // Check if it's an IP packet
    if (etherType == 0x0800) { // IPv4
        IpHeader* ipHeader = (IpHeader*)(packet + sizeof(EthernetHeader));
        
        // Analyze protocol
        switch (ipHeader->protocol) {
            case 6: // TCP
                stats.tcpPackets++;
                break;
            case 17: // UDP
                stats.udpPackets++;
                break;
            case 1: // ICMP
                stats.icmpPackets++;
                break;
            default:
                stats.otherPackets++;
                break;
        }
    } else {
        stats.otherPackets++;
    }
    
    // Call visualization callback every 10 packets
    if (visualizationCallback && stats.totalPackets % 10 == 0) {
        visualizationCallback(stats, stats.totalBytes);
    }
}

void PacketCapture::setVisualizationCallback(std::function<void(const PacketStats&, int)> callback) {
    visualizationCallback = callback;
}
