/**
 * File     : mysql_interface.hpp
 * Purpose  : Header file exposing APIs for MySQL db
*/

#pragma once

/* C++ STL header files */
#include <string>
#include <iostream>

/* C++ MySQL header files */
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace arpwatch_cpp {
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
     * @param iface Interface from where packet was monitored.
     * @return 0 on success, -1 on failure.
    */
    int update_record(sql::Connection *conn, std::string &mac, std::string &ip, std::string &iface);

    /**
     * @brief Retrieve MAC-IP record from `arp_records` by MAC and interface.
     * @param conn Pointer to `sql::Connection` object.
     * @param mac MAC address.
     * @param iface Interface from where packet was monitored.
     * @return `sql::ResultSet` object containing the required record
    */
    sql::ResultSet retrieve_record(sql::Connection *conn, std::string &mac, std::string &iface);

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
}