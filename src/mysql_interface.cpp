/**
 * File     : mysql_interface.cpp
 * Purpose  : Implementations of APIs for MySQL db
*/

#include "mysql_interface.hpp"

sql::Connection *start_session(std::string &uname, std::string &password, std::string dbname) {
    try {
        sql::Driver *driver;
        sql::Connection *conn;

        // set connection options
        sql::ConnectOptionsMap connopt;
        connopt["userName"] = uname;
        connopt["password"] = password;
        connopt["schema"] = dbname;
        connopt["OPT_RECONNECT"] = true;
        /* Create the connection */
        driver = get_driver_instance();
        conn = driver->connect(connopt);
        return conn;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}

int terminate_session(sql::Connection *conn) {
    try {
        conn->close();
        return 0;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return -1;
    }
}

arp_record retrieve_record(sql::Connection *conn, std::string &mac) {
    arp_record db_record;
    try {
        sql::PreparedStatement *pstmt;
        sql::ResultSet *rset;
        // Create and execute PreparedStatement
        pstmt = conn->prepareStatement(ARP_RETRIEVE_SQL_QUERY);
        pstmt->setString(1, mac);
        rset = pstmt->executeQuery();
        // Parse rset into db_record
        db_record.mac = rset->getString("mac");
        db_record.ip = rset->getString("ip");
        db_record.iface = rset->getString("iface");
        db_record.tstamp = rset->getInt("tstamp");
        // Cleanup
        delete rset;
        delete pstmt;
        return db_record;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        db_record.tstamp = -1;
        return db_record;
    }
}

int update_record(sql::Connection *conn, std::string &mac, std::string &ip, std::string &iface) {
    try {
        sql::PreparedStatement *pstmt;
        pstmt = conn->prepareStatement(ARP_UPDATE_SQL_QUERY);
        pstmt->setString(1, mac);
        pstmt->setString(2, ip);
        pstmt->setString(3, iface);
        std::time_t curr_tstamp = std::time(nullptr);
        pstmt->setInt(4, curr_tstamp);
        int res = pstmt->executeUpdate();
        delete pstmt;
        return 0;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return -1;
    }
}

int insert_record(sql::Connection *conn, std::string &mac, std::string &iface, std::string &ip) {
    try {
        sql::PreparedStatement *pstmt;
        pstmt = conn->prepareStatement(ARP_INSERT_SQL_QUERY);
        std::time_t curr_tstamp = std::time(nullptr);
        pstmt->setString(1, mac);
        pstmt->setString(2, ip);
        pstmt->setString(3, iface);
        pstmt->setInt(4, curr_tstamp);
        int res = pstmt->execute();
        delete pstmt;
        return 0;
    } catch (sql::SQLException &e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return -1;
    }
}
