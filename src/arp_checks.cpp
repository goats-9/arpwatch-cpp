#include "arp_checks.hpp"
#include "send_mail.hpp"
#include "syslog_interface.hpp"
#include "env.hpp"
using namespace std;

// Check functions

bool new_station(pqxx::connection *conn, arp_record &record)
{
    vector<arp_record> old_recs = retrieve_record_by_mac(conn, record.mac);

    if (old_recs.empty())
    {
        string body =
            "A new station detected with ip, mac, interface, time as follows:\n" +
            string("IP: ") + record.ip +
            "\nMAC: " + record.mac +
            "\ninterface: " + record.iface +
            "\ntime: " + ctime(&record.tstamp);
        // arp_log(LOG_INFO, "New station found with ip " + record.ip + " and mac address " + record.mac + " at " + ctime(&record.tstamp));
        arp_log(LOG_INFO,body);
        send_email("ARPWATCH", body);
        return true;
    }
    return false;
}

bool flip_flop(pqxx::connection *conn, arp_record &record)
{
    vector<arp_record> old_recs = retrieve_record_by_mac(conn, record.mac);

    if (old_recs.size() == 2 && record.ip == old_recs[1].ip)
    {
        string message =
            "Flip flop found between " + record.ip + ", " + old_recs[1].ip +
            " for mac address " + record.mac + " on interface " + record.iface +
            " at " + ctime(&record.tstamp);
        arp_log(LOG_INFO, message);
        send_email("ARPWATCH", message);
        return true;
    }
    return false;
}

bool new_activity(pqxx::connection *conn, arp_record &record)
{
    vector<arp_record> old_recs = retrieve_record_by_mac_ip(conn, record.mac, record.ip);

    if (!old_recs.empty() && record.tstamp - old_recs[0].tstamp <= NEWACTIVITY_DELTA)
    {
        // update recode here
        string body =
            "A new activity detected from ip, mac, interface, time as follows:\n" +
            string("IP: ") + record.ip +
            "\nMAC: " + record.mac +
            "\ninterface: " + record.iface +
            "\ntime: " + ctime(&record.tstamp);
        // arp_log(LOG_INFO, "New activity found from ip " + record.ip + " with mac address " + record.mac + " at " + ctime(&record.tstamp));
        arp_log(LOG_INFO,body);
        send_email("ARPWATCH", body);
        return true;
    }
    return false;
}

bool changed_ethernet_address(pqxx::connection *conn, arp_record &record)
{
    vector<arp_record> old_recs = retrieve_record_by_ip(conn, record.ip);

    if (!old_recs.empty() && old_recs[0].mac != record.mac)
    {
        string message = 
            "Ethernet address for ip " + record.ip + " changed from " + old_recs[0].mac + " to " + record.mac +
            " on interface " + record.iface + " at " + ctime(&record.tstamp);
        ;
        arp_log(LOG_INFO,message);
        send_email("ARPWATCH", message);
        return true;
    }
}

void update_and_check_records(arp_record &new_record)
{
    pqxx::connection *conn = db_session_init();
    if (new_station(conn, new_record))
    {
        insert_record(conn, new_record);
        db_session_close(conn);
        return;
    }
    if (flip_flop(conn, new_record))
    {
        insert_record(conn, new_record);
        db_session_close(conn);
        return;
    }
    if (new_activity(conn, new_record))
    {
        insert_record(conn, new_record);
        db_session_close(conn);
        return;
    }
    if (changed_ethernet_address(conn, new_record))
    {
        insert_record(conn, new_record);
        db_session_close(conn);
        return;
    }
    insert_record(conn, new_record);
    db_session_close(conn);
    return;
}