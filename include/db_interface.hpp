#pragma once

/* C++ STL header files */
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

/* C++ psql header files */
#include <pqxx/pqxx>

/* Global variables */
pqxx::connection conn;

/* SQL prepared statements */
#define ARP_RETRIEVE_SQL_QUERY "SELECT * FROM arp_records WHERE mac = $1"
#define ARP_INSERT_SQL_QUERY "INSERT INTO arp_records VALUES ($1, $2, $3, $4)"

/**
 * @brief Struct representing records of arpwatch database.
*/
struct arp_record {
    /// @brief MAC address
    std::string mac;
    /// @brief Interface
    std::string iface;
    /// @brief IP address
    std::string ip;
    /// @brief Timestamp
    int tstamp;
};

/**
 * @brief Insert MAC-IP record into `arp_records`.
 * @param conn Pointer to `pqxx::connection` object for the current session.
 * @param mac MAC address.
 * @param ip IP address.
 * @param iface Interface from where packet was monitored.
 * @return 0 on success, -1 on failure.
*/
int insert_record(pqxx::connection *conn, std::string &mac, std::string &ip, std::string &iface);

/**
 * @brief Retrieve two most recent MAC-IP records from `arp_records` by MAC.
 * @param conn Pointer to `pqxx::connection` object for the current session.
 * @param mac MAC address.
 * @return `std::vector<arp_record>` object containing the required records. In case of an
 * error, the returned object will be empty.
*/
std::vector<arp_record> retrieve_record(pqxx::connection *conn, std::string &mac);

/**
 * @brief Create and start a psql session.
 * @return Pointer to `pqxx::connection object` if successful connection,
 * `nullptr` otherwise.
*/
pqxx::connection *db_session_init();

/**
 * @brief Terminate a psql session gracefully.
 * @param conn Pointer to `pqxx::connection` object whose session must be
 * terminated.
 * @return 0 on successful termination, -1 otherwise.
*/
int db_session_close(pqxx::connection *conn);