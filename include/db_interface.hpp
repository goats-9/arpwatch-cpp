#pragma once

/* C++ STL header files */
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

/* C++ psql header files */
#include <pqxx/pqxx>

/* SQL prepared statements */
#define ARP_RETRIEVE_MAC_SQL_QUERY "SELECT * FROM arp_records WHERE mac = $1 ORDER BY tstamp DESC LIMIT 2"
#define ARP_RETRIEVE_IP_SQL_QUERY "SELECT * FROM arp_records WHERE ip = $1 ORDER BY tstamp DESC LIMIT 1"
#define ARP_RETRIEVE_MAC_IP_SQL_QUERY "SELECT * FROM arp_records WHERE mac = $1 AND ip = $2"
#define ARP_INSERT_SQL_QUERY "INSERT INTO arp_records VALUES ($1, $2, $3, $4) ON CONFLICT(mac, ip) DO UPDATE set iface = EXCLUDED.iface, tstamp = EXCLUDED.tstamp"

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
 * TODO:
 * 1. Insert and update
 * 2. Retrieve
 *    - by MAC/bool (new station)
 *    - by MAC and IP (new activity)
 *    - top 2 records for MAC (by tstamp, flip flop)
 *    - by IP (latest tstamp, changed ethernet address)
*/

/**
 * @brief Insert MAC-IP record into `arp_records`.
 * @param conn Pointer to `pqxx::connection` object for the current session.
 * @param ar `arp_record` struct for the record to be inserted.
 * @return 0 on success, -1 on failure.
*/
int insert_record(pqxx::connection *conn, arp_record &ar);

/**
 * @brief Retrieve two most recent MAC-IP records from `arp_records` by MAC.
 * @param conn Pointer to `pqxx::connection` object for the current session.
 * @param mac MAC address.
 * @return `std::vector<arp_record>` object containing the required records. In case of an
 * error, the returned object will be empty.
*/
std::vector<arp_record> retrieve_record_by_mac(pqxx::connection *conn, std::string &mac);
std::vector<arp_record> retrieve_record_by_ip(pqxx::connection *conn, std::string &ip);
std::vector<arp_record> retrieve_record_by_mac_ip(pqxx::connection *conn, std::string &mac, std::string &ip);

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