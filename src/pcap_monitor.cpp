#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

struct ArpAddresses {
    std::string senderIp;
    std::string senderMac;
    std::string targetIp;
    std::string targetMac;
};

void processArpPacket(const u_char *packet, std::vector<ArpAddresses> &arpAddresses) {
    // arp header is 28 bytes.
    const uint8_t *arpHeader = packet;

    uint8_t hardwareAddrLength = *(arpHeader + 4);
    uint8_t protocolAddrLength = *(arpHeader + 5);

    const uint8_t *senderHardwareAddr = arpHeader + 8;
    const uint8_t *senderProtocolAddr = senderHardwareAddr + hardwareAddrLength;
    const uint8_t *targetHardwareAddr = senderProtocolAddr + protocolAddrLength;
    const uint8_t *targetProtocolAddr = targetHardwareAddr + hardwareAddrLength;

    ArpAddresses addresses;

    // Convert MAC addresses to string
    for (int i = 0; i < hardwareAddrLength; ++i) {
        char buffer[3];
        snprintf(buffer, sizeof(buffer), "%02X", senderHardwareAddr[i]);
        addresses.senderMac += buffer;

        snprintf(buffer, sizeof(buffer), "%02X", targetHardwareAddr[i]);
        addresses.targetMac += buffer;
    }

    // Convert IP addresses to string
    for (int i = 0; i < protocolAddrLength; ++i) {
        addresses.senderIp += std::to_string(senderProtocolAddr[i]);
        addresses.targetIp += std::to_string(targetProtocolAddr[i]);
        if (i < protocolAddrLength - 1) {
            addresses.senderIp += ".";
            addresses.targetIp += ".";
        }
    }

    arpAddresses.push_back(addresses);
}

void packetHandler(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet, std::vector<ArpAddresses> &arpAddresses) {
    // Callback function for pcap_loop
    if (*(uint16_t *)(packet + 12) == ntohs(0x0806)) {
        processArpPacket(packet, arpAddresses);
    }
}

std::vector<ArpAddresses> listenPCAP() {
    std::vector<ArpAddresses> arpAddresses;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open a pcap session on the network interface
    handle = pcap_open_live(nullptr, BUFSIZ, 1, 1000, errbuf);

    if (handle == nullptr) {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        return arpAddresses;
    }

    // The 100 here shows that it will terminate after listening to 100 packets. Can be changed.
    pcap_loop(handle, 100, packetHandler, reinterpret_cast<unsigned char *>(&arpAddresses));
    // Close the handle
    pcap_close(handle);

    return arpAddresses;
}

std::vector<ArpAddresses> extractARP() {
    std::vector<ArpAddresses> arpAddresses;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *dev;

    // this gives the list of all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return arpAddresses;
    }

    // iterate through all available interfaces
    for (dev = alldevs; dev != NULL; dev = dev->next) {
        std::vector<ArpAddresses> addressesOnInterface = listenPCAP();
        arpAddresses.insert(arpAddresses.end(), addressesOnInterface.begin(), addressesOnInterface.end());
    }

    pcap_freealldevs(alldevs);

    return arpAddresses;
}

int main() {
    std::vector<ArpAddresses> addresses = extractARP();

    // Display the results
    for (const auto &addr : addresses) {
        std::cout << "Sender IP: " << addr.senderIp << ", Sender MAC: " << addr.senderMac << std::endl;
        std::cout << "Target IP: " << addr.targetIp << ", Target MAC: " << addr.targetMac << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
