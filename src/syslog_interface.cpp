#include "syslog_interface.hpp"

void arp_log(int log_level, std::string &log_msg) {
    syslog(log_level, log_msg.c_str());
}