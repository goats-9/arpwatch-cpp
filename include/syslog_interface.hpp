#pragma once
#include <syslog.h>
#include <string>

/**
 * @brief Wrapper around `syslog` to log ARP packets.
 * @param log_level Type of message, whether notice, information or error.
 * @param log_msg Message to be logged.  
*/
void arp_log(int log_level, const std::string &log_msg);