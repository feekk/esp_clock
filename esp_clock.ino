#include <Ticker.h>
#include "clock.h"
#include "display.h"
#include "network.h"

// 固件版本
#define CLOCK_VERSION "202304201836"

// oled i2c address
#define OLED_ADDR 0x78

// nodemcu pin
#define NodeMCU_PIN_D1 5
#define NodeMCU_PIN_D2 4

// esp01s pin
#define Esp01s_PIN_GPIO0 0
// #define Esp01s_PIN_TX 1
#define Esp01s_PIN_GPIO2 2
#define Esp01s_PIN_RX 3


// clock
Clock clk;
// 显示屏
DISPLAY_OLED_U8G2_SSD1306_128X32 screen(Esp01s_PIN_GPIO2, Esp01s_PIN_GPIO0);
// DISPLAY_OLED_U8G2_SSD1306_128X64 screen(Esp01s_PIN_GPIO2, Esp01s_PIN_GPIO0);

// ap 
ApModule ap("{ap_ssid}", "{ap_password}");
// wifi
WifiModule wifi("{wifi_ssid}", "{wifi_password}");
// ntp
NtpModule ntp(&ntpClient, "ntp.ntsc.ac.cn");  
// web
WebModule web(&webServer);
void webConfig(void){
    clk.webConfig();
}

// 
// ticker
// 
Ticker tickerPerSecond;
// 每秒定时器
void tickerPerSecondCallback(void) {
    clk.setTimestamp(clk.getTimestamp()+1);
    clk.setTimeSeparatorFlicker(!clk.timeSeparatorIsFlicker());
    clk.getNtpModule()->timingIncr();
    Serial.println("[INFO] tickerPerSecondCallback finish");
}

static char ntpServerName[] = "ntp.ntsc.ac.cn";

void setup() {
    delay(100);

    // 启动串口
    Serial.begin(115200);
    Serial.println("setup start");

    // 连接wifi
    wifi.begin();
    wifi.setAutoConnect(true);
    clk.setWifiModule(&wifi);
    Serial.println("[INFO] wifi init ok");


    // 开启AP
    if( ap.begin()){
        Serial.println("[INFO] ap begin success.");
    }else{
        Serial.println("[WARNING] ap begin fail.");
    }
    clk.setApModule(&ap);
    Serial.println("[INFO] ap init ok");

    // 启动ntp
    ntp.begin();
    clk.setNtpModule(&ntp);
    Serial.println("[INFO] ntp init ok");

    // 启动web
    web.on("/", webConfig);
    web.begin();
    clk.setWebModule(&web);
    Serial.println("[INFO] web init ok");

    // 屏幕配置
    // 设置屏幕地址
    screen.setAddress(OLED_ADDR);
    screen.setRotation(180);
    // 设置屏幕亮度，经测试无效
    // screen.setContrast(50);
    // 唤醒，清屏
    screen.begin();
    clk.setDisplayModule(&screen);
    Serial.println("[INFO] screen init ok");

    // 定时1s
    tickerPerSecond.attach(1, tickerPerSecondCallback);
    Serial.println("[INFO] start tickerPerSecond ok.");

    // 开机展示内容
    clk.getDisplayModule()->displayOnStart(&clk);
}

void loop() {
    // 要放loop里，不能放ticker，不然会报错
    clk.ntpSync();
    clk.getWebModule()->handleClient();
    clk.getDisplayModule()->display(&clk);
    delay(100);
}