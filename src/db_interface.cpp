#include "db_interface.hpp"

pqxx::connection *db_session_init() {
    try {
        // Connect to db
        auto *conn = new pqxx::connection("keepalives=1");
        return conn;
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return NULL;
    }
}

int db_session_close(pqxx::connection *conn) {
    try {
        conn->disconnect();
        return 0;
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return -1;
    }
}

std::vector<arp_record> retrieve_record_by_mac(pqxx::connection *conn, std::string &mac) {
    std::vector<arp_record> db_records;
    try {
        conn->prepare("retrieve_query_mac", ARP_RETRIEVE_MAC_SQL_QUERY);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared("retrieve_query_mac", mac);
        // Parse pqxx::result into db_record
        for (auto const &row : res) {
            arp_record ar;
            ar.mac = row["mac"].as<std::string>();
            ar.ip = row["ip"].as<std::string>();
            ar.iface = row["iface"].as<std::string>();
            ar.tstamp = row["tstamp"].as<std::time_t>();
            db_records.push_back(ar);
        }
        // Cleanup
        w.commit();
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        db_records.clear();
    }
    return db_records;
}

std::vector<arp_record> retrieve_record_by_ip(pqxx::connection *conn, std::string &ip) {
    std::vector<arp_record> db_records;
    try {
        conn->prepare("retrieve_query_ip", ARP_RETRIEVE_IP_SQL_QUERY);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared("retrieve_query_ip", ip);
        // Parse pqxx::result into db_record
        for (auto const &row : res) {
            arp_record ar;
            ar.mac = row["mac"].as<std::string>();
            ar.ip = row["ip"].as<std::string>();
            ar.iface = row["iface"].as<std::string>();
            ar.tstamp = row["tstamp"].as<std::time_t>();
            db_records.push_back(ar);
        }
        // Cleanup
        w.commit();
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        db_records.clear();
    }
    return db_records;
}

std::vector<arp_record> retrieve_record_by_mac_ip(pqxx::connection *conn, std::string &mac, std::string &ip) {
    std::vector<arp_record> db_records;
    try {
        conn->prepare("retrieve_query_mac_ip", ARP_RETRIEVE_MAC_IP_SQL_QUERY);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared("retrieve_query_mac_ip", mac, ip);
        // Parse pqxx::result into db_record
        for (auto const &row : res) {
            arp_record ar;
            ar.mac = row["mac"].as<std::string>();
            ar.ip = row["ip"].as<std::string>();
            ar.iface = row["iface"].as<std::string>();
            ar.tstamp = row["tstamp"].as<std::time_t>();
            db_records.push_back(ar);
        }
        // Cleanup
        w.commit();
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        db_records.clear();
    }
    return db_records;
}

int insert_record(pqxx::connection *conn, arp_record &ar) {
    try {
        conn->prepare("insert_query", ARP_INSERT_SQL_QUERY);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared0("insert_query", ar.mac, ar.ip, ar.iface, ar.tstamp);
        w.commit();
        return 0;
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return -1;
    }
}
