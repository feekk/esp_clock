#include <Arduino.h>
#include <U8g2lib.h>
#include "display.h"
#include "clock.h"

// 
// {{{ DISPLAY_OLED_U8G2_SSD1306_128X32
// 
void DISPLAY_OLED_U8G2_SSD1306_128X32::displayOnStart(Clock* clk){
    if(clk->getApModule()){
        clear();
        // Weclome
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 12);
        driver.print("Network Clock");
        // WIFI name
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 30);
        driver.print("WIFI");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(32, 26, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 30);
        driver.print(clk->getApModule()->getSSID());
        send();
        delay(5000);

        clear();
        // IP
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 12);
        driver.print("IP");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(16, 10, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 12);
        driver.print(clk->getApModule()->getIP());
        // PWD
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 30);
        driver.print("Pwd");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(30, 26, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 30);
        driver.print(clk->getApModule()->getPWD());
        send();
        delay(5000);
    }
}

void DISPLAY_OLED_U8G2_SSD1306_128X32::display(Clock* clk){
    clear();
    showDate(clk);
    showTime(clk);
    showApIcon(clk);
    showWifiIcon(clk);
    send();
}

void DISPLAY_OLED_U8G2_SSD1306_128X32::showDate(Clock* clk){
    driver.setFont(u8g2_font_crox2tb_tn);
    // month
    driver.setCursor(0, 14);
    driver.print(clk->getMonth());
    // day
    driver.setCursor(22, 14);
    driver.print(clk->getDay());
    // year
    driver.setCursor(4, 28);
    driver.print(clk->getYear());
    // separator
    driver.setFont(u8g2_font_7x13_t_symbols);
    driver.drawGlyph(15, 14, 0x002f);
}
void DISPLAY_OLED_U8G2_SSD1306_128X32::showTime(Clock* clk) {
    driver.setFont(u8g2_font_VCR_OSD_mn);
    // hour
    driver.setCursor(48, 24);
    driver.print(clk->getHour());
    // minute
    driver.setCursor(80, 24);
    driver.print(clk->getMinute());
    // flicker
    driver.setFont(u8g2_font_9x15_t_symbols);
    if(clk->timeSeparatorIsFlicker()) {
        driver.drawGlyph(70, 20, 0x003a);
    }else{
        driver.drawGlyph(70, 20, 0x0020);
    }
}
void DISPLAY_OLED_U8G2_SSD1306_128X32::showApIcon(Clock* clk){
    if(!clk->getApModule()) return;
    if(clk->getApModule()->isRuning()){
        driver.setFont(u8g2_font_siji_t_6x10); 
        driver.drawGlyph(116, 20, 0xe02d);
    }
}
void DISPLAY_OLED_U8G2_SSD1306_128X32::showWifiIcon(Clock* clk){
    if(!clk->getWifiModule()) return;
    if(clk->getWifiModule()->isConnected()){
        driver.setFont(u8g2_font_siji_t_6x10); 
        driver.drawGlyph(116, 8, 0xe21a);
    }
}
// 
// DISPLAY_OLED_U8G2_SSD1306_128X32 }}}
// 


// 
// {{{ DISPLAY_OLED_U8G2_SSD1306_128X64
// 

void DISPLAY_OLED_U8G2_SSD1306_128X64::displayOnStart(Clock* clk){
    if(clk->getApModule()){
        clear();
        // WIFI name
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 12);
        driver.print("WIFI");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(32, 10, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 12);
        driver.print(clk->getApModule()->getSSID());

        // IP
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 30);
        driver.print("IP");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(16, 26, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 30);
        driver.print(clk->getApModule()->getIP());
        // PWD
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(0, 48);
        driver.print("Pwd");
        driver.setFont(u8g2_font_7x13_t_symbols);
        driver.drawGlyph(30, 44, 0x003a);
        driver.setFont(u8g2_font_helvB10_tf);
        driver.setCursor(40, 48);
        driver.print(clk->getApModule()->getPWD());
        send();
        delay(5000);
    }
}

void DISPLAY_OLED_U8G2_SSD1306_128X64::display(Clock* clk){
    clear();
    showDate(clk);
    showTime(clk);
    showApIcon(clk);
    showWifiIcon(clk);
    send();
}

void DISPLAY_OLED_U8G2_SSD1306_128X64::showDate(Clock* clk){
    driver.setFont(u8g2_font_crox2tb_tn);
    // month
    driver.setCursor(0, 14);
    driver.print(clk->getMonth());
    // day
    driver.setCursor(22, 14);
    driver.print(clk->getDay());
    // year
    driver.setCursor(4, 28);
    driver.print(clk->getYear());
    // separator
    driver.setFont(u8g2_font_7x13_t_symbols);
    driver.drawGlyph(15, 14, 0x002f);
}
void DISPLAY_OLED_U8G2_SSD1306_128X64::showTime(Clock* clk) {
    driver.setFont(u8g2_font_VCR_OSD_mn);
    // hour
    driver.setCursor(48, 24);
    driver.print(clk->getHour());
    // minute
    driver.setCursor(80, 24);
    driver.print(clk->getMinute());
    // flicker
    driver.setFont(u8g2_font_9x15_t_symbols);
    if(clk->timeSeparatorIsFlicker()) {
        driver.drawGlyph(70, 20, 0x003a);
    }else{
        driver.drawGlyph(70, 20, 0x0020);
    }
}
void DISPLAY_OLED_U8G2_SSD1306_128X64::showApIcon(Clock* clk){
    if(!clk->getApModule()) return;
    if(clk->getApModule()->isRuning()){
        driver.setFont(u8g2_font_siji_t_6x10); 
        driver.drawGlyph(116, 20, 0xe02d);
    }
}
void DISPLAY_OLED_U8G2_SSD1306_128X64::showWifiIcon(Clock* clk){
    if(!clk->getWifiModule()) return;
    if(clk->getWifiModule()->isConnected()){
        driver.setFont(u8g2_font_siji_t_6x10); 
        driver.drawGlyph(116, 8, 0xe21a);
    }
}
// 
// DISPLAY_OLED_U8G2_SSD1306_128X64 }}}
// 