#pragma once

#include <map>
#include <string>
#include <utility>

struct ArpRecord {
    std::string ip;
    std::string mac;
};

std::map<std::string, ArpRecord> records;

// Check functions
void new_station(const ArpRecord& record) {
    // MAC address not in records => new station
}

void flip_flop(const ArpRecord& record) {
    // MAC address associated with a different IP in a short time frame => flip flop
}

void new_activity(const ArpRecord& record) {
    // MAC address not active for long time is seen again => new activity
}

void changed_ethernet_address(const ArpRecord& record) {
    // IP address is now associated with different MAC => Ethernet address changed
}

void update_and_check_records(const ArpRecord& new_record) {
    new_station(new_record);
    flip_flop(new_record);
    new_activity(new_record);
    changed_ethernet_address(new_record);
    
    // Update the records with the new_record
    records[new_record.ip] = new_record;
}