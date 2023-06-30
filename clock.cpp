#include <Arduino.h>
#include <TimeLib.h>
#include "clock.h"
#include "display.h"
#include "network.h"

// 时钟前置时间加0
String twoDigits(int digits) {
    if (digits < 10) {
        String i = '0' + String(digits);
        return i;
    }else {
        return String(digits);
    }
}

Clock::Clock(void){
    setTimestamp(946656000);
}
void Clock::setWifiModule(WifiModule* m){
    wifiModule = m;
}
WifiModule* Clock::getWifiModule(void){
    return wifiModule;
}
void Clock::setApModule(ApModule* m){
    apModule = m;
}
ApModule* Clock::getApModule(void){
    return apModule;
}
void Clock::setNtpModule(NtpModule* m){
    ntpModule = m;
}
NtpModule* Clock::getNtpModule(void){
    return ntpModule;
}

void Clock::ntpSync(void){
    if(!getWifiModule()) return;
    if(!getNtpModule()) return;
    if(getWifiModule()->isConnected() && getNtpModule()->sync()){
        setTimestamp(getNtpModule()->getTimestamp());
    }
}

void Clock::setDisplayModule(DISPLAY_OLED_U8G2* m){
    displayModule = m;
}
DISPLAY_OLED_U8G2* Clock::getDisplayModule(void){
    return displayModule;
}

void Clock::setWebModule(WebModule* m){
    webModule = m;
}
WebModule* Clock::getWebModule(void){
    return webModule;
}

void Clock::setDateTime(int yr, int month, int day, int hr, int min, int sec){
    setTime(hr, min, sec, day, month, yr);
    unsigned long nowTimestamp = (unsigned long)now() - timeZone*SECS_PER_HOUR;
    setTimestamp(nowTimestamp);
}
void Clock::setTimestamp(unsigned long t){
    timestamp = t;
    setTime((time_t)(t + timeZone*SECS_PER_HOUR));
}
unsigned long Clock::getTimestamp(void){
    return timestamp;
}

void Clock::setTimeZone(uint8_t tz){
    timeZone = tz;
}
uint8_t Clock::getTimeZone(void){
    return timeZone;
}

int Clock::getYear(void){
    return year();
}
String Clock::getMonth(void){
    return twoDigits(month());
}
String Clock::getDay(void){
    return twoDigits(day());
}
String Clock::getHour(void){
    return twoDigits(hour());
}
String Clock::getMinute(void){
    return twoDigits(minute());
}

void Clock::setTimeSeparatorFlicker(bool f){
    timeFlickerStatus = f;
}
bool Clock::timeSeparatorIsFlicker(void){
    return timeFlickerStatus;
}

String Clock::webHtmlSystemConfig(void){
    String code = "";
    code += "<form action=\"\">";
    code += "<h3>系统配置</h3>";
    if(getApModule()){
        code += "<label>AP_SSID</label>";
        code += "<input type=\"text\"name=\"ap_ssid\"value=\""+getApModule()->getSSID()+"\"disabled/>";
        code += "<br />";
        code += "<label>AP_密码</label>";
        code += "<input type=\"text\"name=\"ap_pwd\"value=\""+getApModule()->getPWD()+"\"disabled/>";
        code += "<br>";
    }
    if( getDisplayModule()){
        int rotate = getDisplayModule()->getRotation();
        String select0Str   = rotate==0   ? "selected": "";
        String select90Str  = rotate==90  ? "selected": "";
        String select180Str = rotate==180 ? "selected": "";
        String select270Str = rotate==270 ? "selected": "";
        code += "<label>屏幕翻转</label>";
        code += "<select name=\"screen_flip\">";
        code += "<option value=\"0\" " + select0Str + ">0°</option>";
        code += "<option value=\"90\" " + select90Str + ">90°</option>";
        code += "<option value=\"180\" " + select180Str + ">180°</option>";
        code += "<option value=\"270\" " + select270Str + ">270°</option>";
        code += "</select>";
        code += "<br>";
    }
    if(code != ""){
        code += "<button>保存</button>";
        code += "</form>";
    }
    return code;
}
String Clock::webHtmlWifiConfig(void){
    String code = "";
    if(getWifiModule()){
        code += "<form action=\"\">";
        code += "<h3>WIFI配置</h3>";
    }
    if(getWifiModule()){
        code += "<label>WIFI_SSID</label>";
        code += "<input type=\"text\" name=\"wifi_ssid\" value=\"" + getWifiModule()->getSSID() + "\"/>";
        code += "<br />";
        code += "<label>WIFI_密码</label>";
        code += "<input type=\"text\" name=\"wifi_pwd\" value=\"" + getWifiModule()->getPWD() + "\"/>";
        code += "<br>";
    }
    if(code != ""){
        code += "<button>保存</button>";
        code += "</form>";
    }
    return code;
}
String Clock::webHtmlTimeConfig(void){
    String code = "";
    code += "<form action=\"\">";
    code += "<h3>时间配置</h3>";

    String datetime = String(getYear()) + "-" + getMonth() + "-" + getDay() +"T" + getHour() + ":" + getMinute();
    code += "<label>时钟设置</label>";
    code += "<input type=\"datetime-local\" name=\"clock_time\" value=\"" + datetime + "\">";
    code += "<br />";

    String timezoneStr = String(getTimeZone());
    code += "<label>时区（时）</label>";
    code += "<input type=\"text\" name=\"clock_time_zone\" value=\"" + timezoneStr + "\" disabled/>";
    code += "<br>";

    if( getNtpModule()){
        
        String checkOpenStr = getNtpModule()->getSyncStatus() ? "checked": "";
        String checkCloseStr = getNtpModule()->getSyncStatus() ? "": "checked";

        code += "<label>NTP同步</label>";
        code += "<input type=\"radio\" name=\"clock_ntp\" value=1 " + checkOpenStr + "/>";
        code += "<label>启动</label>";
        code += "<input type=\"radio\" name=\"clock_ntp\" value=0 " + checkCloseStr + "/>";
        code += "<label>禁止</label>";
        code += "<br />";

        String intervalStr = String(getNtpModule()->getSyncCount());
        code += "<label>NTP同步间隔(秒)</label>";
        code += "<input type=\"text\" name=\"clock_ntp_interval\" value=\"" + intervalStr + "\"/>";
        code += "<br />";

        String address = getNtpModule()->getServerName();
        code += "<label>NTP地址</label>";
        code += "<input type=\"text\" name=\"clock_ntp_addr\" value=\"" + address + "\" disabled/>";
        code += "<br>";
    }
    if(code != ""){
        code += "<button>保存</button>";
        code += "</form>";
    }
    return code;
}

String Clock::webHtml(void){
    String htmlCode = "<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title>配置页面</title></head><body><div>";
    htmlCode += webHtmlSystemConfig();
    htmlCode += webHtmlWifiConfig();
    htmlCode += webHtmlTimeConfig();   
    htmlCode += "</div></body></html>";   
    return htmlCode;
}

void Clock::webConfig(void){
    if(!getWebModule()) return;
    // 处理数据提交逻辑
    if(getWebModule()->getInstance()->args() > 0) {

        // 屏幕设置
        if(getDisplayModule()){
            if(getWebModule()->getInstance()->hasArg("screen_flip")){
                // 屏幕角度
                getDisplayModule()->setRotation((int)getWebModule()->getInstance()->arg("screen_flip").toInt());
            }
        }
        // WIFI设置
        if(getWifiModule()){
            if(getWebModule()->getInstance()->hasArg("wifi_ssid") && getWebModule()->getInstance()->hasArg("wifi_pwd")){
                // wifi账号、密码设置
                getWifiModule()->setSSIDAndPWD(getWebModule()->getInstance()->arg("wifi_ssid"), getWebModule()->getInstance()->arg("wifi_pwd"));
                getWifiModule()->disconnect();
                getWifiModule()->begin();
                getWifiModule()->setAutoConnect(true);
            }
        }
        // 时钟设置
        if(getWebModule()->getInstance()->hasArg("clock_time")){
            // 时间设置
            String  clockTime = getWebModule()->getInstance()->arg("clock_time");
            int year = (clockTime.substring(0,4)).toInt();
            int month = (clockTime.substring(5,7)).toInt();
            int day = (clockTime.substring(8,10)).toInt();
            int hour = (clockTime.substring(11,13)).toInt();
            int min = (clockTime.substring(14,16)).toInt();
            setDateTime(year, month, day, hour, min, 0);
        }
        // NTP设置
        if(getNtpModule()){
            if(getWebModule()->getInstance()->hasArg("clock_ntp")){
                // ntp开关设置
                if(getWebModule()->getInstance()->arg("clock_ntp").toInt() > 0){
                    getNtpModule()->setSyncStatus(true);
                }else{
                    getNtpModule()->setSyncStatus(false);
                }
            }
            if(getWebModule()->getInstance()->hasArg("clock_ntp_interval")){
                // ntp同步间隔
                getNtpModule()->setSyncCount((int)getWebModule()->getInstance()->arg("clock_ntp_interval").toInt());
            }
        }

        getWebModule()->getInstance()->sendHeader("Location", "/", true);
        getWebModule()->getInstance()->send(302);
        return;
    }

    getWebModule()->send(200, "text/html", webHtml());
}