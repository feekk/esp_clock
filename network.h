#ifndef __NETWORK__H
#define __NETWORK__H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>

class ApModule {
    protected:
        String ssid;
        String pwd;
        IPAddress ip = IPAddress(192, 168, 1, 1); 
        IPAddress gateway = IPAddress(192, 168, 0, 1);
        IPAddress subnet = IPAddress(255, 255, 255, 0); 
        // 热点状态 fasle未启动 true已启动
        bool runingStatus;
    public:
        ApModule(String name, String password);
    public:
        void setSSIDAndPWD(String name, String password);
        String getSSID(void);
        String getPWD(void);
        IPAddress getIP(void);
        bool begin(void);
        void setRuning(bool s);
        bool isRuning(void);
};

class WifiModule {
    protected:
        String ssid;
        String pwd;
    public:
        WifiModule(String name, String password);
    public:
        void setSSIDAndPWD(String name, String password);
        String getSSID(void);
        String getPWD(void);
        void begin(void);
        void setAutoConnect(bool c);
        bool isConnected(void);
        void disconnect(void);
};

extern NTPClient ntpClient;
class NtpModule {
    protected:
        NTPClient* client;
        String serverName;
        // ntp计数器
        int count = 0;
        // ntp同步状态总开关 true开启 false关闭
        bool syncStatus = true;
        // ntp同步时间间隔/秒
        int syncCount = 300;
        // ntp快速同步状态 true开启 false关闭
        bool fastSyncStatus = true;
        // ntp快速同步时间间隔/秒
        int fastSyncCount = 3;
    public:
        NtpModule(NTPClient* c, String sn);
    public:
        String getServerName(void);
        void setSyncStatus(bool s);
        bool getSyncStatus(void);
        void setSyncCount(int c);
        int getSyncCount(void);
        void timingIncr(void);
        void begin(void);
        bool sync(void);
        unsigned long getTimestamp(void);
};

extern ESP8266WebServer webServer;
class WebModule {
    protected:
        int port=80;
        ESP8266WebServer* server;
    public:
        WebModule(ESP8266WebServer* svr);
    public:
        void on(const Uri &uri, std::function<void(void)> handler);
        void begin(void);
        void handleClient(void);
        void send(int code, String content_type, String content);
        ESP8266WebServer* getInstance(void);
};
#endif /* __NETWORK__H */