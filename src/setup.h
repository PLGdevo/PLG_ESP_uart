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

// int cmdIndex;
void exeCmd(String message)
{
  if (message.startsWith("CH"))
  {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  }
}
void uartPIC()
{
  if (Serial2.available())
  {
    char c = Serial2.read();
    cmd += c;
    if (c == '\n')
    {
      exeCmd(cmd);
      DEBUG_PRINTLN(" PIC_gui");
      DEBUG_PRINTF("%s",cmd);
      cmd = "";
    } // end read uart2
  }
}
void oled()
{
  if (value == HIGH)
  {
    data = "on";
  }
  else // if(value == LOW)
  {
    data = "off";
  }
  if (value1 == HIGH)
  {
    data1 = "on";
  }
  else
  {
    data1 = "off";
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE); // dao nguoc man hinh 180'
  display.setCursor(30, 0);
  display.println(F("PLG.ERA"));
  display.setCursor(0, 10);
  display.println("RN1");
  display.setCursor(0, 20);
  display.println("RN2");
  display.setCursor(35, 10);
  display.println(data);
  display.setCursor(35, 20);
  display.println(data1);
  display.setCursor(0, 30);
  display.printf("%S",cmd);
  // display.setCursor(0, 40);
  // display.println(pass);

  // Update the display with the buffer
  display.display();
}
ERA_WRITE(V0)
{
  /* Get value from Virtual Pin 0 and write LED */
  value = param.getInt();
  // digitalWrite(LED_PIN, value ? HIGH : LOW);
  if (value == HIGH)
  {
    DEBUG_PRINTLN("--------------write data R1------------- ");
    DEBUG_PRINTLN(value);
    SENTLN("f");
  }
  else
  {
    DEBUG_PRINTLN("--------------write data R1------------- ");
    DEBUG_PRINTLN(value);
    SENTLN("d");
  }
}
ERA_WRITE(V1)
{
  /* Get value from Virtual Pin 0 and write LED */
  value1 = param.getInt();
  // digitalWrite(LED_PIN, value ? HIGH : LOW);
  if (value1 == HIGH)
  {
    DEBUG_PRINTLN("--------------write data R2------------- ");
    DEBUG_PRINTLN(value1);
    SENTLN("R2 1");
  }
  else
  {
    DEBUG_PRINTLN("--------------write data R2------------- ");
    DEBUG_PRINTLN(value1);
    SENTLN("R2 0");
  }
}
////////len chuagit 
