-- Create the table
CREATE TABLE IF NOT EXISTS arp_records (
    mac VARCHAR(17) NOT NULL,
    ip VARCHAR(64),
    iface VARCHAR(50),
    tstamp INT NOT NULL,
    PRIMARY KEY (ip, mac)
);
