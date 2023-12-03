#pragma once

#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

/**
 * @brief struct for ARP packet
*/
struct ArpAddresses {
    /// @brief IP address of sending host
    std::string senderIp;
    /// @brief MAC address of sending host
    std::string senderMac;
    /// @brief IP address of target host
    std::string targetIp;
    /// @brief MAC address of target host
    std::string targetMac;
};

void processArpPacket(const u_char *packet, std::vector<ArpAddresses> &arpAddresses);
void packetHandler(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet, std::vector<ArpAddresses> &arpAddresses);
std::vector<ArpAddresses> listenPCAP();
std::vector<ArpAddresses> extractARP();