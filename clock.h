#ifndef __CLOCK__H
#define __CLOCK__H
#include <Arduino.h>
#include "display.h"
#include "network.h"

class Clock {
    protected:
        // 时间戳/秒
        unsigned long timestamp;
        // 时区
        uint8_t timeZone = 8;
        // 时分间隔符闪烁false不显示 true显示
        bool timeFlickerStatus;
        // wifiModule
        WifiModule* wifiModule;
        // apModule
        ApModule* apModule;
        // ntpModule
        NtpModule* ntpModule;
        // webModule
        WebModule* webModule;
        // display
        DISPLAY_OLED_U8G2* displayModule;
    public:
        Clock(void);
    public:
        void setWifiModule(WifiModule* m);
        WifiModule* getWifiModule(void);

        void setApModule(ApModule* m);
        ApModule* getApModule(void);

        void setNtpModule(NtpModule* m);
        NtpModule* getNtpModule(void);
        void ntpSync(void);

        void setDisplayModule(DISPLAY_OLED_U8G2* m);
        DISPLAY_OLED_U8G2* getDisplayModule(void);

        void setWebModule(WebModule* m);
        WebModule* getWebModule(void);

        // 设置时间，加上时区
        void setDateTime(int yr, int month, int day, int hr, int min, int sec);
        // 设置时钟时间戳/秒
        void setTimestamp(unsigned long t);
        // 获取时钟时间戳/秒
        unsigned long getTimestamp(void);

        // 设置时钟时区
        void setTimeZone(uint8_t tz);
        // 获取时钟时区
        uint8_t getTimeZone(void);

        // 获取年份 YYYY
        int getYear(void);
        // 获取月份 01~12
        String getMonth(void);
        // 获取日期 01~31
        String getDay(void);
        // 获取小时 00~23
        String getHour(void);
        // 获取分钟 00~59
        String getMinute(void);

        // 设置时间闪烁
        void setTimeSeparatorFlicker(bool f);
        // 时间是否闪烁
        bool timeSeparatorIsFlicker(void);

        // html
        String webHtml(void);
        void webConfig(void);
    protected:
        String webHtmlSystemConfig(void);
        String webHtmlWifiConfig(void);
        String webHtmlTimeConfig(void);
};


#endif /* __CLOCK__H */