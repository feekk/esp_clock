#ifndef __DISPLAY__H
#define __DISPLAY__H

#include <Arduino.h>
#include <U8g2lib.h>

class Clock;

class DISPLAY_OLED_U8G2 {
    protected:
        U8G2 driver;
        void setDriver(U8G2 drv){
            driver = drv;
        }

        // 屏幕旋转， 0度 90度 180度 270度
        int rotation = 0;
    public:
        // 设置地址
        void setAddress(uint8_t adr){
            driver.setI2CAddress(adr);
        }
        // 屏幕翻转
        void setRotation(int val){
            if(val == 0){
                rotation = val;
                driver.setDisplayRotation(U8G2_R0);
            }
            if(val == 90){
                rotation = val;
                driver.setDisplayRotation(U8G2_R1);
            }
            if(val == 180){
                rotation = val;
                driver.setDisplayRotation(U8G2_R2);
            }
            if(val == 270){
                rotation = val;
                driver.setDisplayRotation(U8G2_R3);
            }
        }
        // 获取屏幕旋转度数
        int getRotation(void){
            return rotation;
        }
        // 设置亮度 0~255
        void setContrast(uint8_t val){
            driver.setContrast(val);
        }
        // 初始化显示器, 清屏, 唤醒屏幕
        void begin(void){
            driver.begin();
        }
        // 清屏
        void clear(void){
            driver.clearBuffer();
        }
        // 发送缓冲区的内容到显示器
        void send(void){
            driver.sendBuffer();
        }
        // 开启Arduino平台下支持输出UTF8字符集
        void enableUTF8(void){
            driver.enableUTF8Print();
        }
        // 开机时展示的内容(可以是欢迎内容，其他信息等)
        virtual void displayOnStart(Clock* clk) = 0;
        // 具体显示器内容排版实现
        virtual void display(Clock* clk) = 0;
};

class DISPLAY_OLED_U8G2_SSD1306_128X32: public DISPLAY_OLED_U8G2 {
    public:
        DISPLAY_OLED_U8G2_SSD1306_128X32(uint8_t sck, uint8_t sda){
            U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, sck, sda, U8X8_PIN_NONE);
            setDriver(u8g2);
        }
        void displayOnStart(Clock* clk);
        void display(Clock* clk);
    private:
        void showDate(Clock* clk);
        void showTime(Clock* clk);
        void showApIcon(Clock* clk);
        void showWifiIcon(Clock* clk);
};

class DISPLAY_OLED_U8G2_SSD1306_128X64: public DISPLAY_OLED_U8G2 {
    public:
        DISPLAY_OLED_U8G2_SSD1306_128X64(uint8_t sck, uint8_t sda){
            U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, sck, sda, U8X8_PIN_NONE);
            setDriver(u8g2);
        }
        void displayOnStart(Clock* clk);
        void display(Clock* clk);
    private:
        void showDate(Clock* clk);
        void showTime(Clock* clk);
        void showApIcon(Clock* clk);
        void showWifiIcon(Clock* clk);
};


#endif /* __DISPLAY__H */