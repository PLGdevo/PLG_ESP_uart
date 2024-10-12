#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "d58ef8ad-326a-4889-844a-39a16fdcb75b"


#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char ssid[] = "PLG.mcu";
const char pass[] = "ok11111111";
// const char ssid[] = "MakerSpaceLab";
// const char pass[] = "Maker2017";
ERaTimer timer;
// /****************************OLED**********************************
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//  **************************************************************/

void timerEvent()
{
  ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}

// // #include <Preferences.h>
// /**************************************************************
//  *                UART1                                       *
//  **************************************************************/

#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)     // Định nghĩa hàm in ra thông tin debug
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__) // Định nghĩa hàm in ra thông tin debug kèm xuống dòng
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)   // Định nghĩa hàm in ra thông tin debug theo định dạng
// /**************************************************************
//  *                UART_2                                      *
//  **************************************************************/
#define SENT(...) Serial2.print(__VA_ARGS__)     // Định nghĩa hàm in ra thông tin debug
#define SENTLN(...) Serial2.println(__VA_ARGS__) // Định nghĩa hàm in ra thông tin debug kèm xuống dòng
#define SENTF(...) Serial2.printf(__VA_ARGS__)   // Định nghĩa hàm in ra thông tin debug theo định dạng

String data = "";
string datapic = "";
String data1 = "";
uint8_t value1;
uint8_t value;
String cmd = "";


