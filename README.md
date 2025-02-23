# sdbus-cpp-examples

Examples using the sdbus-cpp library to communicate with the system bus.

## Examples

The basics

* AVAHI - query for services and get state changes
* BlueZ - discover devices and dump all properties
* GeoClue2 - start a location session and print updates
* hostname1 - get the current hostname and get runtime updates
* locale1 - get the current locale and get runtime updates
* login1 - access the login1 interface and get runtime updates
* resolve1 - resolve a hostname to an IP address
* timedate1 - get the current time and get runtime updates
* UPower - get the current battery status and get runtime updates

## Building

    https://github.com/jwinarske/sdbus-cpp-examples.git
    git submodule update --init --recursive
    mkdir build && cd build
    cmake .. -GNinja
    ninja
