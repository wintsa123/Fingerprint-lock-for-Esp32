#ifndef __SET_H__
#define __SET_H__

#include <Adafruit_Fingerprint.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <ESP32_Servo.h>
#include <DHT.h>
#include <NTPClient.h>
#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET
#include <Blinker.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <WiFiUdp.h>
int q = 300;
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  //屏幕接sda、scl接口
#define UP 27         //实体按键接口
#define DOWN 26
#define LEFT 25
#define RIGHT 33
DHT dht(5, DHT22);//温度传感器接口
#define mySerial Serial2   //指纹接urt2，esp芯片接1上电乱码
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo myServo;//舵机对象不用改
char auth[] = "*****";   //点灯秘钥自己设置
char ssid[] = "*******";   //WiFi名
char pswd[] = "********";   //WiFi密码
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);
#endif
