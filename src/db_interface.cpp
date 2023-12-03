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

int terminate_session(pqxx::connection *conn) {
    try {
        conn->disconnect();
        return 0;
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return -1;
    }
}

std::vector<arp_record> retrieve_record(pqxx::connection *conn, std::string &mac) {
    std::vector<arp_record> db_records;
    try {
        conn->prepare("retrieve_query", ARP_RETRIEVE_SQL_QUERY);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared("retrieve_query", mac);
        // Parse pqxx::result into db_record
        for (auto const &row : res) {
            arp_record ar;
            ar.mac = row["mac"].as<std::string>();
            ar.ip = row["ip"].as<std::string>();
            ar.iface = row["iface"].as<std::string>();
            ar.tstamp = row["tstamp"].as<int>();
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

int insert_record(pqxx::connection *conn, std::string &mac, std::string &iface, std::string &ip) {
    try {
        conn->prepare("insert_query", ARP_RETRIEVE_SQL_QUERY);
        std::time_t curr_tstamp = std::time(nullptr);
        pqxx::work w{*conn};
        pqxx::result res = w.exec_prepared0("insert_query", mac, ip, iface, curr_tstamp);
        w.commit();
        return 0;
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return -1;
    }
}