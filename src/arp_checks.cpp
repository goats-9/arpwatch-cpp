#include "arp_checks.hpp"
#include "send_mail.hpp"
#include "syslog_interface.hpp"
#include "env.hpp"
using namespace std;

// Check functions

bool new_station(pqxx::connection *conn,arp_record& record)
{
    vector<arp_record> old_recs = retrieve_record(conn,record.mac);
    if (old_recs.empty())
    {
        insert_record(conn,record.mac,record.ip,record.iface);
        arp_log(LOG_INFO,"New station found on ip " + record.ip + " with mac address " + record.mac);
        string body =
            "A new station detected with ip, mac, interface as follows:\n" +
            string("IP: ") + record.ip +
            "\nMAC: " + record.mac +
            "\ninterface: " + record.iface ;
        send_email(EMAIL,"ARPWATCH",body);
        return true;
    }
    return false;
}

bool new_activity(pqxx::connection *conn,arp_record& record)
{
    vector<arp_record> old_recs = retrieve_record(conn,record.mac);

    if (old_recs[0].ip == record.ip)
    {
        return false;
    }
    if (old_recs.size() == 1)
    {
        insert_record(conn,record.mac,record.ip,record.iface);
        return false;
    }

    if ( record.ip == old_recs[1].ip && record.tstamp - old_recs[0].tstamp <= NEWACTIVITY_DELTA)
    {
        // update recode here
        arp_log(LOG_INFO,"New activity found on ip " + record.ip + " with mac address " + record.mac);
        string body =
            "A new activity detected with ip, mac, interface as follows:\n" +
            string("IP: ") + record.ip +
            "\nMAC: " + record.mac +
            "\ninterface: " + record.iface ;
        send_email(EMAIL,"ARPWATCH",body);
        return true;
    }
    return false;
}

void update_and_check_records(arp_record& new_record) {
    pqxx::connection *conn = db_session_init();
    if (new_station(conn,new_record))
    {
        db_session_close(conn);
        return;
    }
    if (flip_flop(conn,new_record))
    {
        db_session_close(conn);
        return;
    }
    if (new_activity(conn,new_record))
    {
        db_session_close(conn);
        return;
    }
    if (changed_ethernet_address(conn,new_record))
    {
        db_session_close(conn);
        return;
    }
    db_session_close(conn);
    return;
}