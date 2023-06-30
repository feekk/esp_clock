#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "network.h"

// 
// {{{ ApModule
// 
ApModule::ApModule(String name, String password){
    setSSIDAndPWD(name, password);
}

void ApModule::setSSIDAndPWD(String name, String password){
    ssid = name;
    pwd = password;
}
String ApModule::getSSID(void){
    return ssid;
}
String ApModule::getPWD(void){
    return pwd;
}
IPAddress ApModule::getIP(void){
    return ip;
}
bool ApModule::begin(void){
    WiFi.softAPConfig(ip, gateway, subnet);
    setRuning(WiFi.softAP(ssid, pwd));
    return isRuning();
}
void ApModule::setRuning(bool s){
    runingStatus = s;
}
bool ApModule::isRuning(void){
    return runingStatus;
}
// 
// ApModule }}}
// 

// 
// {{{ WifiModule
// 
WifiModule::WifiModule(String name, String password){
    setSSIDAndPWD(name, password);
}
void WifiModule::setSSIDAndPWD(String name, String password){
    ssid = name;
    pwd = password;
}
String WifiModule::getSSID(void){
    return ssid;
}
String WifiModule::getPWD(void){
    return pwd;
}
void WifiModule::begin(void){
    WiFi.begin(ssid, pwd);
}
void WifiModule::setAutoConnect(bool c){
    WiFi.setAutoConnect(c);
}
bool WifiModule::isConnected(void){
    return WiFi.status() == WL_CONNECTED;
}
void WifiModule::disconnect(void){
    WiFi.disconnect();
}
// 
// WifiModule }}}
// 

// 
// {{{ NtpModule
//
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP);

NtpModule::NtpModule(NTPClient* c, String sn){
    serverName = sn;
    client = c;
    client->setPoolServerName(serverName.c_str());
}
String NtpModule::getServerName(void){
    return serverName;
}
void NtpModule::setSyncStatus(bool s){
    syncStatus = s;
}
bool NtpModule::getSyncStatus(void){
    return syncStatus;
}
void NtpModule::setSyncCount(int c){
    syncCount = c;
}
int NtpModule::getSyncCount(void){
    return syncCount;
}
void NtpModule::begin(void){
    client->begin();
}
void NtpModule::timingIncr(void){
    if(!syncStatus) return;
    count++;
    if(count > (1<<32)){
        count = 0;
    }
}
bool NtpModule::sync(void){
    if(!syncStatus) return false;
    if(fastSyncStatus && count%fastSyncCount==0 && client->forceUpdate()){
        fastSyncStatus = false;
        return true;
    }
    if(count > syncCount){
        count = 0;
        return client->forceUpdate();
    }
    return false;
}
unsigned long NtpModule::getTimestamp(void){
    return client->getEpochTime();
}
// 
// NtpModule }}}
// }


// 
// {{{ WebModule
// 
ESP8266WebServer webServer;

WebModule::WebModule(ESP8266WebServer* svr){
    server = svr;
}
void WebModule::begin(void){
    server->begin(port);
}
void WebModule::on(const Uri &uri, std::function<void(void)> handler){
    server->on(uri, handler);
}
void WebModule::handleClient(void){
    server->handleClient();
}
void WebModule::send(int code, String content_type, String content){
    server->send(code, content_type, content);
}
ESP8266WebServer* WebModule::getInstance(void){
    return server;
}
//  
// WebModule }}}
// }
