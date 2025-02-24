# sdbus-cpp-examples

Examples using the sdbus-cpp library to communicate with the system bus.

This approach uses XML interface files to create proxy classes. All members, signals, and properties are available. No
introspection is used at runtime.

## Examples

The basics

* AVAHI - query for services and get state changes
* BlueZ - discover devices and dump all properties
* GeoClue2 - start a location session and print updates
* hostname1 - get the current hostname and get runtime updates
* locale1 - get the current locale and get runtime updates
* login1 - access the login1 interface and get runtime updates
* NetworkManager - access objects and properties
* resolve1 - resolve a hostname to an IP address
* timedate1 - get the current time and get runtime updates
* UPower - get the current battery status and get runtime updates
* UDisks2 - get all disk info and runtime updates

## Building

    https://github.com/jwinarske/sdbus-cpp-examples.git
    git submodule update --init --recursive
    mkdir build && cd build
    cmake .. -GNinja
    ninja

## Updating proxy files

Build project once for artifact `sdbus-c++-xml2cpp`.

    ./generate.sh

If you re-generate interfaces with different version of sdbus-cpp, likely you will need to make source code changes.
This is the primary reason for using sdbus-cpp as a submodule.