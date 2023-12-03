#pragma once

/* C++ STL header files */
#include <string>
#include <iostream>
#include <ctime>

/* C++ MySQL header files */
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

/* SQL prepared statements */
#define ARP_RETRIEVE_SQL_QUERY "SELECT * FROM arp_records WHERE mac = ?"
#define ARP_INSERT_SQL_QUERY "INSERT INTO arp_records VALUES (?, ?, ?, ?)"
#define ARP_UPDATE_SQL_QUERY "UPDATE arp_records SET ip = ?, iface = ? WHERE mac = ?"

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
 * @param conn Pointer to `sql::Connection` object.
 * @param mac MAC address.
 * @param ip IP address.
 * @param iface Interface from where packet was monitored.
 * @return 0 on success, -1 on failure.
*/
int insert_record(sql::Connection *conn, std::string &mac, std::string &ip, std::string &iface);

/**
 * @brief Update MAC-IP record in `arp_records` based on MAC and interface.
 * @param conn Pointer to `sql::Connection` object.
 * @param mac MAC address.
 * @param ip New IP address.
 * @param iface New interface from where packet was monitored.
 * @return 0 on success, -1 on failure.
*/
int update_record(sql::Connection *conn, std::string &mac, std::string &ip, std::string &iface);

/**
 * @brief Retrieve MAC-IP record from `arp_records` by MAC.
 * @param conn Pointer to `sql::Connection` object.
 * @param mac MAC address.
 * @return `arp_record` object containing the required record. In case of an
 * error, the `tstamp` field is set to -1.
*/
arp_record retrieve_record(sql::Connection *conn, std::string &mac);

/**
 * @brief Create and start a MySQL session.
 * @param uname Username of the user.
 * @param password Password of the user.
 * @param dbname Name of the database to connect to.
 * @return Pointer to `sql::Pointer to `sql::Connection`` object with the connection, or
 * `NULL` in case of a failure.
*/
sql::Connection *start_session(std::string &username, std::string &password, std::string dbname);

/**
 * @brief Terminate a MySQL session gracefully.
 * @param conn `sql::Pointer to `sql::Connection`` object whose session must be terminated.
 * @return 0 on successful termination, -1 otherwise.
*/
int terminate_session(sql::Connection *conn); 