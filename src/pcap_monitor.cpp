#include <iostream>
#include <pcap.h>
#include <arpa/inet.h>
#include <vector>
#include <string>

/* ARP Header, (assuming Ethernet+IPv4)            */
#define ARP_REQUEST 1 /* ARP Request             */
#define ARP_REPLY 2   /* ARP Reply               */

typedef struct arphdr
{
    u_int16_t htype; /* Hardware Type           */
    u_int16_t ptype; /* Protocol Type           */
    u_char hlen;     /* Hardware Address Length */
    u_char plen;     /* Protocol Address Length */
    u_int16_t oper;  /* Operation Code          */
    u_char sha[6];   /* Sender hardware address */
    u_char spa[4];   /* Sender IP address       */
    u_char tha[6];   /* Target hardware address */
    u_char tpa[4];   /* Target IP address       */
} arphdr_t;

struct ArpAddresses
{
    std::string senderIp;
    std::string senderMac;
    std::string targetIp;
    std::string targetMac;
};

void processArpPacket(const u_char *packet, std::vector<ArpAddresses> &arpAddresses)
{
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
    for (int i = 0; i < hardwareAddrLength; ++i)
    {
        char buffer[3];
        snprintf(buffer, sizeof(buffer), "%02X", senderHardwareAddr[i]);
        addresses.senderMac += buffer;

        snprintf(buffer, sizeof(buffer), "%02X", targetHardwareAddr[i]);
        addresses.targetMac += buffer;
    }

    // Convert IP addresses to string
    for (int i = 0; i < protocolAddrLength; ++i)
    {
        addresses.senderIp += std::to_string(senderProtocolAddr[i]);
        addresses.targetIp += std::to_string(targetProtocolAddr[i]);
        if (i < protocolAddrLength - 1)
        {
            addresses.senderIp += ".";
            addresses.targetIp += ".";
        }
    }

    arpAddresses.push_back(addresses);
}

void packetHandler(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet)
{
    // Callback function for pcap_loop
    std::vector<ArpAddresses> &arpAddresses = *reinterpret_cast<std::vector<ArpAddresses> *>(user);

    arphdr_t *arpheader = (struct arphdr *)(packet + 14); /* Point to the ARP header */

    printf("\n\nReceived Packet Size: %d bytes\n", pkthdr->len);
    printf("Hardware type: %s\n", (ntohs(arpheader->htype) == 1) ? "Ethernet" : "Unknown");
    printf("Protocol type: %s\n", (ntohs(arpheader->ptype) == 0x0800) ? "IPv4" : "Unknown");
    printf("Operation: %s\n", (ntohs(arpheader->oper) == ARP_REQUEST) ? "ARP Request" : "ARP Reply");
    /* If is Ethernet and IPv4, print packet contents */
    if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800)
    {
        printf("Sender MAC: ");

        for (int i = 0; i < 6; i++)
            printf("%02X:", arpheader->sha[i]);

        printf("\nSender IP: ");

        for (int i = 0; i < 4; i++)
            printf("%d.", arpheader->spa[i]);

        printf("\nTarget MAC: ");

        for (int i = 0; i < 6; i++)
            printf("%02X:", arpheader->tha[i]);

        printf("\nTarget IP: ");

        for (int i = 0; i < 4; i++)
            printf("%d.", arpheader->tpa[i]);

        printf("\n");
    }
}

std::vector<ArpAddresses> listenPCAP(const char *dev_name)
{
    std::vector<ArpAddresses> arpAddresses;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open a pcap session on the network interface
    handle = pcap_open_live(dev_name, BUFSIZ, 1, 1000, errbuf);

    if (handle == nullptr)
    {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        return arpAddresses;
    }
    bpf_u_int32 netaddr = 0, mask = 0; /* To Store network address and netmask   */
    struct bpf_program filter;         /* Place to store the BPF filter program  */

    /* Look up info from the capture device. */
    if (pcap_lookupnet(dev_name, &netaddr, &mask, errbuf) == -1)
    {
        fprintf(stderr, "ERROR: %s\n", errbuf);
        exit(1);
    }

    /* Compiles the filter expression into a BPF filter program */
    if (pcap_compile(handle, &filter, "arp", 1, mask) == -1)
    {
        fprintf(stderr, "ERROR: %s\n", pcap_geterr(handle));
        exit(1);
    }

    /* Load the filter program into the packet capture device. */
    if (pcap_setfilter(handle, &filter) == -1)
    {
        fprintf(stderr, "ERROR: %s\n", pcap_geterr(handle));
        exit(1);
    }

    // The 100 here shows that it will terminate after listening to 100 packets. Can be changed.
    pcap_loop(handle, 10, packetHandler, reinterpret_cast<unsigned char *>(&arpAddresses));

    // Close the handle
    pcap_close(handle);

    return arpAddresses;
}

std::vector<ArpAddresses> extractARP()
{
    std::vector<ArpAddresses> arpAddresses;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *dev;

    // this gives the list of all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return arpAddresses;
    }

    // iterate through all available interfaces
    for (dev = alldevs; dev != NULL; dev = dev->next)
    {
        std::cout << "listening " << dev->name << "\n";
        std::vector<ArpAddresses> addressesOnInterface = listenPCAP(dev->name);
        arpAddresses.insert(arpAddresses.end(), addressesOnInterface.begin(), addressesOnInterface.end());
    }

    pcap_freealldevs(alldevs);

    return arpAddresses;
}
