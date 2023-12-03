#include "arp_checks.hpp"
#include "send_mail.hpp"
using namespace std;

// Check functions

bool new_station(const sql::Connection *conn,const arp_record& record)
{
    arp_record old_rec = retrieve_record(conn,record.mac);
    if (old_rec.tstamp == -1)
    {
        insert_record(conn,record.mac,record.ip,record.iface);
        return true;
    }
    return false;
}

bool new_activity(const sql::Connection *conn,const arp_record& record)
{
    arp_record old_rec = retrieve_record(conn,record.mac);

    if ( record.tstamp - old_rec.tstamp <= NEWACTIVITY_DELTA)
    {
        return true;
    }
    return false;
}

void update_and_check_records(const arp_record& new_record) {
    sql::Connection *conn = start_session(DB_USERNAME,DB_PASSWORD,DB_NAME);
    if (new_station(conn,new_record))
    {
        string body =
            "A new station detected. The information is as follows:\n" +
            string("IP: ") + new_record.ip +
            "\nMAC: " + new_record.mac +
            "\ninterface: " + new_record.iface ;
        send_email(EMAIL,"ARPWATCH",body);
        terminate_session(conn);
    }
    if (flip_flop(conn,new_record))
    {
        terminate_session(conn);
    }
    if (new_activity(conn,new_record))
    {
        terminate_session(conn);
    }
    if (changed_ethernet_address(conn,new_record))
    {
        terminate_session(conn);
    }
    terminate_session(conn);
    return;
}