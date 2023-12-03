#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>

void processArpPacket(const u_char *packet) {
    // arp header is 28 bytes.
    const uint8_t *arpHeader = packet;

    uint16_t hardwareType = ntohs(*(uint16_t *)(arpHeader));
    uint16_t protocolType = ntohs(*(uint16_t *)(arpHeader + 2));
    uint8_t hardwareAddrLength = *(arpHeader + 4);
    uint8_t protocolAddrLength = *(arpHeader + 5);
    uint16_t opCode = ntohs(*(uint16_t *)(arpHeader + 6));

    std::cout << "Hardware Type: " << hardwareType << std::endl;
    std::cout << "Protocol Type: " << protocolType << std::endl;
    std::cout << "Hardware Address Length: " << (int)hardwareAddrLength << std::endl;
    std::cout << "Protocol Address Length: " << (int)protocolAddrLength << std::endl;
    std::cout << "Operation Code: " << opCode << std::endl;

    /*
    Hardware Addresses <-> MAC Addresses
    Protocol Addresses <-> IP Addresses
    */

    const uint8_t *senderHardwareAddr = arpHeader + 8;
    const uint8_t *senderProtocolAddr = senderHardwareAddr + hardwareAddrLength;
    const uint8_t *targetHardwareAddr = senderProtocolAddr + protocolAddrLength;
    const uint8_t *targetProtocolAddr = targetHardwareAddr + hardwareAddrLength;

    // display
    std::cout << "Sender Hardware Address: "; // Sender MAC Address
    for (int i = 0; i < hardwareAddrLength; ++i) {
        printf("%02X ", senderHardwareAddr[i]);
    }
    std::cout << std::endl;

    std::cout << "Sender Protocol Address: "; // Sender IP Address
    for (int i = 0; i < protocolAddrLength; ++i) {
        printf("%d ", senderProtocolAddr[i]);
    }
    std::cout << std::endl;

    std::cout << "Target Hardware Address: "; // Target MAC address
    for (int i = 0; i < hardwareAddrLength; ++i) {
        printf("%02X ", targetHardwareAddr[i]);
    }
    std::cout << std::endl;

    std::cout << "Target Protocol Address: "; // Target IP Address
    for (int i = 0; i < protocolAddrLength; ++i) {
        printf("%d ", targetProtocolAddr[i]);
    }
    std::cout << std::endl;
}

void packetHandler(unsigned char* user, const struct pcap_pkthdr* pkthdr, const unsigned char* packet) {
    // Callback function for pcap_loop
    if (*(uint16_t *)(packet + 12) == ntohs(0x0806)) {
        processArpPacket(packet);
    }
}

void listenPCAP(const char* interface) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open a pcap session on the network interface
    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);

    if (handle == nullptr) {
        fprintf(stderr, "Could not open device %s: %s\n", interface, errbuf);
        return;
    }

    // Assuming ARP protocol is 0x0806
    if (pcap_datalink(handle) == DLT_EN10MB || pcap_datalink(handle) == DLT_IEEE802_11) {
        pcap_loop(handle, 0, packetHandler, NULL);
    } else {
        std::cerr << "Unsupported link-layer type" << std::endl;
    }

    // Close the handle
    pcap_close(handle);
}

void extractARP() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *dev;

    // this gives the list of all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return 1;
    }

    // printing the devices
    int count = 0;
    for (dev = alldevs; dev != NULL; dev = dev->next) {
        if (count == 2) {
            break;
        }
        count++;
        printf("%s\n", dev->name);
    }

    // just for funzies, take the second device
    if (alldevs != NULL && alldevs->next != NULL) {
        listenPCAP(alldevs->next->name);
    }

    pcap_freealldevs(alldevs);

    return 0;
}

int main(){
    extractARP();
    return 0;
}
