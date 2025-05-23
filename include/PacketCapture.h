// PacketCapture.h - Interface for packet capture
#pragma once
#include <pcap.h>
#include <string>
#include <functional>

struct PacketStats {
    int totalPackets = 0;
    int tcpPackets = 0;
    int udpPackets = 0;
    int icmpPackets = 0;
    int otherPackets = 0;
    int totalBytes = 0;
};

class PacketCapture {
public:
    PacketCapture(const std::string& deviceName);
    ~PacketCapture();
    bool startCapture();
    void stopCapture();
    PacketStats getStats() const { return stats; }
    void setVisualizationCallback(std::function<void(const PacketStats&, int)> callback);
    
private:
    static void packetHandler(u_char* userData, const struct pcap_pkthdr* header, const u_char* packet);
    void analyzePacket(const struct pcap_pkthdr* header, const u_char* packet);
    
    pcap_t* handle;
    std::string deviceName;
    PacketStats stats;
    bool capturing;
    std::function<void(const PacketStats&, int)> visualizationCallback;
};
