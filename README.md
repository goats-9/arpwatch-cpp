# arpwatch-cpp
A port of arpwatch to C++.

## Repository Structure

This repo is a CMake project, structured as follows.

1. A top level `CMakeLists.txt` file for the entire project.
2. The `src` directory containing the source code implementations of `arpwatch` and a `CMakeLists.txt` file that is called by the outer `CMakeLists.txt` file.
3. The `include` directory containing the header files used by the source code in `src`.

## Build Instructions

To build this project, run

```bash
mkdir build
cd build
cmake ..
cmake --build . --target install
./arpwatch  
```

## Files

1. `arpwatch.cpp`: Main program
2. `pcap_monitor.cpp`: Monitor for ARP packets on selected interfaces.
3. `mysql_interface.cpp`: Interface to DB to store ARP records. DB functions offered:
    - Insert record
    - Update record
    - Delete record
    - Retrieve record
4. `syslog_output.cpp`: Log output to `syslog`.
5. `send_mail.cpp`: Send mail to network admin.
6. `arp_checks.cpp`: Implement various checks such as (see `man 8 arpwatch`)
    - new station
    - flip flop
    - new activity
    - changed ethernet address

## Database Schema

The database has one table `arp_records`. Its schema is as follows (`mac` shall be the primary key).

| Field | Type |
|-|-|
| ip | string |
| interface | string |
| mac | string |
| time | timestamp |
