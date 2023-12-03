#pragma once

// #include <map>
#include <string>
#include "db_interface.hpp"

#define STATUS_NO_UPDATE -1
#define STATUS_NEW_STATION 0
#define STATUS_NEW_ACTIVITY 1
#define STATUS_FLIP_FLOP 2
#define STATUS_NEW_MAC 3

#define NEWACTIVITY_DELTA (6*30*24*60*60)	/* 6 months in seconds */
#define FLIPFLIP_DELTA (24*60*60)		/* 24 hours in seconds */

// std::map<std::string, ArpRecord> records;

// Check functions

/**
 * @brief Checks whether MAC address is there in records
 * @param record packet's destination ip,mac,timestamp and interface
 * @return Return true or false
*/
bool new_station(pqxx::connection *conn, arp_record& record);

/**
 * @brief Checks whether MAC address is associated with a different IP in a short time frame
 * @param record packet's destination ip,mac,timestamp and interface
 * @return Return true or false
*/
bool flip_flop(pqxx::connection *conn,arp_record& record);

/**
 * @brief Checks whether MAC address not active for long time is seen again
 * @param record packet's destination ip,mac,timestamp and interface
 * @return Return true or false
*/
bool new_activity(pqxx::connection *conn,arp_record& record);

/**
 * @brief Checks whether IP address is now associated with different MAC
 * @param record packet's destination ip,mac,timestamp and interface
 * @return Return true or false
*/
bool changed_ethernet_address(pqxx::connection *conn,arp_record& record);

/**
 * @brief performs all checks
 * @param new_record packet's destination ip,mac and interface
*/
void update_and_check_records(arp_record& record);