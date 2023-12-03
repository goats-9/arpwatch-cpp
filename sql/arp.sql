-- Create the table
CREATE TABLE arp_db (
    mac_address VARCHAR(17) NOT NULL,
    ip_address VARCHAR(64),
    interface VARCHAR(50),
    tstamp INT NOT NULL,
    PRIMARY KEY (tstamp, mac_address)
);

-- Create an index for performance
CREATE INDEX idx_mac_tstamp ON arp_db (mac_address, tstamp DESC);

-- Example Trigger (simplified for demonstration)
CREATE OR REPLACE FUNCTION manage_mac_records()
RETURNS TRIGGER AS $$
BEGIN
    IF (SELECT COUNT(*) FROM arp_db WHERE mac_address = NEW.mac_address) > 2 THEN
        DELETE FROM arp_db 
        WHERE tstamp = (SELECT MIN(tstamp) FROM arp_db WHERE mac_address = NEW.mac_address);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_manage_mac_records
AFTER INSERT ON arp_db
FOR EACH ROW EXECUTE FUNCTION manage_mac_records();