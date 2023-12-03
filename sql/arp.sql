-- Create the table
CREATE TABLE arp_db (
    mac_address VARCHAR(17) NOT NULL,
    ip_address VARCHAR(64),
    interface VARCHAR(50),
    tstamp INT NOT NULL,
    PRIMARY KEY (ip_address, mac_address)
);
