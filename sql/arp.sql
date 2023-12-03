-- Create the table
CREATE TABLE arp_records (
    mac VARCHAR(17) NOT NULL,
    ip VARCHAR(64),
    iface VARCHAR(50),
    tstamp INT NOT NULL,
    PRIMARY KEY (ip_address, mac_address)
);
