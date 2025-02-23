#!/bin/bash

PROXY=src/proxy
XML2CPP=./cmake-build-debug/third_party/sdbus-cpp/tools/sdbus-c++-xml2cpp

mkdir -p ${PROXY} |true

${XML2CPP} --verbose --proxy=${PROXY}/Adapter1_proxy.h interfaces/org.bluez.Adapter1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Device1_proxy.h interfaces/org.bluez.Device1.xml
${XML2CPP} --verbose --proxy=${PROXY}/AgentManager1_proxy.h interfaces/org.bluez.AgentManager1.xml

${XML2CPP} --verbose --proxy=${PROXY}/UPower_proxy.h interfaces/org.freedesktop.UPower.xml
${XML2CPP} --verbose --proxy=${PROXY}/UPowerDisplayDevice_proxy.h interfaces/org.freedesktop.UPower.DisplayDevice.xml

${XML2CPP} --verbose --proxy=${PROXY}/Login1Manager_proxy.h interfaces/org.freedesktop.login1.Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1Seat_proxy.h interfaces/org.freedesktop.login1.Seat.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1Session_proxy.h interfaces/org.freedesktop.login1.Session.xml
${XML2CPP} --verbose --proxy=${PROXY}/Login1User_proxy.h interfaces/org.freedesktop.login1.User.xml

${XML2CPP} --verbose --proxy=${PROXY}/Hostname1_proxy.h interfaces/org.freedesktop.hostname1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Locale1_proxy.h interfaces/org.freedesktop.locale1.xml
${XML2CPP} --verbose --proxy=${PROXY}/Timedate1_proxy.h interfaces/org.freedesktop.timedate1.xml

${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2Manager_proxy.h interfaces/org.freedesktop.GeoClue2.xml
${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2Client_proxy.h interfaces/org.freedesktop.GeoClue2.Client.xml
${XML2CPP} --verbose --proxy=${PROXY}/GeoClue2ClientLocation_proxy.h interfaces/org.freedesktop.GeoClue2.Client.Location.xml

${XML2CPP} --verbose --proxy=${PROXY}/Resolve1Manager_proxy.h interfaces/org.freedesktop.resolve1.Manager.xml
${XML2CPP} --verbose --proxy=${PROXY}/Resolve1Link_proxy.h interfaces/org.freedesktop.resolve1.Link.xml

${XML2CPP} --verbose --proxy=${PROXY}/AvahiServer_proxy.h interfaces/org.freedesktop.Avahi.Server.xml
${XML2CPP} --verbose --proxy=${PROXY}/AvahiServer2_proxy.h interfaces/org.freedesktop.Avahi.Server2.xml
